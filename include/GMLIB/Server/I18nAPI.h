#pragma once
#include "GMLIB/Files/Language/McLang.h"
#include "GMLIB/Macros.h"
#include "mc/locale/I18n.h"
#include "mc/locale/Localization.h"
#include "nlohmann/json.hpp"

class I18nAPI {
public:
    GMLIB_API static void chooseLanguage(std::string const& languageCode);

    GMLIB_API static void chooseLanguage(class Localization const& localization);

    GMLIB_NDAPI static optional_ref<const Localization> getCurrentLanguage();

    GMLIB_NDAPI static std::string getCurrentLanguageCode();

    GMLIB_NDAPI static std::vector<std::string> getSupportedLanguageCodes();

    GMLIB_NDAPI static std::optional<std::string>
    tryGet(std::string const& key, std::vector<std::string> const& params = {});

    GMLIB_NDAPI static std::optional<std::string>
    tryGet(std::string const& key, std::vector<std::string> const& params, std::string const& languageCode);

    GMLIB_NDAPI static std::optional<std::string> tryGet(
        std::string const&                  key,
        std::vector<std::string> const&     params,
        std::shared_ptr<class Localization> localization
    );

    GMLIB_NDAPI static std::string get(std::string const& key, std::vector<std::string> const& params = {});

    GMLIB_NDAPI static std::string
    get(std::string const& key, std::vector<std::string> const& params, std::string const& languageCode);

    GMLIB_NDAPI static std::string
    get(std::string const& key, std::vector<std::string> const& params, std::shared_ptr<class Localization> localization
    );

public:
    GMLIB_API static void
    loadLanguage(std::string const& languageCode, std::unordered_map<std::string, std::string> const& language);

    GMLIB_API static void loadLanguage(std::string const& languageCode, GMLIB::Files::McLang const& language);

    GMLIB_API static void loadLanguage(std::string const& languageCode, nlohmann::json const& language);

    GMLIB_API static void loadLanguageFromFile(std::string const& languageCode, std::filesystem::path const& path);

    GMLIB_API static void updateOrCreateLanguageFile(
        std::filesystem::path const& path,
        std::string const&           languageCode,
        std::string const&           language
    );

    GMLIB_API static void updateOrCreateLanguageFile(
        std::filesystem::path const& path,
        std::string const&           languageCode,
        GMLIB::Files::McLang const&  language
    );

    GMLIB_API static void updateOrCreateLanguageFile(
        std::filesystem::path const&                        path,
        std::string const&                                  languageCode,
        std::unordered_map<std::string, std::string> const& language
    );

    GMLIB_API static void updateOrCreateLanguageFile(
        std::filesystem::path const& path,
        std::string const&           languageCode,
        nlohmann::json const&        language
    );

    GMLIB_API static void loadLanguagesFromDirectory(std::filesystem::path const& path);
};