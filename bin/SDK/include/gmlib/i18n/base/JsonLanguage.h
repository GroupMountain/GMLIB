#pragma once
#include "gmlib/i18n/base/McLang.h"
#include "nlohmann/json.hpp"

namespace gmlib::i18n {

class JsonLanguage : public McLang {
public:
    GMLIB_NDAPI JsonLanguage(std::filesystem::path const& filePath, std::string const& defaultJson);

    GMLIB_NDAPI JsonLanguage(std::filesystem::path const& filePath, nlohmann::json const& defaultJson);

    GMLIB_NDAPI JsonLanguage(
        std::filesystem::path const&                        filePath,
        std::unordered_map<std::string, std::string> const& defaultLanguage
    );

    GMLIB_NDAPI JsonLanguage(std::filesystem::path const& filePath, McLang const& defaultLanguage);

    JsonLanguage() = delete;

public:
    GMLIB_API virtual ~JsonLanguage();

public:
    GMLIB_API bool init();

    GMLIB_API bool reload();

    GMLIB_API bool save_file();
};

} // namespace gmlib::i18n