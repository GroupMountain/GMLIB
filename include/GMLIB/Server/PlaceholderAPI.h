#pragma once
#include "GMLIB/Macros.h"
#include "mc/world/actor/player/Player.h"

namespace GMLIB::Server {

class PlaceholderAPI {
private:
    std::string                                                                       mValue;
    std::string                                                                       mPlaceholder;
    std::string                                                                       mPluginName;
    int                                                                               mUpdateInterval;
    bool                                                                              mAutoUpdate;
    bool                                                                              mProcessPlayer;
    bool                                                                              mProcessParameters;
    std::function<std::string(Player*)>                                               mCallback;
    std::function<std::string(Player*, std::unordered_map<std::string, std::string>)> mCallbackWithParameters;

public:
    PlaceholderAPI() = default;

    PlaceholderAPI(
        std::string const&                                                                Name,
        int                                                                               UpdateInterval,
        bool                                                                              AutoUpdate,
        bool                                                                              ProcessPlayer,
        bool                                                                              ProcessParameters,
        std::string const&                                                                PluginName,
        std::function<std::string(Player*)>                                               callback,
        std::function<std::string(Player*, std::unordered_map<std::string, std::string>)> CallbackWithParameters
    );

public:
    virtual ~PlaceholderAPI() = default;

public:
    GMLIB_NDAPI static std::string getValue(std::string placeholder, optional_ref<Player> player);

    GMLIB_NDAPI static std::string getValue(std::string placeholder);

    GMLIB_API static void translate(std::string& value, optional_ref<Player> sp = nullptr);

    GMLIB_NDAPI static std::string translateString(std::string_view value, optional_ref<Player> sp = nullptr);

    GMLIB_API static bool
    registerStaticPlaceholder(std::string const& placeholder, std::string const& value, std::string const& pluginName);

    GMLIB_API static bool
    registerStaticPlaceholder(std::string const& placeholder, std::string (*Func)(), std::string const& pluginName);

    GMLIB_API static bool registerStaticPlaceholder(
        std::string const&           placeholder,
        std::function<std::string()> callback,
        std::string const&           pluginName
    );

    GMLIB_API static bool registerStaticPlaceholder(
        std::string const& placeholder,
        int                UpdateInterval,
        std::string (*Func)(),
        std::string const& pluginName
    );

    GMLIB_API static bool registerStaticPlaceholder(
        std::string const&           name,
        int                          UpdateInterval,
        std::function<std::string()> callback,
        std::string const&           pluginName
    );

    GMLIB_API static bool registerPlayerPlaceholder(
        std::string const&                  name,
        std::function<std::string(Player*)> callback,
        std::string const&                  pluginName
    );

    GMLIB_API static bool registerPlayerPlaceholder(
        std::string const&                                                                name,
        std::function<std::string(Player*, std::unordered_map<std::string, std::string>)> callback,
        std::string const&                                                                PluginName
    );

    GMLIB_API static bool registerServerPlaceholder(
        std::string const&           placeholder,
        std::function<std::string()> callback,
        std::string const&           pluginName
    );

    GMLIB_API static bool registerServerPlaceholder(
        std::string const&                                                       name,
        std::function<std::string(std::unordered_map<std::string, std::string>)> callback,
        std::string const&                                                       pluginName
    );

    GMLIB_API static bool unregisterPlaceholder(std::string const& placeholder);

    GMLIB_API static void update(PlaceholderAPI papi);

    GMLIB_NDAPI static std::unordered_set<std::string> getPAPIPluginsList();

    GMLIB_NDAPI static std::vector<PlaceholderAPI> getPAPIInfoList();

    GMLIB_NDAPI static std::vector<std::string> getAllPAPI();

public:
    GMLIB_API std::string getName();

    GMLIB_API std::string getValue();

    GMLIB_API std::string getPluginName();

private:
    static bool registerPlaceholder(PlaceholderAPI papi);
};

} // namespace GMLIB::Server