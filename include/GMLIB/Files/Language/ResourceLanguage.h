#pragma once
#include "GMLIB/GMLIB.h"
#include "GMLIB/Macros.h"

namespace GMLIB::Files {

class ResourceLanguage {
private:
    std::unordered_map<std::string, std::string> mLanguages;
    std::filesystem::path                        mPath;
    Version                                      mVersion;
    std::string                                  mPluginName;

public:
    GMLIB_NDAPI
    ResourceLanguage(
        std::filesystem::path const& directoryPath,
        std::string const&           pluginName,
        ushort                       versionMajor = 0,
        ushort                       versionMinor = 0,
        ushort                       versionPatch = 1
    );

    ResourceLanguage() = delete;

public:
    GMLIB_API void addLanguage(std::string const& identifider, std::string const& language);

    GMLIB_API void addLanguages(std::vector<std::pair<std::string, std::string>> const& languages);

    GMLIB_API void initLanguage();
};

} // namespace GMLIB::Files