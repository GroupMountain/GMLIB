#pragma once
#include "GMLIB/Files/Language/LangLanguage.h"

namespace GMLIB::Files::I18n {

class LangI18n {
private:
    std::filesystem::path                                          mLanguageDirectory;
    std::unordered_map<std::string, std::shared_ptr<LangLanguage>> mAllLanguages;
    std::string                                                    mLanguageCode;
    std::shared_ptr<LangLanguage>                                  mLocalization    = nullptr;
    std::string                                                    mDefaultLanguage = "en_US";

public:
    GMLIB_NDAPI LangI18n(std::filesystem::path const& languageDirectory, std::string const& languageCode = "en_US");

    LangI18n() = delete;

public:
    GMLIB_API virtual ~LangI18n();

public:
    GMLIB_API bool updateOrCreateLanguage(std::string const& languageCode, std::string const& language);

    GMLIB_API bool updateOrCreateLanguage(std::string const& languageCode, McLang const& language);

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
    bool loadOrCreateLanguage(std::string const& languageCode, std::shared_ptr<LangLanguage> language);
};


} // namespace GMLIB::Files::I18n