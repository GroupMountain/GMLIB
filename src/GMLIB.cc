#include "Global.h"
#include "Version.h"
#include <GMLIB/GMLIB.h>
#include <include_all.h>

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
    GMLIB::Server::UserCache::initUserCache();
}

void enableLib() { 
    initExperiments(&ll::service::bedrock::getLevel()->getLevelData());
    CaculateTPS(); 
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

GMLIB_API std::string getLibVersionString() { return getLibVersion().asString(); }

GMLIB_API bool isReleaseVersion() { return getLibVersion().getPreRelease().empty(); }

GMLIB_API bool isPreReleaseVersion() { return !getLibVersion().getPreRelease().empty(); }

GMLIB_API std::string getPreReleaseInfo() { return getLibVersion().getPreRelease(); }

GMLIB_API bool checkLibVersionMatch(SemVersion minVersion) {
    auto currentVer = getLibVersion();
    if (currentVer.mMajor >= minVersion.mMajor) {
        if (currentVer.mMajor == minVersion.mMajor) {
            if (currentVer.mMinor >= minVersion.mMinor) {
                if (currentVer.mMinor == minVersion.mMinor) {
                    if (currentVer.mPatch >= minVersion.mPatch) {
                        return true;
                    }
                    return false;
                }
                return true;
            }
            return false;
        }
        return true;
    }
    return false;
}

GMLIB_API bool checkLibVersionMatch(SemVersion minVersion, SemVersion maxVersion) {
    auto currentVer = getLibVersion();
    if (checkLibVersionMatch(minVersion)) {
        if (currentVer.mMajor <= minVersion.mMajor) {
            if (currentVer.mMajor == minVersion.mMajor) {
                if (currentVer.mMinor <= minVersion.mMinor) {
                    if (currentVer.mMinor == minVersion.mMinor) {
                        if (currentVer.mPatch <= minVersion.mPatch) {
                            return true;
                        }
                        return false;
                    }
                    return true;
                }
                return false;
            }
            return true;
        }
        return false;
    }
    return false;
}

} // namespace Version

} // namespace GMLIB