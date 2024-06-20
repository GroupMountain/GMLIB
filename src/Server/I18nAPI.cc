#include "GMLIB/Server/I18nAPI.h"
#include "GMLIB/Files/FileUtils.h"
#include "Global.h"

void I18nAPI::chooseLanguage(std::string const& languageCode) { getI18n().chooseLanguage(languageCode); }

void I18nAPI::chooseLanguage(class Localization const& localization) {
    getI18n().chooseLanguage(localization.getFullLanguageCode());
}

optional_ref<const Localization> I18nAPI::getCurrentLanguage() { return getI18n().getCurrentLanguage().get().get(); }

std::string I18nAPI::getCurrentLanguageCode() { return getI18n().getCurrentLanguage()->getFullLanguageCode(); }

std::vector<std::string> I18nAPI::getSupportedLanguageCodes() { return getI18n().getSupportedLanguageCodes(); }

std::optional<std::string> I18nAPI::tryGet(std::string const& key, std::vector<std::string> const& params) {
    auto        localization = getI18n().getCurrentLanguage();
    std::string result;
    auto        has_value = localization->get(key, result, params);
    return has_value ? result : std::string();
}

std::optional<std::string>
I18nAPI::tryGet(std::string const& key, std::vector<std::string> const& params, std::string const& languageCode) {
    if (auto localization = getI18n().getLocaleFor(languageCode)) {
        std::string result;
        auto        has_value = localization->get(key, result, params);
        if (has_value) return result;
    }
    return {};
}

std::optional<std::string> I18nAPI::tryGet(
    std::string const&                  key,
    std::vector<std::string> const&     params,
    std::shared_ptr<class Localization> localization
) {
    if (localization) {
        std::string result;
        auto        has_value = localization->get(key, result, params);
        if (has_value) return result;
    }
    return {};
}

std::string I18nAPI::get(std::string const& key, std::vector<std::string> const& params) {
    auto localization = getI18n().getLocaleFor(getCurrentLanguageCode());
    return getI18n().get(key, params, localization);
}

std::string
I18nAPI::get(std::string const& key, std::vector<std::string> const& params, std::string const& languageCode) {
    auto localization = getI18n().getLocaleFor(languageCode);
    return getI18n().get(key, params, localization);
}

std::string I18nAPI::get(
    std::string const&                  key,
    std::vector<std::string> const&     params,
    std::shared_ptr<class Localization> localization
) {
    return getI18n().get(key, params, localization);
}

void I18nAPI::loadLanguage(
    std::string const&                                  languageCode,
    std::unordered_map<std::string, std::string> const& language
) {
    getI18n().appendTranslations(languageCode, language);
}

void I18nAPI::loadLanguage(std::string const& languageCode, GMLIB::Files::McLang const& language) {
    auto data = language;
    getI18n().appendTranslations(languageCode, data.getTranslationMap());
}

void I18nAPI::loadLanguage(std::string const& languageCode, nlohmann::json const& language) {
    std::unordered_map<std::string, std::string> languageMap;
    for (nlohmann::json::const_iterator it = language.begin(); it != language.end(); ++it) {
        if (it.value().is_string()) {
            languageMap[it.key()] = it.value().get<std::string>();
        }
    }
    loadLanguage(languageCode, languageMap);
}

void I18nAPI::loadLanguageFromFile(std::string const& languageCode, std::filesystem::path const& path) {
    auto language = GMLIB::Files::McLang::parse_file(path.string());
    getI18n().appendTranslations(languageCode, language.getTranslationMap());
}

void I18nAPI::updateOrCreateLanguageFile(
    std::filesystem::path const& path,
    std::string const&           languageCode,
    std::string const&           language
) {
    auto mclang = GMLIB::Files::McLang::parse(language);
    updateOrCreateLanguageFile(path, languageCode, mclang);
}

void I18nAPI::updateOrCreateLanguageFile(
    std::filesystem::path const& path,
    std::string const&           languageCode,
    GMLIB::Files::McLang const&  language
) {
    if (!std::filesystem::exists(path)) {
        std::filesystem::create_directories(path);
    }
    auto filepath  = path / (languageCode + ".lang");
    auto finalLang = language;
    auto localLang = GMLIB::Files::McLang::parse_file(filepath.string());
    finalLang.merge_patch(localLang);
    finalLang.write_to_file(filepath.string());
    loadLanguage(languageCode, finalLang);
}

void I18nAPI::updateOrCreateLanguageFile(
    std::filesystem::path const&                        path,
    std::string const&                                  languageCode,
    std::unordered_map<std::string, std::string> const& language
) {
    auto mclang = GMLIB::Files::McLang(language);
    updateOrCreateLanguageFile(path, languageCode, mclang);
}

void I18nAPI::updateOrCreateLanguageFile(
    std::filesystem::path const& path,
    std::string const&           languageCode,
    nlohmann::json const&        language
) {
    std::unordered_map<std::string, std::string> languageMap;
    for (nlohmann::json::const_iterator it = language.begin(); it != language.end(); ++it) {
        if (it.value().is_string()) {
            languageMap[it.key()] = it.value().get<std::string>();
        }
    }
    updateOrCreateLanguageFile(path, languageCode, languageMap);
}

void I18nAPI::loadLanguagesFromDirectory(std::filesystem::path const& path) {
    auto files = GMLIB::Files::FileUtils::getAllFileFullNameInDirectory(path.string());
    for (auto& file : files) {
        if (file.ends_with(".lang")) {
            auto code = file;
            ll::string_utils::replaceAll(code, ".lang", "");
            auto filepath = path / file;
            auto mclang   = GMLIB::Files::McLang::parse_file(filepath.string());
            loadLanguage(code, mclang);
        }
    }
}