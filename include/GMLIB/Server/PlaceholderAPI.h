#pragma once
#include "GMLIB/GMLIB.h"

namespace GMLIB::Server {

class PlaceholderAPI {
public:
    std::string mValue;
    std::string mName;

private:
    std::string                                                                       mPluginName;
    int                                                                               mUpdateInterval;
    bool                                                                              mAutoUpdate;
    bool                                                                              mProcessPlayer;
    bool                                                                              mProcessParameters;
    std::function<std::string(Player*)>                                               mCallback;
    std::function<std::string(Player*, std::unordered_map<std::string, std::string>)> mCallbackWithParameters;

public:
    PlaceholderAPI(){};
    PlaceholderAPI(
        std::string                                                                       Name,
        int                                                                               UpdateInterval,
        bool                                                                              AutoUpdate,
        bool                                                                              mProcessPlayer,
        bool                                                                              mProcessParameters,
        std::string                                                                       PluginName,
        std::function<std::string(Player*)>                                               callback,
        std::function<std::string(Player*, std::unordered_map<std::string, std::string>)> CallbackWithParameters
    );
    ~PlaceholderAPI(){};

    GMLIB_API static void registerPlaceholder(PlaceholderAPI papi);

    GMLIB_API static std::string getValue(std::string name, Player* sp);

    GMLIB_API static std::string getValue(std::string name);

    GMLIB_API static void translateString(std::string& papi, Player* sp = nullptr);

    GMLIB_API static void registerStaticPlaceholder(std::string name, std::string value, std::string PluginName = "");

    GMLIB_API static void registerStaticPlaceholder(std::string name, std::string (*Func)(), std::string PluginName);

    GMLIB_API static void
    registerStaticPlaceholder(std::string name, std::function<std::string()> callback, std::string PluginName = "");

    GMLIB_API static void
    registerStaticPlaceholder(std::string name, int UpdateInterval, std::string (*Func)(), std::string PluginName);

    GMLIB_API static void registerStaticPlaceholder(
        std::string                  name,
        int                          UpdateInterval,
        std::function<std::string()> callback,
        std::string                  PluginName = ""
    );

    GMLIB_API static void registerPlayerPlaceholder(
        std::string                         name,
        std::function<std::string(Player*)> callback,
        std::string                         PluginName = ""
    );

    GMLIB_API static void registerPlayerPlaceholder(
        std::string                                                                       name,
        std::function<std::string(Player*, std::unordered_map<std::string, std::string>)> callback,
        std::string                                                                       PluginName = ""
    );

    GMLIB_API static void
    registerServerPlaceholder(std::string name, std::function<std::string()> callback, std::string PluginName = "");

    GMLIB_API static void registerServerPlaceholder(
        std::string                                                              name,
        std::function<std::string(std::unordered_map<std::string, std::string>)> callback,
        std::string                                                              PluginName = ""
    );

    GMLIB_API static bool unRegisterPlaceholder(std::string name);

    GMLIB_API static void Update(PlaceholderAPI a1);

    GMLIB_API static std::unordered_set<std::string> getPAPIPluginsList();

    GMLIB_API static std::vector<PlaceholderAPI> getPAPIInfoList();

    GMLIB_API inline std::string getValue() { return mValue; }

    GMLIB_API inline std::string getPluginName() { return mPluginName; }
};

} // namespace GMLIB::Server