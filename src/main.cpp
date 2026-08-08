#include <platform/Xbox/XboxSystem.h>
#include <platform/Xbox/XboxInput.h>
#include <title/TitleManager.h>

#include <hooks/PowerPCHookService.h>
#include "platform/Xbox/XboxInput.h"

hooks::PowerPCHookService *g_hooks;
platform::XboxSystem g_platform;
platform::XboxInput g_input;
core::TitleManager *g_pManager = nullptr;
HANDLE g_threadHandle = INVALID_HANDLE_VALUE;

DWORD WINAPI Attach(HANDLE hModule) {
    g_pManager = new core::TitleManager(g_platform, g_input, *g_hooks);
    g_pManager->run();
    return 0;
}

extern "C" {
    BOOL WINAPI _CRT_INIT(HINSTANCE hDll, DWORD reason, LPVOID reserved);
}


extern "C" BOOL WINAPI DllMain(HINSTANCE hModule, DWORD reason, void *pReserved) {
    switch (reason) {
        case DLL_PROCESS_ATTACH :
            if (!_CRT_INIT(hModule, reason, pReserved)) {
                std::cerr << "_CRT_INIT did not run";
                return false;
            }

            ExCreateThread(&g_threadHandle, 0, nullptr, nullptr,
                reinterpret_cast<PTHREAD_START_ROUTINE>(Attach), nullptr, 2
            );
            break;
        case DLL_PROCESS_DETACH :
            std::cerr << "Detaching dll";
            if (g_pManager) g_pManager->stop();
            WaitForSingleObject(g_threadHandle, INFINITE);
            CloseHandle(g_threadHandle);
            delete g_pManager;

            _CRT_INIT(hModule, reason, pReserved);
            break;
    }
    return true;
}