#include "Entry.h"
#include "Global.h"
#include "Version.h"
#include <GMLIB/GMLIB.h>
#include <mc/common/Common.h>
#include <mc/common/SharedConstants.h>
#include <mc/deps/core/sem_ver/SemVersion.h>

ll::Logger logger(LIB_NAME);

namespace GMLIB {

void printLogo() {
    std::cout << R"(                                                                        )" << std::endl;
    std::cout << R"(         ________  _____ ______   ___       ___  ________               )" << std::endl;
    std::cout << R"(        |\   ____\|\   _ \  _   \|\  \     |\  \|\   __  \              )" << std::endl;
    std::cout << R"(        \ \  \___|\ \  \\\__\ \  \ \  \    \ \  \ \  \|\ /_             )" << std::endl;
    std::cout << R"(         \ \  \  __\ \  \\|__| \  \ \  \    \ \  \ \   __  \            )" << std::endl;
    std::cout << R"(          \ \  \|\  \ \  \    \ \  \ \  \____\ \  \ \  \|\  \           )" << std::endl;
    std::cout << R"(           \ \_______\ \__\    \ \__\ \_______\ \__\ \_______\          )" << std::endl;
    std::cout << R"(            \|_______|\|__|     \|__|\|_______|\|__|\|_______|          )" << std::endl;
    std::cout << R"(                                                                        )" << std::endl;
    std::cout << R"(           ------------  Group Mountain Library  ------------           )" << std::endl;
    std::cout << R"(                                                                        )" << std::endl;
}

void printLibInfo() {
    logger.info("GMLIB Loaded!");
    logger.info("Version: {}", Version::getLibVersionString());
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

std::string getBdsVersion() { return Common::getGameVersionString(); }

} // namespace Version

} // namespace GMLIB