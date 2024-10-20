#pragma once
#include "Macros.h"
#include "gmlib/GMLIB.h"
#include "gmlib/i18n/base/McLang.h"

namespace gmlib::i18n {

class ResourceI18n {
private:
    std::unordered_map<std::string, McLang> mLanguages;
    std::filesystem::path                   mPath;
    Version                                 mVersion;
    std::string                             mPluginName;

public:
    GMLIB_NDAPI
    ResourceI18n(
        std::filesystem::path const& directoryPath,
        std::string const&           pluginName,
        ushort                       versionMajor = 0,
        ushort                       versionMinor = 0,
        ushort                       versionPatch = 1
    );

    ResourceI18n() = delete;

public:
    GMLIB_API void addLanguage(std::string const& identifier, std::string const& language);

    GMLIB_API void addLanguage(std::string const& identifier, McLang const& language);

    GMLIB_API void addLanguageFromPath(std::string const& identifier, std::filesystem::path const& path);

    GMLIB_API void addLanguages(std::vector<std::pair<std::string, McLang>> const& languages);

    GMLIB_API void addLanguages(std::vector<std::pair<std::string, std::string>> const& languages);

    GMLIB_API void addLanguagesFromDirectory(std::filesystem::path const& path);

    GMLIB_API void loadAllLanguages();
};

} // namespace gmlib::i18n