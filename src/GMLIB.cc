#include "Entry.h"
#include "Global.h"
#include "Version.h"
#include <GMLIB/GMLIB.h>
#include <ll/api/Versions.h>
#include <ll/api/service/ServerInfo.h>
#include <ll/api/utils/SystemUtils.h>
#include <mc/common/Common.h>
#include <mc/common/SharedConstants.h>
#include <mc/deps/core/sem_ver/SemVersion.h>
#include <regex>

ll::Logger logger(ll::sys_utils::isStdoutSupportAnsi() ? fmt::format(fg(fmt::color::pink), LIB_NAME) : LIB_NAME);

namespace GMLIB {

#define LOGO(x)                                                                                                        \
    std::cout << (ll::sys_utils::isStdoutSupportAnsi() ? fmt::format(fg(fmt::color::pink), x) : fmt::format(x))        \
              << std::endl;

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
        ll::sys_utils::isStdoutSupportAnsi()
            ? fmt::format(fg(fmt::color::light_sky_blue), "LeviLamina-" + Version::getLeviLaminaVersionString())
            : "LeviLamina-" + Version::getLeviLaminaVersionString(),
        ll::sys_utils::isStdoutSupportAnsi()
            ? fmt::format(fg(fmt::color::pink), "GMLIB-" + Version::getLibVersionString())
            : "GMLIB-" + Version::getLibVersionString()
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
    initUserCache();
}

void disableLib() {}

Version::Version() : SemVersion() {}

Version::Version(ushort major, ushort minor, ushort patch, std::string const& preRelease, std::string const& buildMeta)
: SemVersion(major, minor, patch, preRelease, buildMeta) {}

Version::Version(class SemVersion const& sem_version) : SemVersion(sem_version) {}

bool Version::isInRange(Version const& minVersion, Version const& maxVersion) {
    return (*this >= minVersion) && (*this <= maxVersion);
}

std::string Version::toString(bool prefix) {
    auto result = asString();
    if (prefix) {
        return "v" + result;
    }
    return result;
}

bool Version::isValidVersionString(std::string const& version) {
    std::regex pattern("(v)?(\\d+)\\.(\\d+)\\.(\\d+)");
    return std::regex_match(version, pattern);
}

std::optional<Version> Version::fromString(std::string const& version) {
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

std::optional<Version> Version::fromVector(std::vector<int> const& version) {
    if (version.size() == 3) {
        return Version(version[0], version[1], version[2]);
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

bool Version::checkLibVersionMatch(Version const& minVersion) { return getLibVersion() >= minVersion; }

bool Version::checkLibVersionMatch(Version const& minVersion, Version const& maxVersion) {
    return getLibVersion().isInRange(minVersion, maxVersion);
}

int Version::getProtocolVersion() { return SharedConstants::NetworkProtocolVersion; }

Version Version::getGameVersion() {
    auto version = ll::getGameVersion();
    return Version(version.major, version.minor, version.patch);
}

std::string Version::getGameVersionString() { return Common::getGameVersionString(); }

Version Version::getLeviLaminaVersion() {
    auto version = ll::getLoaderVersion();
    return Version(version.major, version.minor, version.patch);
}

std::string Version::getLeviLaminaVersionString() { return getLeviLaminaVersion().asString(); }

} // namespace GMLIB
