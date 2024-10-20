#pragma once
#include "gmlib/i18n/base/McLang.h"
#include "Macros.h"

namespace gmlib::i18n {

class LangLanguage : public McLang {
public:
    GMLIB_NDAPI LangLanguage(std::filesystem::path const& filePath, std::string const& defaultLanguage);

    GMLIB_NDAPI LangLanguage(std::filesystem::path const& filePath, McLang const& defaultLanguage);

    GMLIB_NDAPI LangLanguage(
        std::filesystem::path const&                        filePath,
        std::unordered_map<std::string, std::string> const& defaultLanguage
    );

public:
    GMLIB_API bool init();

    GMLIB_API bool reload();

    GMLIB_API bool save_file();
};

} // namespace gmlib::i18n