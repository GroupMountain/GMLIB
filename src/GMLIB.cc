#include "Version.h"
#include <GMLIB/GMLIB.h>
#include <include_all.h>
#include "Global.h"

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
    logger.info("Author: {}", LIB_AUTHOR);
    logger.info("Repository: {}", LIB_REPOSITORT);
}

void loadLib() {
    printLogo();
    printLibInfo();
}

void enableLib() {
    initExperiments(&ll::service::bedrock::getLevel()->getLevelData());
}

void disableLib() {}

namespace Version {

GMLIB_API SemVersion getLibVersion() {
    return SemVersion(
        LIB_VERSION_MAJOR,
        LIB_VERSION_MINOR,
        LIB_VERSION_REVISION,
        LIB_VERSION_PRERELEASE,
        LIB_VERSION_BUILD_META
    );
}

GMLIB_API bool isReleaseVersion() { return getLibVersion().getPreRelease().empty(); }

GMLIB_API bool isPreReleaseVersion() { return !getLibVersion().getPreRelease().empty(); }

GMLIB_API std::string getLibVersionString() { return getLibVersion().asString(); }

GMLIB_API bool checkLibVersionMatch(SemVersion minVersion) { return minVersion.operator>=(getLibVersion()); }

GMLIB_API bool checkLibVersionMatch(SemVersion minVersion, SemVersion maxVersion) {
    auto currentVer = getLibVersion();
    if (minVersion.operator>=(currentVer) && maxVersion.operator<=(currentVer)) {
        return true;
    }
    return false;
}

} // namespace Version

} // namespace GMLIB