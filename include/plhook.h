#pragma once

#ifdef PRELOADER_EXPORT
#define PLAPI [[maybe_unused]] __declspec(dllexport)
#else
#define PLAPI [[maybe_unused]] __declspec(dllimport)
#endif

#define PLCAPI extern "C" PLAPI

namespace pl::hook {

    typedef void* FuncPtr;

    enum Priority : int {
        PriorityHighest = 0,
        PriorityHigh = 100,
        PriorityNormal = 200,
        PriorityLow = 300,
        PriorityLowest = 400,
    };

    PLCAPI int pl_hook(FuncPtr target, FuncPtr detour, FuncPtr* originalFunc, Priority priority);

    PLCAPI bool pl_unhook(FuncPtr target, FuncPtr detour);

} // namespace pl::hook