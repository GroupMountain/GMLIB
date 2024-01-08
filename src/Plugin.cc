#include "Global.h"
#include "include_all.h"

ll::Logger logger("GMLIB");

namespace plugin {

Plugin::Plugin(ll::plugin::NativePlugin& self) : mSelf(self) {
    // Code for loading the plugin goes here.
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
    logger.info("GMLIB Loaded!");
    logger.info("Author: GroupMountain");
    logger.info("Repository: https://github.com/GroupMountain/GMLIB");
}

bool Plugin::enable() {
    // Codes
    initExperiments(&ll::service::bedrock::getLevel()->getLevelData());
    return true;
}

bool Plugin::disable() {
    logger.info("Disabling GMLIB...");
    // Code for disabling the plugin goes here.
    logger.info("GMLIB Disabled!");
    return true;
}

} // namespace plugin