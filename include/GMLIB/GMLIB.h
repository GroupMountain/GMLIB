#pragma once
#include "Macros.h"
#include "mc/deps/core/sem_ver/SemVersion.h"

namespace GMLIB {

class Version : public SemVersion {
public:
    GMLIB_API Version();

    GMLIB_API
    Version(
        ushort             major,
        ushort             minor,
        ushort             patch,
        std::string const& preRelease = "",
        std::string const& buildMeta  = ""
    );

    GMLIB_API Version(class SemVersion const& version);

    GMLIB_API bool isInRange(Version minVersion, Version maxVersion);

public:
    GMLIB_API static bool isValidVersionString(std::string version);

    GMLIB_API static std::optional<Version> fromString(std::string version);

    GMLIB_API static std::optional<Version> fromVector(std::vector<int> version);

    GMLIB_API static Version getLibVersion();

    GMLIB_API static bool isReleaseVersion();

    GMLIB_API static bool isPreReleaseVersion();

    GMLIB_API static std::string getLibVersionString();

    GMLIB_API static std::string getPreReleaseInfo();

    GMLIB_API static bool checkLibVersionMatch(Version minVersion);

    GMLIB_API static bool checkLibVersionMatch(Version minVersion, Version maxVersion);

    GMLIB_API static int getProtocolVersion();

    GMLIB_API static Version getBdsVersion();

    GMLIB_API static std::string getBdsVersionString();

    GMLIB_API static Version getLeviLaminaVersion();

    GMLIB_API static std::string getLeviLaminaVersionString();
};

} // namespace GMLIB
