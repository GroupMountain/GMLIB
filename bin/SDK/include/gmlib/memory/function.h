#pragma once
#include "Macros.h"
#include <Windows.h>

namespace gmlib::memory {

template <typename RetType, typename... Args>
using FunctionPtr = RetType (*)(Args...);

GMLIB_NDAPI FARPROC GetFunctionAddress(LPCSTR funcName);

GMLIB_NDAPI FARPROC GetFunctionAddress(LPCWSTR dllName, LPCSTR funcName);

template <typename RetType, typename... Args>
inline FunctionPtr<RetType, Args...> GetFunctionPtr(LPCSTR funcName) {
    if (auto funcPtr = GetFunctionAddress(funcName)) {
        return reinterpret_cast<FunctionPtr<RetType, Args...>>(funcPtr);
    }
    return nullptr;
}

template <typename RetType, typename... Args>
inline FunctionPtr<RetType, Args...> GetFunctionPtr(LPCWSTR dllName, LPCSTR funcName) {
    if (auto funcPtr = GetFunctionAddress(dllName, funcName)) {
        return reinterpret_cast<FunctionPtr<RetType, Args...>>(funcPtr);
    }
    return nullptr;
}

template <typename RetType, typename... Args>
inline std::optional<RetType> CallFunction(LPCSTR funcName, Args... args) {
    if (auto func = GetFunctionPtr<RetType, Args...>(funcName); func) {
        return func(std::forward<Args>(args)...);
    }
    return {};
}

template <typename RetType, typename... Args>
inline std::optional<RetType> CallFunction(LPCWSTR dllName, LPCSTR funcName, Args... args) {
    if (auto func = GetFunctionPtr<RetType, Args...>(dllName, funcName); func) {
        return func(std::forward<Args>(args)...);
    }
    return {};
}

} // namespace gmlib::memory