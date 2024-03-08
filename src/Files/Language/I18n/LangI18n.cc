#include "Global.h"
#include <GMLIB/Files/FileUtils.h>
#include <GMLIB/Files/Language/I18n/LangI18n.h>

namespace GMLIB::Files::I18n {

LangI18n::LangI18n(std::string languageDirectory, std::string languageCode)
: mLanguageDirectory(languageDirectory),
  mLanguageCode(languageCode) {
    if (!mLanguageDirectory.ends_with("/") && !mLanguageDirectory.ends_with("\\")) {
        mLanguageDirectory = mLanguageDirectory + "/";
    }
}

LangI18n::~LangI18n() {
    for (auto lang : mAllLanguages) {
        delete lang.second;
    }
    mAllLanguages.clear();
}

bool LangI18n::loadOrCreateLanguage(std::string languageCode, LangLanguage* language) {
    auto result                 = language->init();
    mAllLanguages[languageCode] = language;
    return result;
}

bool LangI18n::updateOrCreateLanguage(std::string languageCode, std::string& language) {
    auto path = mLanguageDirectory;
    path      = path + languageCode + ".lang";
    auto lang = new LangLanguage(path, language);
    return loadOrCreateLanguage(languageCode, lang);
}

bool LangI18n::loadAllLanguages() {
    bool result     = true;
    auto parentPath = mLanguageDirectory;
    auto files      = GMLIB::Files::FileUtils::getAllFileFullNameInDirectory(mLanguageDirectory);
    for (auto& file : files) {
        if (file.ends_with(".lang")) {
            auto code = file;
            ll::string_utils::replaceAll(code, ".lang", "");
            if (!mAllLanguages.count(code)) {
                auto        path = parentPath + file;
                std::string emptyLang;
                auto        language = new GMLIB::Files::LangLanguage(path, emptyLang);
                auto        temp     = language->init();
                result               = result && temp;
                mAllLanguages[code]  = language;
            }
        }
    }
    return result;
}

void LangI18n::reloadAllLanguages() {
    for (auto& lang : mAllLanguages) {
        lang.second->reload();
    }
}

bool LangI18n::chooseLanguage(std::string languageCode) {
    if (mAllLanguages.count(languageCode)) {
        mLocalization = mAllLanguages[languageCode];
        return true;
    }
    return false;
}

std::string LangI18n::translate(std::string key, std::vector<std::string> data, std::string translateKey) {
    if (!mLocalization) {
        chooseLanguage("en_US");
    }
    if (mLocalization) {
        if (mLocalization->has_value(key)) {
            return mLocalization->translate(key, data, translateKey);
        }
        if (mAllLanguages.count("en_US")) {
            auto temp = mAllLanguages["en_US"];
            if (temp) {
                return temp->translate(key, data, translateKey);
            }
        }
    }
    return key;
}

} // namespace GMLIB::Files::I18n