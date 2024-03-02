#pragma once
#include "mc/deps/core/sem_ver/SemVersion.h"

#ifdef GMLIB_EXPORTS
#define GMLIB_API __declspec(dllexport)
#else
#define GMLIB_API __declspec(dllimport)
#endif

namespace GMLIB::Version {

GMLIB_API SemVersion getLibVersion();

GMLIB_API bool isReleaseVersion();

GMLIB_API bool isPreReleaseVersion();

GMLIB_API std::string getLibVersionString();

GMLIB_API std::string getPreReleaseInfo();

GMLIB_API bool checkLibVersionMatch(SemVersion minVersion);

GMLIB_API int getProtocolVersion();

GMLIB_API SemVersion getBdsVersion();

GMLIB_API std::string getBdsVersionString();

GMLIB_API SemVersion getLeviLaminaVersion();

GMLIB_API std::string getLeviLaminaVersionString();

} // namespace GMLIB::Version
