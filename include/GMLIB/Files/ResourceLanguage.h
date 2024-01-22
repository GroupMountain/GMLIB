#pragma once
#include "GMLIB/GMLIB.h"

namespace GMLIB::Files {

class ResourceLanguage {
private:
    std::unordered_map<std::string, std::string> mLanguages;
    std::string                                  mPath;
    SemVersion                                   mVersion;
    std::string                                  mPluginName;

public:
    GMLIB_API
    ResourceLanguage(
        std::string directoryPath,
        std::string pluginName,
        ushort      versionMajor = 0,
        ushort      versionMinor = 0,
        ushort      versionPatch = 1
    );

    ResourceLanguage() = delete;

public:
    GMLIB_API void addLanguage(std::string identifider, std::string& language);

    GMLIB_API void addLanguages(std::vector<std::pair<std::string, std::string>>& languages);

    GMLIB_API void initLanguage();
};

} // namespace GMLIB::Files