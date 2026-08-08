#include "PowerPCHookService.h"
#include "iostream"
#include <XexUtils.h>
#include <hooks/PowePCPatching.h>

namespace hooks {
    DWORD PowerPCHookService::install(DWORD address, void *destination) {
        if (!MmIsAddressValid((PVOID)address)) {
            std::cerr << "\n[PowerPCHookService::install] - Address:" <<address << " is not valid!";
            return 0;
        }

        HookEntry entry;
        memcpy(entry.originalASM, (PVOID)address, 0x10);
        memcpy(entry.stubASM, (PVOID)address, 0x10);

        m_ppcPatching.PatchInJump((PDWORD)(entry.stubASM + 0x10), (PVOID)(address + 0x10), FALSE);
        m_ppcPatching.PatchInJump((PDWORD)address, destination, FALSE);

        m_hooks[address] = entry;
        std::cerr << "\n[PowerPCHookService::install] - Address:" <<address << " hooked!";
        return address;
    }

    void *PowerPCHookService::callOriginal(DWORD handle, void *a1, void *a2, void *a3) {
        std::map<DWORD, HookEntry>::iterator it = m_hooks.find(handle);

        if (it == m_hooks.end()) {
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
            m_hooks.erase(it);
            std::cerr << "\n[PowerPCHookService::uninstall] - Hook Uninstalled";
        }
    }

    bool PowerPCHookService::isInstalled(DWORD handle) {
        return m_hooks.find(handle) != m_hooks.end();
    }
}
