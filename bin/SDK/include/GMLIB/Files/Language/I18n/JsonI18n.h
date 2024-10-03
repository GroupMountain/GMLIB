#pragma once
#include "GMLIB/Files/Language/JsonLanguage.h"
#include "GMLIB/Macros.h"

namespace GMLIB::Files::I18n {

class JsonI18n {
private:
    std::filesystem::path                                          mLanguageDirectory;
    std::unordered_map<std::string, std::shared_ptr<JsonLanguage>> mAllLanguages;
    std::string                                                    mLanguageCode;
    std::shared_ptr<JsonLanguage>                                  mLocalization    = nullptr;
    std::string                                                    mDefaultLanguage = "en_US";

public:
    GMLIB_NDAPI JsonI18n(std::filesystem::path const& languageDirectory, std::string const& languageCode = "en_US");

    JsonI18n() = delete;

public:
    GMLIB_API virtual ~JsonI18n();

public:
    GMLIB_API bool updateOrCreateLanguage(std::string const& languageCode, nlohmann::json const& language);

    GMLIB_API bool updateOrCreateLanguage(std::string const& languageCode, std::string const& language);

    GMLIB_API bool loadAllLanguages();

    GMLIB_API void reloadAllLanguages();

    GMLIB_API bool chooseLanguage(std::string const& languageCode = "en_US");

    GMLIB_API void setDefaultLanguage(std::string const& languageCode = "en_US");

    GMLIB_NDAPI std::string
    translate(std::string const& key, std::vector<std::string> const& params = {}, std::string const& data = "%0$s");

    GMLIB_NDAPI std::string translate(
        std::string const&              key,
        std::string const&              localLanguage,
        std::vector<std::string> const& params = {},
        std::string const&              data   = "%0$s"
    );

    GMLIB_NDAPI std::string
    get(std::string const& key, std::vector<std::string> const& params = {}, std::string const& data = "%0$s");

    GMLIB_NDAPI std::string
                get(std::string const&              key,
                    std::string const&              localLanguage,
                    std::vector<std::string> const& params = {},
                    std::string const&              data   = "%0$s");

private:
    bool loadOrCreateLanguage(std::string const& languageCode, std::shared_ptr<JsonLanguage> language);
};

} // namespace GMLIB::Files::I18n