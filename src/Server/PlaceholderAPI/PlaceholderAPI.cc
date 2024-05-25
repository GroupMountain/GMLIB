#include "Global.h"
#include "Server/PlaceholderAPI/Helper.h"
#include <GMLIB/Server/PlaceholderAPI.h>

namespace GMLIB {

namespace Server {

std::unordered_map<std::string, PlaceholderAPI> mGlobalPAPI;
std::unordered_map<std::string, PlaceholderAPI> mUpdatePlaceholders;

PlaceholderAPI::PlaceholderAPI(
    std::string const&                                                                      placeholder,
    int                                                                                     updateInterval,
    bool                                                                                    autoUpdate,
    bool                                                                                    processPlayer,
    bool                                                                                    processParameters,
    std::string const&                                                                      pluginName,
    std::function<std::string(class Player*)>                                               callback,
    std::function<std::string(class Player*, std::unordered_map<std::string, std::string>)> callbackWithParameters
)
: mUpdateInterval(updateInterval),
  mAutoUpdate(autoUpdate),
  mProcessPlayer(processPlayer),
  mProcessParameters(processParameters),
  mCallback(callback),
  mCallbackWithParameters(callbackWithParameters) {
    mPlaceholder = "%" + placeholder + "%";
    mPluginName  = pluginName.empty() ? "unknown" : pluginName;
}

void PlaceholderAPI::update(PlaceholderAPI papi) {
    ll::schedule::GameTickScheduler Scheduler;
    using namespace ll::chrono_literals;
    Scheduler.add<ll::schedule::DelayTask>(ll::chrono::ticks{papi.mUpdateInterval}, [papi] {
        if (mGlobalPAPI.count(papi.mPlaceholder)) {
            if (!papi.mProcessParameters) {
                mGlobalPAPI[papi.mPlaceholder].mValue = papi.mCallback(NULL);
            }
        }
    });
}

bool PlaceholderAPI::registerPlaceholder(PlaceholderAPI placeholderapi) {
    if (mGlobalPAPI.count(placeholderapi.getName())) {
        return false;
    }
    mGlobalPAPI[placeholderapi.getName()] = placeholderapi;
    if (placeholderapi.mUpdateInterval > 0 && placeholderapi.mAutoUpdate) {
        mUpdatePlaceholders[placeholderapi.getName()] = placeholderapi;
    }
    return true;
}

////---------------------------------------Static------------------------------------------

bool PlaceholderAPI::registerStaticPlaceholder(
    std::string const& placeholder,
    std::string const& value,
    std::string const& PluginName
) {
    PlaceholderAPI placeholderapi(
        placeholder,
        -1,
        false,
        false,
        false,
        PluginName,
        [value](Player* sp) { return value; },
        nullptr
    );
    return registerPlaceholder(placeholderapi);
}

bool PlaceholderAPI::registerStaticPlaceholder(
    std::string const&           name,
    std::function<std::string()> callback,
    std::string const&           PluginName
) {
    PlaceholderAPI placeholderapi(
        name,
        -1,
        false,
        false,
        false,
        PluginName,
        [callback](Player* sp) { return callback(); },
        nullptr
    );
    return registerPlaceholder(placeholderapi);
}
bool PlaceholderAPI::registerStaticPlaceholder(
    std::string const& placeholder,
    std::string (*Func)(),
    std::string const& PluginName
) {
    PlaceholderAPI placeholderapi(
        placeholder,
        -1,
        false,
        false,
        false,
        PluginName,
        [Func](Player* sp) { return Func(); },
        nullptr
    );
    return registerPlaceholder(placeholderapi);
}

bool PlaceholderAPI::registerStaticPlaceholder(
    std::string const& placeholder,
    int                UpdateInterval,
    std::string (*Func)(),
    std::string const& PluginName
) {
    PlaceholderAPI placeholderapi(
        placeholder,
        UpdateInterval,
        true,
        false,
        false,
        PluginName,
        [Func](Player* sp) { return Func(); },
        nullptr
    );
    return registerPlaceholder(placeholderapi);
}

bool PlaceholderAPI::registerStaticPlaceholder(
    std::string const&           name,
    int                          UpdateInterval,
    std::function<std::string()> callback,
    std::string const&           PluginName
) {
    PlaceholderAPI placeholderapi(
        name,
        UpdateInterval,
        true,
        false,
        true,
        PluginName,
        [callback](Player* sp) { return callback(); },
        nullptr
    );
    return registerPlaceholder(placeholderapi);
}


//---------------------------------------Server------------------------------------------
bool PlaceholderAPI::registerServerPlaceholder(
    std::string const&           name,
    std::function<std::string()> callback,
    std::string const&           PluginName
) {
    PlaceholderAPI placeholderapi(
        name,
        -1,
        false,
        false,
        false,
        PluginName,
        [callback](Player* sp) { return callback(); },
        nullptr
    );
    return registerPlaceholder(placeholderapi);
}

bool PlaceholderAPI::registerServerPlaceholder(
    std::string const&                                                       name,
    std::function<std::string(std::unordered_map<std::string, std::string>)> callback,
    std::string const&                                                       PluginName
) {
    PlaceholderAPI placeholderapi(
        name,
        -1,
        false,
        false,
        true,
        PluginName,
        nullptr,
        [callback](Player* sp, std::unordered_map<std::string, std::string> map) { return callback(map); }
    );
    return registerPlaceholder(placeholderapi);
}


//---------------------------------------Player------------------------------------------
bool PlaceholderAPI::registerPlayerPlaceholder(
    std::string const&                        name,
    std::function<std::string(class Player*)> callback,
    std::string const&                        PluginName
) {
    PlaceholderAPI placeholderapi(name, -1, false, true, false, PluginName, callback, nullptr);
    return registerPlaceholder(placeholderapi);
}

bool PlaceholderAPI::registerPlayerPlaceholder(
    std::string const&                                                                      name,
    std::function<std::string(class Player*, std::unordered_map<std::string, std::string>)> callback,
    std::string const&                                                                      PluginName
) {
    PlaceholderAPI placeholderapi(name, -1, false, true, true, PluginName, nullptr, callback);
    return registerPlaceholder(placeholderapi);
}

bool PlaceholderAPI::unregisterPlaceholder(std::string const& placeholder) {
    std::string placeholderapi = Helper::checkPAPIName(placeholder);
    bool        isremove       = false;
    placeholderapi             = Helper::checkPAPIName(placeholderapi);
    if (mGlobalPAPI.count(placeholderapi)) {
        mGlobalPAPI.erase(placeholderapi);
        return true;
    } else {
        for (auto& i : mGlobalPAPI) {
            if (i.second.getPluginName() == placeholder) {
                mGlobalPAPI.erase(Helper::checkPAPIName(i.first));
                isremove = true;
            }
        }
        if (isremove) return true;
    }
    return false;
}

std::string PlaceholderAPI::getValue(std::string placeholderapi, optional_ref<Player> sp) {
    placeholderapi = Helper::checkPAPIName(placeholderapi);
    if (mGlobalPAPI.count(placeholderapi)) {
        auto& papi = mGlobalPAPI[placeholderapi];
        if (!papi.mProcessParameters) {
            if (papi.mProcessPlayer) {
                if (sp) {
                    return mGlobalPAPI[placeholderapi].mCallback(sp);
                }
            } else {
                if (papi.mAutoUpdate) {
                    return papi.mValue;
                } else {
                    return papi.mCallback(nullptr);
                }
            }
        }
    } else {
        for (auto& i : mGlobalPAPI) {
            if (i.second.mProcessParameters) {
                auto [out, map] = Helper::FindPlaceholder(i.first, placeholderapi);
                if (out) {
                    return i.second.mCallbackWithParameters(sp, map);
                }
            }
        }
    }
    return placeholderapi;
}

std::string PlaceholderAPI::getValue(std::string placeholderapi) {
    placeholderapi = Helper::checkPAPIName(placeholderapi);
    if (mGlobalPAPI.count(placeholderapi)) {
        auto& papi = mGlobalPAPI[placeholderapi];
        if (!papi.mProcessParameters) {
            if (!papi.mProcessPlayer) {
                if (papi.mAutoUpdate) {
                    return papi.mValue;
                } else {
                    return papi.mCallback(nullptr);
                }
            } else {
                return "Unknown Player";
            }
        }
    } else {
        for (auto& i : mGlobalPAPI) {
            if (i.second.mProcessParameters) {
                auto [out, map] = Helper::FindPlaceholder(i.first, placeholderapi);
                if (out) {
                    return i.second.mCallbackWithParameters(nullptr, map);
                }
            }
        }
    }
    return placeholderapi;
}

void PlaceholderAPI::translate(std::string& value, optional_ref<Player> sp) {
    auto list = Helper::getPercentage(value);
    for (auto& i : list) {
        ll::string_utils::replaceAll(value, i, getValue(i, sp));
    }
}

std::string PlaceholderAPI::translateString(std::string_view value, optional_ref<Player> sp) {
    auto key  = std::string(value);
    auto list = Helper::getPercentage(key);
    for (auto& i : list) {
        ll::string_utils::replaceAll(key, i, getValue(i, sp));
    }
    return key;
}

std::unordered_set<std::string> PlaceholderAPI::getPAPIPluginsList() {
    std::unordered_set<std::string> list;
    for (auto& i : mGlobalPAPI) {
        list.insert(i.second.getPluginName());
    }
    return list;
}

std::vector<PlaceholderAPI> PlaceholderAPI::getPAPIInfoList() {
    std::vector<PlaceholderAPI> list;
    for (auto& i : mGlobalPAPI) {
        list.push_back(i.second);
    }
    return list;
}

std::vector<std::string> PlaceholderAPI::getAllPAPI() {
    std::vector<std::string> list;
    for (auto& i : mGlobalPAPI) {
        list.push_back(i.second.getName());
    }
    return list;
}

std::string PlaceholderAPI::getName() { return mPlaceholder; }

std::string PlaceholderAPI::getValue() { return mValue; }

std::string PlaceholderAPI::getPluginName() { return mPluginName; }

} // namespace Server

void updatePlaceholder() {
    ll::schedule::GameTickScheduler Scheduler;
    using namespace ll::chrono_literals;
    Scheduler.add<ll::schedule::DelayTask>(20_tick, [] {
        for (auto& i : Server::mUpdatePlaceholders) {
            Server::PlaceholderAPI::update(i.second);
        }
        Server::mUpdatePlaceholders.clear();
    });
}

} // namespace GMLIB