#include "Entry.h"
#include "Global.h"
#include "Version.h"
#include <GMLIB/GMLIB.h>
#include <ll/api/service/ServerInfo.h>
#include <mc/common/Common.h>
#include <mc/common/SharedConstants.h>
#include <mc/deps/core/sem_ver/SemVersion.h>

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
    GMLIB::Server::UserCache::initUserCache();
    updatePlaceholder();
    regServerPAPI();
    regPlayerPAPI();
}

void enableLib() {
    initExperiments(&ll::service::bedrock::getLevel()->getLevelData());
    CaculateTPS();
}

void disableLib() {}

namespace Version {

SemVersion getLibVersion() {
    return SemVersion(
        LIB_VERSION_MAJOR,
        LIB_VERSION_MINOR,
        LIB_VERSION_REVISION,
        LIB_VERSION_PRERELEASE,
        LIB_VERSION_BUILD_META
    );
}

std::string getLibVersionString() { return getLibVersion().asString(); }

bool isReleaseVersion() { return getLibVersion().getPreRelease().empty(); }

bool isPreReleaseVersion() { return !getLibVersion().getPreRelease().empty(); }

std::string getPreReleaseInfo() { return getLibVersion().getPreRelease(); }

bool checkLibVersionMatch(SemVersion minVersion) {
    auto currentVer = getLibVersion();
    return currentVer.satisfies(minVersion);
}

int getProtocolVersion() { return SharedConstants::NetworkProtocolVersion; }

SemVersion getBdsVersion() {
    auto version = ll::getBdsVersion();
    return SemVersion(version.major, version.minor, version.patch, "", "");
}

std::string getBdsVersionString() { return Common::getGameVersionString(); }

SemVersion getLeviLaminaVersion() {
    auto version = ll::getLoaderVersion();
    return SemVersion(version.major, version.minor, version.patch, "", "");
}

std::string getLeviLaminaVersionString() { return getLeviLaminaVersion().asString(); }

} // namespace Version

} // namespace GMLIB