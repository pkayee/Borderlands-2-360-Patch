#pragma once
#include <hooks/IHookService.h>
#include <hooks/PowePCPatching.h>
#include <map>

namespace hooks {
    class PowerPCHookService : public IHookService {
    public:
        PowerPCHookService(PowerPCPatching &ppcPatching) : m_ppcPatching(ppcPatching) {}
        virtual DWORD install(DWORD address, void *destination);
        virtual void* callOriginal(DWORD handle, void *a1, void *a2, void *a3);
        virtual void uninstall(DWORD handle);
        virtual bool isInstalled(DWORD handle);
    private:
        struct HookEntry {
            BYTE originalASM[0x10];
            BYTE* stubASM;
        };
        std::map<DWORD, HookEntry> m_hooks;
        PowerPCPatching &m_ppcPatching;
    };
}
