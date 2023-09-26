#pragma once
#include <vector>

#ifdef GMLib_EXPORTS
#define GMLib_ModAPI __declspec(dllexport)
#else
#define GMLib_ModAPI __declspec(dllimport)
#endif