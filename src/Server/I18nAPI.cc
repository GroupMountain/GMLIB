#include "GMLIB/Server/I18nAPI.h"

void I18nAPI::chooseLanguage(std::string const& languageCode) { getI18n().chooseLanguage(languageCode); }

void I18nAPI::chooseLanguage(class Localization const& localization) { getI18n().chooseLanguage(localization); }

optional_ref<const Localization> I18nAPI::getCurrentLanguage() { return getI18n().getCurrentLanguage(); }

std::optional<std::string> I18nAPI::getCurrentLanguageCode() {
    return getI18n().getCurrentLanguage()->getFullLanguageCode();
}

std::optional<std::string> I18nAPI::tryGet(std::string const& key, std::vector<std::string> const& params) {
    if (auto localization = getI18n().getCurrentLanguage()) {
        std::string result;
        auto        has_value = localization->get(key, result, params);
        if (has_value) return result;
    }
    return {};
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
    if (auto localization = getI18n().getCurrentLanguage()) {
        std::string result;
        auto        has_value = localization->get(key, result, params);
        if (has_value) return result;
    }
    return key;
}

std::string
I18nAPI::get(std::string const& key, std::vector<std::string> const& params, std::string const& languageCode) {
    if (auto localization = getI18n().getLocaleFor(languageCode)) {
        std::string result;
        auto        has_value = localization->get(key, result, params);
        if (has_value) return result;
    }
    return key;
}

std::string I18nAPI::get(
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
