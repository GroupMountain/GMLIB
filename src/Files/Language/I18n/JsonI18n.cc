#include "Global.h"
#include <GMLIB/Files/Language/I18n/JsonI18n.h>

namespace GMLIB::Files::I18n {

JsonI18n::JsonI18n(std::string languageDirectory, std::string languageCode)
: mLanguageDirectory(languageDirectory),
  mLanguageCode(mLanguageCode) {}

JsonI18n::~JsonI18n() {
    mAllLanguages.clear();
    delete mLocalization;
}

bool JsonI18n::loadLanguage(std::string languageCode, JsonLanguage* language) {
    auto result                 = language->init();
    mAllLanguages[languageCode] = language;
    return result;
}

bool JsonI18n::loadLanguage(std::string languageCode, nlohmann::json& language) {
    auto path = mLanguageDirectory;
    if (!path.ends_with("/") && !path.ends_with("\\")) {
        path = path + "/";
    }
    path      = path + languageCode + ".json";
    auto lang = new JsonLanguage(path, language);
    loadLanguage(languageCode, lang);
}

bool JsonI18n::loadLanguage(std::string languageCode, std::string& language) {
    auto path = mLanguageDirectory;
    if (!path.ends_with("/") && !path.ends_with("\\")) {
        path = path + "/";
    }
    path      = path + languageCode + ".json";
    auto lang = new JsonLanguage(path, language);
    loadLanguage(languageCode, lang);
}

bool JsonI18n::chooseLanguage(std::string languageCode) {
    if (mAllLanguages.count(languageCode)) {
        mLocalization = mAllLanguages[languageCode];
        return true;
    }
    return false;
}

std::string JsonI18n::translate(std::string key, std::vector<std::string> data, std::string translateKey) {
    if (!mLocalization) {
        if (mAllLanguages.count("en_US")) {
            mLocalization = mAllLanguages["en_US"];
        }
    }
    if (mLocalization) {
        if (mLocalization->hasValue(key)) {
            return mLocalization->translate(key, data, translateKey);
        }
        if (mAllLanguages.count("en_US")) {
            auto temp = mAllLanguages["en_US"];
            if (temp) {
                temp->translate(key, data, translateKey);
            }
        }
    }
    return key;
}

} // namespace GMLIB::Files::I18n