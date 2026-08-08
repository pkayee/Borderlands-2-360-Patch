#pragma once
#include <xtl.h>

namespace hooks {
    class IHookService {
    public:
        virtual ~IHookService() {}

        virtual DWORD install(DWORD address, void *destination) = 0;
        virtual void* callOriginal(DWORD handle, void *a1, void *a2, void *a3) = 0;
        virtual void uninstall(DWORD handle) = 0;
        virtual bool isInstalled(DWORD handle) = 0;
    };
}