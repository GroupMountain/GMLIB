#include "GMLIB/Macros.h"
#include "mc/locale/I18n.h"
#include "mc/locale/Localization.h"

I18n& getI18n();

class I18nAPI {
public:
    GMLIB_API static void chooseLanguage(std::string const& languageCode);

    GMLIB_API static void chooseLanguage(class Localization const& localization);

    GMLIB_API static optional_ref<const Localization> getCurrentLanguage();

    GMLIB_API static std::optional<std::string> getCurrentLanguageCode();

    GMLIB_API static std::optional<std::string>
    tryGet(std::string const& key, std::vector<std::string> const& params = {});

    GMLIB_API static std::optional<std::string>
    tryGet(std::string const& key, std::vector<std::string> const& params, std::string const& languageCode);

    GMLIB_API static std::optional<std::string> tryGet(
        std::string const&                  key,
        std::vector<std::string> const&     params,
        std::shared_ptr<class Localization> localization
    );

    GMLIB_API static std::string get(std::string const& key, std::vector<std::string> const& params = {});

    GMLIB_API static std::string
    get(std::string const& key, std::vector<std::string> const& params, std::string const& languageCode);

    GMLIB_API static std::string
    get(std::string const& key, std::vector<std::string> const& params, std::shared_ptr<class Localization> localization
    );
};