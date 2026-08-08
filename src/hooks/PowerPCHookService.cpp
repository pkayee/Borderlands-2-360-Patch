#include "PowerPCHookService.h"
#include "iostream"
#include <XexUtils.h>
#include <hooks/PowePCPatching.h>

extern "C" {
    DECLSPEC_IMPORT void* NTAPI ExAllocatePoolWithTag(DWORD PoolType, SIZE_T NumberOfBytes, DWORD Tag);
    DECLSPEC_IMPORT void NTAPI ExFreePool(PVOID P);
}


namespace hooks {
    DWORD PowerPCHookService::install(DWORD address, void *destination) {
        if (!MmIsAddressValid((PVOID)address)) {
            std::cerr << "\n[PowerPCHookService::install] - Address:" <<address << " is not valid!";
            return 0;
        }

        HookEntry& entry = m_hooks[address];
        memcpy(entry.originalASM, (PVOID)address, 0x10);
        void* stubMem = ExAllocatePoolWithTag(0, 0x20, 'Hook');
        if (!stubMem) {
            std::cerr << "\n[PowerPCHookService::install] - Failed to allocate stub memory!";
            return 0;
        }

        entry.stubASM = (BYTE*)stubMem;

        memcpy(entry.stubASM, (PVOID)address, 0x10);
        m_ppcPatching.PatchInJump((PDWORD)(entry.stubASM + 0x10), (PVOID)(address + 0x10), FALSE);

        KeFlushCacheRange(entry.stubASM, 0x20);
        __sync();

        DWORD oldProtect;
        VirtualProtect((PVOID)address, 0x10, PAGE_EXECUTE_READWRITE, &oldProtect);

        m_ppcPatching.PatchInJump((PDWORD)address, destination, false);

        VirtualProtect((PVOID)address, 0x10, oldProtect, &oldProtect);

        KeFlushCacheRange((PVOID)address, 0x10);
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
