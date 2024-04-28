#include "Global.h"
#include <GMLIB/Files/FileUtils.h>
#include <GMLIB/Files/Language/I18n/LangI18n.h>

namespace GMLIB::Files::I18n {

LangI18n::LangI18n(std::string const& languageDirectory, std::string const& languageCode)
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

bool LangI18n::loadOrCreateLanguage(std::string const& languageCode, LangLanguage* language) {
    auto result                 = language->init();
    mAllLanguages[languageCode] = language;
    return result;
}

bool LangI18n::updateOrCreateLanguage(std::string const& languageCode, std::string const& language) {
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

bool LangI18n::chooseLanguage(std::string const& languageCode) {
    if (mAllLanguages.count(languageCode)) {
        mLocalization = mAllLanguages[languageCode];
        return true;
    }
    return false;
}

void LangI18n::setDefaultLanguage(std::string const& languageCode) { mDefaultLanguage = languageCode; }

std::string
LangI18n::translate(std::string const& key, std::vector<std::string> const& data, std::string const& translateKey) {
    if (!mLocalization) {
        chooseLanguage(mDefaultLanguage);
    }
    if (mLocalization) {
        if (mLocalization->has_value(key)) {
            return mLocalization->translate(key, data, translateKey);
        }
        if (mAllLanguages.count(mDefaultLanguage)) {
            auto temp = mAllLanguages[mDefaultLanguage];
            if (temp) {
                return temp->translate(key, data, translateKey);
            }
        }
    }
    return key;
}

std::string LangI18n::translate(
    std::string const&              key,
    std::string const&              localLanguage,
    std::vector<std::string> const& data,
    std::string const&              translateKey
) {
    if (mAllLanguages.count(localLanguage)) {
        auto temp = mAllLanguages[localLanguage];
        if (temp) {
            return temp->translate(key, data, translateKey);
        }
    } else if (mAllLanguages.count(mDefaultLanguage)) {
        auto temp = mAllLanguages[mDefaultLanguage];
        if (temp) {
            return temp->translate(key, data, translateKey);
        }
    }
    return key;
}

std::string
LangI18n::get(std::string const& key, std::vector<std::string> const& data, std::string const& translateKey) {
    return translate(key, data, translateKey);
}

std::string LangI18n::get(
    std::string const&              key,
    std::string const&              localLanguage,
    std::vector<std::string> const& data,
    std::string const&              translateKey
) {
    return translate(key, localLanguage, data, translateKey);
}


} // namespace GMLIB::Files::I18n