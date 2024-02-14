#include "Global.h"
#include <GMLIB/Files/FileUtils.h>
#include <GMLIB/Files/Language/I18n/JsonI18n.h>

namespace GMLIB::Files::I18n {

JsonI18n::JsonI18n(std::string languageDirectory, std::string languageCode)
: mLanguageDirectory(languageDirectory),
  mLanguageCode(mLanguageCode) {
    if (!mLanguageDirectory.ends_with("/") && !mLanguageDirectory.ends_with("\\")) {
        mLanguageDirectory = mLanguageDirectory + "/";
    }
}

JsonI18n::~JsonI18n() {
    for (auto lang : mAllLanguages) {
        delete lang.second;
    }
    mAllLanguages.clear();
    delete mLocalization;
}

bool JsonI18n::loadOrCreateLanguage(std::string languageCode, JsonLanguage* language) {
    auto result                 = language->init();
    mAllLanguages[languageCode] = language;
    return result;
}

bool JsonI18n::updateOrCreateLanguage(std::string languageCode, nlohmann::json& language) {
    auto path = mLanguageDirectory;
    path      = path + languageCode + ".json";
    auto lang = new JsonLanguage(path, language);
    return loadOrCreateLanguage(languageCode, lang);
}

bool JsonI18n::updateOrCreateLanguage(std::string languageCode, std::string& language) {
    auto path = mLanguageDirectory;
    if (!path.ends_with("/") && !path.ends_with("\\")) {
        path = path + "/";
    }
    path      = path + languageCode + ".json";
    auto lang = new JsonLanguage(path, language);
    return loadOrCreateLanguage(languageCode, lang);
}

bool JsonI18n::loadAllLanguages() {
    auto parentPath = mLanguageDirectory;
    auto files      = GMLIB::Files::FileUtils::getAllFileFullNameInDirectory(mLanguageDirectory);
    for (auto& file : files) {
        if (file.ends_with(".json")) {
            auto code = file;
            ll::string_utils::replaceAll(code, ".json", "");
            if (!mAllLanguages.count(code)) {
                auto path      = parentPath + file;
                auto emptyJson = nlohmann::json::object();
                auto language  = new GMLIB::Files::JsonLanguage(path, emptyJson);
                language->init();
                mAllLanguages[code] = language;
            }
        }
    }
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
        chooseLanguage("en_US");
    }
    if (mLocalization) {
        if (mLocalization->hasValue(key)) {
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