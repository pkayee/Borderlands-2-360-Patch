#include "PowerPCHookService.h"
#include "iostream"
#include <XexUtils.h>
#include <hooks/PowePCPatching.h>

#ifndef DECLSPEC_IMPORT
#define DECLSPEC_IMPORT
#endif

extern "C" {
    DECLSPEC_IMPORT void NTAPI ExFreePool(PVOID P);
    DECLSPEC_IMPORT void* NTAPI ExAllocatePoolWithTag(DWORD PoolType, SIZE_T NumberOfBytes, DWORD Tag);
    DECLSPEC_IMPORT DWORD __stdcall MmQueryAddressProtect(PVOID Address);
    DECLSPEC_IMPORT NTSTATUS __stdcall MmSetAddressProtect(PVOID Address, SIZE_T Size, ULONG NewProtect);
}


namespace hooks {
    DWORD PowerPCHookService::install(DWORD address, void *destination) {
        if (!MmIsAddressValid((PVOID)address)) {
            std::cerr << "\n[PowerPCHookService::install] - Address:" <<address << " is not valid!";
            return 0;
        }

        HookEntry& entry = m_hooks[address];
        memcpy(entry.originalASM, (PVOID)address, 0x10);
        void* stubMem = ExAllocatePoolWithTag(0, 0x40, 'Hook');
        if (!stubMem) {
            std::cerr << "\n[PowerPCHookService::install] - Failed to allocate stub memory!";
            m_hooks.erase(address);
            return 0;
        }

        entry.stubASM = (BYTE*)stubMem;

        DWORD stubOffset = 0;
        for (int i = 0; i < 4; i++) {
            DWORD instrAddr = address + (i* 4);
            DWORD instr = *(DWORD*)instrAddr;
            BYTE opcode = (BYTE)((instr >> 24) & 0xFF);

            if (opcode == 0x48 || opcode == 0x4B) {
                int target = m_ppcPatching.GetBranchCall(instrAddr);
                bool isLinked = (instr & 1) != 0;

                std::cerr << "\n[PowerPCHookService::install] - Branch found at +0x"
                << std::hex << (i*4)
                << ", target: 0x" << target;

                m_ppcPatching.PatchInJump((PDWORD)(entry.stubASM + stubOffset), (void*)target, isLinked ? TRUE : FALSE);
                stubOffset += 0x10;

                if (!isLinked) {
                    break;
                } else {
                    *(DWORD*)(entry.stubASM + stubOffset) = instr;
                    stubOffset += 4;
                }
            }
        }

        if (stubOffset < 0x30) {
            m_ppcPatching.PatchInJump((PDWORD)(entry.stubASM + stubOffset), (PVOID)(address + 0x10), FALSE);
        }

        DWORD currentProtect = MmQueryAddressProtect((PVOID)address);
        std::cerr << "\n[PowerPCHookService::install] - Current protect: 0x" << std::hex << currentProtect;

        DWORD oldProtect = currentProtect;
        NTSTATUS status = MmSetAddressProtect((PVOID)address, 0x10, PAGE_EXECUTE_READWRITE);
        std::cerr << "\n[PowerPCHookService::install] - MmSetAddressProtect status: " << status;

        if (!NT_SUCCESS(status)) {
            std::cerr << "\n[PowerPCHookService::install] - Protection change FAILED, aborting!";
            ExFreePool(entry.stubASM);
            m_hooks.erase(address);
            return 0;
        }

        m_ppcPatching.PatchInJump((PDWORD)address, destination, false);
        MmSetAddressProtect((PVOID)address, 0x10, oldProtect);
        __sync();

        std::cerr << "\n[PowerPCHookService::install] - Address:" <<address << " hooked!";
        return address;
    }

    void *PowerPCHookService::callOriginal(DWORD handle, void *a1, void *a2, void *a3) {
        std::map<DWORD, HookEntry>::iterator it = m_hooks.find(handle);

        if (it == m_hooks.end() || !it->second.stubASM) {
            return NULL;
        }

        typedef void* (*StubFn)(void*, void*, void*);
        StubFn stub = (StubFn)(void*)it->second.stubASM;
        return stub(a1, a2, a3);
    }

    void PowerPCHookService::uninstall(DWORD handle) {
        std::map<DWORD, HookEntry>::iterator it = m_hooks.find(handle);

        if (it != m_hooks.end() && MmIsAddressValid((PVOID)handle)) {
            memcpy((PVOID)handle, it->second.originalASM, 0x10);

            KeFlushCacheRange((PVOID)handle, 0x10);
            __sync();

            if (it->second.stubASM) {
                ExFreePool(it->second.stubASM);
            }

            m_hooks.erase(it);
            std::cerr << "\n[PowerPCHookService::uninstall] - Hook Uninstalled";
        }
    }

    bool PowerPCHookService::isInstalled(DWORD handle) {
        return m_hooks.find(handle) != m_hooks.end();
    }
}
