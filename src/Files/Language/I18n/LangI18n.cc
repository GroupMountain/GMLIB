#include "Global.h"
#include <GMLIB/Files/Language/I18n/LangI18n.h>

namespace GMLIB::Files::I18n {

LangI18n::LangI18n(std::string languageDirectory, std::string languageCode)
: mLanguageDirectory(languageDirectory),
  mLanguageCode(mLanguageCode) {}

LangI18n::~LangI18n() {
    mAllLanguages.clear();
    delete mLocalization;
}

bool LangI18n::loadLanguage(std::string languageCode, LangLanguage* language) {
    auto result                 = language->init();
    mAllLanguages[languageCode] = language;
    return result;
}

bool LangI18n::loadLanguage(std::string languageCode, std::string& language) {
    auto path = mLanguageDirectory;
    if (!path.ends_with("/") && !path.ends_with("\\")) {
        path = path + "/";
    }
    path      = path + languageCode + ".lang";
    auto lang = new LangLanguage(path, language);
    loadLanguage(languageCode, lang);
}

bool LangI18n::chooseLanguage(std::string languageCode) {
    if (mAllLanguages.count(languageCode)) {
        mLocalization = mAllLanguages[languageCode];
        return true;
    }
    return false;
}

std::string LangI18n::translate(std::string key, std::vector<std::string> data, std::string translateKey) {
    if (mLocalization) {
        return mLocalization->translate(key, data, translateKey);
    }
    return key;
}

} // namespace GMLIB::Files::I18n