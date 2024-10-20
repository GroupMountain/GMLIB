#pragma once
#include "Macros.h"
#include <Windows.h>
#include <optional>
#include <string>
#include <vector>

namespace gmlib::memory {

inline std::string GetModuleName(bool thisModule = true) {
    static char dummy  = 'x';
    HMODULE     module = NULL;
    if (thisModule) {
        GetModuleHandleExA(
            GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
            &dummy,
            &module
        );
    }
    char lpFilename[MAX_PATH];
    GetModuleFileNameA(module, lpFilename, sizeof(lpFilename));
    std::string moduleName = strrchr(lpFilename, '\\');
    moduleName             = moduleName.substr(1, moduleName.length());
    if (thisModule) {
        moduleName.erase(moduleName.find(".dll"), moduleName.length());
    }
    return moduleName;
}

GMLIB_API DWORD_PTR GetProcessBaseAddress(DWORD processId);

GMLIB_API void ToggleMemoryProtection(bool protectionEnabled, uintptr_t address, size_t size);

GMLIB_API void MemoryCopy(uintptr_t destination, uintptr_t source, size_t numBytes);

GMLIB_API void MemorySet(uintptr_t address, unsigned char byte, size_t numBytes);

GMLIB_API std::vector<std::string> split(std::string str, std::string const& pattern);

GMLIB_API uintptr_t FindSignature(const char* szSignature);

GMLIB_API uintptr_t ScanSignature(std::vector<uint16_t> const& pattern);

GMLIB_API uintptr_t ScanSignature(std::vector<uint16_t> const& pattern, uintptr_t regionStart, int32_t size);

GMLIB_API bool
ReplaceBytes(uintptr_t address, std::vector<uint16_t> const& originalBytes, std::vector<uint8_t> const& newBytes);

GMLIB_API uintptr_t RelativeToAbsoluteAddress(uintptr_t relativeAddressLocation);

GMLIB_API void Hook(uintptr_t address, uintptr_t destination, size_t extraClearance = 0);

GMLIB_API std::optional<std::string> readResourceFile(HMODULE hModule, LPWSTR resource, LPCWSTR type);

inline std::optional<std::string> readResourceFile(LPWSTR resource, bool isBinary) {
    HMODULE hModule = GetModuleHandle(NULL);
    return isBinary ? readResourceFile(hModule, resource, L"BINFILE")
                    : readResourceFile(hModule, resource, L"TEXTFILE");
}

inline std::optional<std::string> readResourceFile(uint64_t resourceId, bool isBinary) {
    HMODULE hModule  = GetModuleHandle(NULL);
    auto    resource = MAKEINTRESOURCE(resourceId);
    return readResourceFile(resource, isBinary);
}

} // namespace gmlib::memory
