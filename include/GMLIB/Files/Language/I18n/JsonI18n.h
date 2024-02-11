#pragma once
#include "GMLIB/Files/Language/JsonLanguage.h"
#include "GMLIB/GMLIB.h"

namespace GMLIB::Files::I18n {

class JsonI18n {
private:
    std::string                                    mLanguageDirectory;
    std::unordered_map<std::string, JsonLanguage*> mAllLanguages;
    std::string                                    mLanguageCode;
    JsonLanguage*                                  mLocalization = nullptr;

public:
    GMLIB_API JsonI18n(std::string languageDirectory, std::string languageCode = "en_US");

    JsonI18n() = delete;

public:
    GMLIB_API virtual ~JsonI18n();

public:
    GMLIB_API bool loadLanguage(std::string languageCode, nlohmann::json& language);

    GMLIB_API bool loadLanguage(std::string languageCode, std::string& language);

    GMLIB_API bool loadLanguage(std::string languageCode, JsonLanguage* language);

    GMLIB_API bool chooseLanguage(std::string languageCode);

    GMLIB_API std::string
              translate(std::string key, std::vector<std::string> data = {}, std::string translateKey = "%0$s");
};

} // namespace GMLIB::Files::I18n