#pragma once

#ifdef GMLIB_EXPORTS
#define GMLIB_API __declspec(dllexport)
#else
#define GMLIB_API __declspec(dllimport)
#endif