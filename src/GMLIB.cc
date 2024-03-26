#include "Entry.h"
#include "Global.h"
#include "Version.h"
#include <GMLIB/GMLIB.h>
#include <ll/api/service/ServerInfo.h>
#include <mc/common/Common.h>
#include <mc/common/SharedConstants.h>
#include <mc/deps/core/sem_ver/SemVersion.h>
#include <regex>

ll::Logger logger(LIB_NAME);

namespace GMLIB {

#define LOGO(x) std::cout << fmt::format(fg(fmt::color::pink), x) << std::endl;

void printLogo() {
    LOGO(R"(                                                                         )")
    LOGO(R"(         ________  _____ ______   ___       ___  ________                )")
    LOGO(R"(        |\   ____\|\   _ \  _   \|\  \     |\  \|\   __  \               )")
    LOGO(R"(        \ \  \___|\ \  \\\__\ \  \ \  \    \ \  \ \  \|\ /_              )")
    LOGO(R"(         \ \  \  __\ \  \\|__| \  \ \  \    \ \  \ \   __  \             )")
    LOGO(R"(          \ \  \|\  \ \  \    \ \  \ \  \____\ \  \ \  \|\  \            )")
    LOGO(R"(           \ \_______\ \__\    \ \__\ \_______\ \__\ \_______\           )")
    LOGO(R"(            \|_______|\|__|     \|__|\|_______|\|__|\|_______|           )")
    LOGO(R"(                                                                         )")
    LOGO(R"(           ------------  Group Mountain Library  ------------            )")
    LOGO(R"(                                                                         )")
}

void printLibInfo() {
    logger.info(
        "Loaded Version: {} with {}",
        fmt::format(fg(fmt::color::light_sky_blue), "LeviLamina-" + Version::getLeviLaminaVersionString()),
        fmt::format(fg(fmt::color::pink), "GMLIB-" + Version::getLibVersionString())
    );
    logger.info("GMLIB is a free library for LeviLamina licensed under LGPLv3");
    logger.info("Author: {}", LIB_AUTHOR);
    logger.info("Repository: {}", LIB_REPOSITORT);
}

void loadLib() {
    if (Version::getProtocolVersion() != TARGET_PROTOCOL_VERSION) {
        logger.warn("GMLIB is running on an unsupported BDS protocol version! ");
        logger.warn(
            "Target protocol version {}, current protocol version {}.",
            TARGET_PROTOCOL_VERSION,
            Version::getProtocolVersion()
        );
        logger.warn("This may result in crash! Please switch to the version matching the BDS version!");
    }
    printLogo();
    printLibInfo();
    updatePlaceholder();
    regServerPAPI();
    regPlayerPAPI();
}

void enableLib() {
    initExperiments(&ll::service::bedrock::getLevel()->getLevelData());
    CaculateTPS();
}

void disableLib() {}

Version::Version() : SemVersion() {}

Version::Version(ushort major, ushort minor, ushort patch, std::string const& preRelease, std::string const& buildMeta)
: SemVersion(major, minor, patch, preRelease, buildMeta) {}

Version::Version(class SemVersion const& sem_version) : SemVersion(sem_version) {}

bool Version::matchesLowestVersion(Version lowestVersion) {
    if (satisfies(lowestVersion)) {
        return getPatch() >= lowestVersion.getPatch();
    }
    return false;
}

bool Version::isInRange(Version lowestVersion, Version highestVersion) {
    return matchesHighestVersion(highestVersion) && matchesLowestVersion(lowestVersion);
}

bool Version::matchesHighestVersion(Version highestVersion) { return highestVersion.matchesLowestVersion(*this); }

bool Version::isValidVersionString(std::string version) {
    std::regex pattern("(v)?(\\d+)\\.(\\d+)\\.(\\d+)");
    return std::regex_match(version, pattern);
}

std::optional<Version> Version::fromString(std::string version) {
    if (isValidVersionString(version)) {
        std::istringstream iss(version);
        char               prefix;
        int                major, minor, patch;
        if (iss.peek() == 'v') {
            iss >> prefix >> major;
        } else {
            iss >> major;
        }
        iss.ignore(1);
        iss >> minor;
        iss.ignore(1);
        iss >> patch;
        return Version(major, minor, patch);
    }
    return {};
}

Version Version::getLibVersion() {
    return Version(
        LIB_VERSION_MAJOR,
        LIB_VERSION_MINOR,
        LIB_VERSION_REVISION,
        LIB_VERSION_PRERELEASE,
        LIB_VERSION_BUILD_META
    );
}

std::string Version::getLibVersionString() { return getLibVersion().asString(); }

bool Version::isReleaseVersion() { return getLibVersion().getPreRelease().empty(); }

bool Version::isPreReleaseVersion() { return !getLibVersion().getPreRelease().empty(); }

std::string Version::getPreReleaseInfo() { return getLibVersion().getPreRelease(); }

bool Version::checkLibVersionMatch(Version minVersion) {
    auto currentVer = getLibVersion();
    return currentVer.satisfies(minVersion);
}

int Version::getProtocolVersion() { return SharedConstants::NetworkProtocolVersion; }

Version Version::getBdsVersion() {
    auto version = ll::getBdsVersion();
    return Version(version.major, version.minor, version.patch);
}

std::string Version::getBdsVersionString() { return Common::getGameVersionString(); }

Version Version::getLeviLaminaVersion() {
    auto version = ll::getLoaderVersion();
    return Version(version.major, version.minor, version.patch);
}

std::string Version::getLeviLaminaVersionString() { return getLeviLaminaVersion().asString(); }

} // namespace GMLIB