#pragma once

#ifdef GMLIB_EXPORTS
#define GMLIB_API [[maybe_unused]] __declspec(dllexport)
#else
#define GMLIB_API [[maybe_unused]] __declspec(dllimport)
#endif

#ifndef GMLIB_NDAPI
#define GMLIB_NDAPI [[nodiscard]] GMLIB_API
#endif