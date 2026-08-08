#pragma once
#include <hooks/IHookService.h>
#include <hooks/PowePCPatching.h>
#include <map>

namespace hooks {
    class PowerPCHookService : public IHookService {
    public:
        virtual DWORD install(DWORD address, void *destination) = 0;
        virtual void* callOriginal(DWORD handle, void *a1, void *a2, void *a3) = 0;
        virtual void uninstall(DWORD handle) = 0;
        virtual bool isInstalled(DWORD handle) = 0;
    private:
        struct HookEntry {
            BYTE originalASM[0x10];
            BYTE stubASM[0x20];
        };
        std::map<DWORD, HookEntry> m_hooks;
        PowerPCPatching &m_ppcPatching;
    };
}
