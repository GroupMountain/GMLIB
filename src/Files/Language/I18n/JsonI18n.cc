#include "Global.h"
#include <GMLIB/Files/FileUtils.h>
#include <GMLIB/Files/Language/I18n/JsonI18n.h>
#include <GMLIB/Server/I18nAPI.h>

namespace GMLIB::Files::I18n {

JsonI18n::JsonI18n(std::filesystem::path const& languageDirectory, std::string const& languageCode)
: mLanguageDirectory(languageDirectory),
  mLanguageCode(mLanguageCode) {}

JsonI18n::~JsonI18n() {
    for (auto& [langCode, langData] : mAllLanguages) {
        langData.reset();
    }
    mAllLanguages.clear();
}

bool JsonI18n::loadOrCreateLanguage(std::string const& languageCode, std::shared_ptr<JsonLanguage> language) {
    auto result                 = language->init();
    mAllLanguages[languageCode] = language;
    return result;
}

bool JsonI18n::updateOrCreateLanguage(std::string const& languageCode, nlohmann::json const& language) {
    auto path = mLanguageDirectory / (languageCode + ".json");
    auto lang = std::make_shared<JsonLanguage>(path, language);
    return loadOrCreateLanguage(languageCode, lang);
}

bool JsonI18n::updateOrCreateLanguage(std::string const& languageCode, std::string const& language) {
    auto path = mLanguageDirectory / (languageCode + ".json");
    auto lang = std::make_shared<JsonLanguage>(path, language);
    return loadOrCreateLanguage(languageCode, lang);
}

bool JsonI18n::loadAllLanguages() {
    bool result     = true;
    auto parentPath = mLanguageDirectory;
    auto files      = GMLIB::Files::FileUtils::getAllFileFullNameInDirectory(mLanguageDirectory);
    for (auto& file : files) {
        if (file.ends_with(".json")) {
            auto code = file;
            ll::string_utils::replaceAll(code, ".json", "");
            if (!mAllLanguages.count(code)) {
                auto path           = parentPath / file;
                auto emptyJson      = nlohmann::json::object();
                auto language       = std::make_shared<JsonLanguage>(path, emptyJson);
                auto temp           = language->init();
                result              = result && temp;
                mAllLanguages[code] = language;
            }
        }
    }
    return result;
}

void JsonI18n::reloadAllLanguages() {
    for (auto& lang : mAllLanguages) {
        lang.second->reload();
    }
}

bool JsonI18n::chooseLanguage(std::string const& languageCode) {
    if (mAllLanguages.count(languageCode)) {
        mLocalization = mAllLanguages[languageCode];
        return true;
    }
    return false;
}

void JsonI18n::setDefaultLanguage(std::string const& languageCode) { mDefaultLanguage = languageCode; }

std::string
JsonI18n::translate(std::string const& key, std::vector<std::string> const& data, std::string const& translateKey) {
    if (!mLocalization) {
        chooseLanguage(mDefaultLanguage);
    }
    if (mLocalization) {
        if (mLocalization->hasValue(key)) {
            return mLocalization->translate(key, data, translateKey);
        }
        if (mAllLanguages.count(mDefaultLanguage)) {
            if (auto temp = mAllLanguages[mDefaultLanguage]) {
                return temp->translate(key, data, translateKey);
            }
        }
    }
    return I18nAPI::get(key, data, mLanguageCode);
}

std::string JsonI18n::translate(
    std::string const&              key,
    std::string const&              localLanguage,
    std::vector<std::string> const& data,
    std::string const&              translateKey
) {
    if (mAllLanguages.count(localLanguage)) {
        if (auto temp = mAllLanguages[localLanguage]) {
            return temp->translate(key, data, translateKey);
        }
    } else if (mAllLanguages.count(mDefaultLanguage)) {
        if (auto temp = mAllLanguages[mDefaultLanguage]) {
            return temp->translate(key, data, translateKey);
        }
    }
    return I18nAPI::get(key, data, localLanguage);
}

std::string
JsonI18n::get(std::string const& key, std::vector<std::string> const& data, std::string const& translateKey) {
    return translate(key, data, translateKey);
}

std::string JsonI18n::get(
    std::string const&              key,
    std::string const&              localLanguage,
    std::vector<std::string> const& data,
    std::string const&              translateKey
) {
    return translate(key, localLanguage, data, translateKey);
}

} // namespace GMLIB::Files::I18n