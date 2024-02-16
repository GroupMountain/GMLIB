#pragma once
#include "GMLIB/Files/Language/LangLanguage.h"

namespace GMLIB::Files::I18n {

class LangI18n {
private:
    std::string                                    mLanguageDirectory;
    std::unordered_map<std::string, LangLanguage*> mAllLanguages;
    std::string                                    mLanguageCode;
    LangLanguage*                                  mLocalization = nullptr;

public:
    GMLIB_API LangI18n(std::string languageDirectory, std::string languageCode = "en_US");

    LangI18n() = delete;

public:
    GMLIB_API virtual ~LangI18n();

public:
    GMLIB_API bool updateOrCreateLanguage(std::string languageCode, std::string& language);

    GMLIB_API bool updateOrCreateLanguage(std::string languageCode, McLang language);

    GMLIB_API bool loadAllLanguages();

    GMLIB_API void reloadAllLanguages();

    GMLIB_API bool chooseLanguage(std::string languageCode);

    GMLIB_API std::string
              translate(std::string key, std::vector<std::string> data = {}, std::string translateKey = "%0$s");

private:
    bool loadOrCreateLanguage(std::string languageCode, LangLanguage* language);
};


} // namespace GMLIB::Files::I18n