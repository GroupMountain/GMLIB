#pragma once
#include "Macros.h"
#include "mc/deps/core/sem_ver/SemVersion.h"

namespace GMLIB {

class Version : public SemVersion {
public:
    GMLIB_NDAPI Version();

    GMLIB_NDAPI
    Version(
        ushort             major,
        ushort             minor,
        ushort             patch,
        std::string const& preRelease = "",
        std::string const& buildMeta  = ""
    );

    GMLIB_NDAPI Version(class SemVersion const& version);

    GMLIB_NDAPI bool isInRange(Version const& minVersion, Version const& maxVersion);

    GMLIB_NDAPI std::string toString(bool prefix = true);

public:
    GMLIB_NDAPI static bool isValidVersionString(std::string const& version);

    GMLIB_NDAPI static std::optional<Version> fromString(std::string const& version);

    GMLIB_NDAPI static std::optional<Version> fromVector(std::vector<int> const& version);

    GMLIB_NDAPI static Version getLibVersion();

    GMLIB_NDAPI static bool isReleaseVersion();

    GMLIB_NDAPI static bool isPreReleaseVersion();

    GMLIB_NDAPI static std::string getLibVersionString();

    GMLIB_NDAPI static std::string getPreReleaseInfo();

    GMLIB_NDAPI static bool checkLibVersionMatch(Version const& minVersion);

    GMLIB_NDAPI static bool checkLibVersionMatch(Version const& minVersion, Version const& maxVersion);

    GMLIB_NDAPI static int getProtocolVersion();

    GMLIB_NDAPI static Version getGameVersion();

    GMLIB_NDAPI static std::string getGameVersionString();

    GMLIB_NDAPI static Version getLeviLaminaVersion();

    GMLIB_NDAPI static std::string getLeviLaminaVersionString();
};

} // namespace GMLIB
