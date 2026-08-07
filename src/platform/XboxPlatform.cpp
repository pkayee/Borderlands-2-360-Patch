//
// Created by pixel on 8/5/2026.
//

#include "XboxPlatform.h"

typedef void (*XNOTIFYQUEUEUI)(uint32_t type, uint32_t userIndex, uint64_t areas, const wchar_t *displayText, void *pContextData);

void *ResolveFunction(const std::string &moduleName, uint32_t ordinal) {
    HMODULE moduleHandle = GetModuleHandle(moduleName.c_str());
    if (moduleHandle == nullptr)
        return nullptr;
    return GetProcAddress(moduleHandle, reinterpret_cast<const char *>(ordinal));
}

static XNOTIFYQUEUEUI XNotifyQueueUI = static_cast<XNOTIFYQUEUEUI>(ResolveFunction("xam.xex", 656));

namespace platform {
    void XboxPlatform::reboot() {

    }

    void XboxPlatform::notify(const std::wstring &message) {
        std::cerr << "Platform notify called";
        XNotifyQueueUI(0 , 0, XNOTIFY_SYSTEM, message.c_str(), nullptr);
    }

    uint32_t XboxPlatform::getFirmwareVersion() {
        return XboxKrnlVersion->Build;
    }

    bool XboxPlatform::isDevkit() {
        return false;
    }

    std::string XboxPlatform::getTitleUpdateVersion() {
        std::cerr << "\n" << "Platform title version called ";
        HANDLE hGameModule = GetModuleHandle(NULL);

        LDR_DATA_TABLE_ENTRY *pDataTable = static_cast<LDR_DATA_TABLE_ENTRY *>(hGameModule);
        XEX_EXECUTION_ID *pExecutionId = static_cast<XEX_EXECUTION_ID *>(
            RtlImageXexHeaderField(pDataTable->XexHeaderBase, XEX_HEADER_FIELD_EXECUTION_ID)
        );

        return XexUtils::Formatter::Format(
           "v%hhu.%hhu.%hhu",
           pExecutionId->Version.Major,
           pExecutionId->Version.Minor,
           pExecutionId->Version.Qfe
        );
    }
}
