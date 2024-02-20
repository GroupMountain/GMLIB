#include "Global.h"
#include "Server/PlaceholderAPI/Helper.h"
#include <GMLIB/Server/PlaceholderAPI.h>

namespace GMLIB {

namespace Server {

std::unordered_map<std::string, PlaceholderAPI> mGlobalPAPI;
std::unordered_map<std::string, PlaceholderAPI> mUpdatePlaceholders;

PlaceholderAPI::PlaceholderAPI(
    std::string                                                                             Name,
    int                                                                                     UpdateInterval,
    bool                                                                                    AutoUpdate,
    bool                                                                                    ProcessPlayer,
    bool                                                                                    ProcessParameters,
    std::string                                                                             PluginName,
    std::function<std::string(class Player*)>                                               callback,
    std::function<std::string(class Player*, std::unordered_map<std::string, std::string>)> CallbackWithParameters
) {
    mName                   = "%" + Name + "%";
    mUpdateInterval         = UpdateInterval;
    mAutoUpdate             = AutoUpdate;
    mProcessParameters      = ProcessParameters;
    mProcessPlayer          = ProcessPlayer;
    mCallback               = callback;
    mCallbackWithParameters = CallbackWithParameters;
    if (PluginName.empty()) {
        mPluginName = "UNKNOWN";
    } else {
        mPluginName = PluginName;
    }
}

void PlaceholderAPI::Update(PlaceholderAPI a1) {
    ll::schedule::GameTickScheduler Scheduler;
    using namespace ll::chrono_literals;

    Scheduler.add<ll::schedule::DelayTask>(ll::chrono::ticks{a1.mUpdateInterval}, [a1] {
        if (mGlobalPAPI.find(a1.mName) != mGlobalPAPI.end()) {
            if (!a1.mProcessParameters) {
                mGlobalPAPI.at(a1.mName).mValue = a1.mCallback(NULL);
            }
        }
    });
}

void PlaceholderAPI::registerPlaceholder(PlaceholderAPI a1) {
    for (auto& i : mGlobalPAPI) {
        if (i.second.mName == a1.mName) {
            // logger.warn("Placeholder {} tried to register which is already used", i.second.mName);
            return;
        }
    }
    mGlobalPAPI.emplace(std::pair{a1.mName, a1});
    if (a1.mUpdateInterval > 0 && a1.mAutoUpdate) {
        mUpdatePlaceholders.emplace(std::pair{a1.mName, a1});
    }
}

////---------------------------------------Static------------------------------------------

void PlaceholderAPI::registerStaticPlaceholder(std::string name, std::string value, std::string PluginName) {
    PlaceholderAPI a1(
        name,
        -1,
        false,
        false,
        false,
        PluginName,
        [value](Player* sp) { return value; },
        nullptr
    );
    registerPlaceholder(a1);
}

void PlaceholderAPI::registerStaticPlaceholder(
    std::string                  name,
    std::function<std::string()> callback,
    std::string                  PluginName
) {
    PlaceholderAPI a1(
        name,
        -1,
        false,
        false,
        false,
        PluginName,
        [callback](Player* sp) { return callback(); },
        nullptr
    );
    registerPlaceholder(a1);
}
void PlaceholderAPI::registerStaticPlaceholder(std::string name, std::string (*Func)(), std::string PluginName) {
    PlaceholderAPI a1(
        name,
        -1,
        false,
        false,
        false,
        PluginName,
        [Func](Player* sp) { return Func(); },
        nullptr
    );
    registerPlaceholder(a1);
}

void PlaceholderAPI::registerStaticPlaceholder(
    std::string name,
    int         UpdateInterval,
    std::string (*Func)(),
    std::string PluginName
) {
    PlaceholderAPI a1(
        name,
        UpdateInterval,
        true,
        false,
        false,
        PluginName,
        [Func](Player* sp) { return Func(); },
        nullptr
    );
    registerPlaceholder(a1);
}

void PlaceholderAPI::registerStaticPlaceholder(
    std::string                  name,
    int                          UpdateInterval,
    std::function<std::string()> callback,
    std::string                  PluginName
) {
    PlaceholderAPI a1(
        name,
        UpdateInterval,
        true,
        false,
        true,
        PluginName,
        [callback](Player* sp) { return callback(); },
        nullptr
    );
    registerPlaceholder(a1);
}


//---------------------------------------Server------------------------------------------
void PlaceholderAPI::registerServerPlaceholder(
    std::string                  name,
    std::function<std::string()> callback,
    std::string                  PluginName
) {
    PlaceholderAPI a1(
        name,
        -1,
        false,
        false,
        false,
        PluginName,
        [callback](Player* sp) { return callback(); },
        nullptr
    );
    registerPlaceholder(a1);
}

void PlaceholderAPI::registerServerPlaceholder(
    std::string                                                              name,
    std::function<std::string(std::unordered_map<std::string, std::string>)> callback,
    std::string                                                              PluginName
) {
    PlaceholderAPI a1(
        name,
        -1,
        false,
        false,
        true,
        PluginName,
        nullptr,
        [callback](Player* sp, std::unordered_map<std::string, std::string> map) { return callback(map); }
    );
    registerPlaceholder(a1);
}


//---------------------------------------Player------------------------------------------
void PlaceholderAPI::registerPlayerPlaceholder(
    std::string                               name,
    std::function<std::string(class Player*)> callback,
    std::string                               PluginName
) {
    PlaceholderAPI a1(name, -1, false, true, false, PluginName, callback, nullptr);
    registerPlaceholder(a1);
}

void PlaceholderAPI::registerPlayerPlaceholder(
    std::string                                                                             name,
    std::function<std::string(class Player*, std::unordered_map<std::string, std::string>)> callback,
    std::string                                                                             PluginName
) {
    PlaceholderAPI a1(name, -1, false, true, true, PluginName, nullptr, callback);
    registerPlaceholder(a1);
}

bool PlaceholderAPI::unRegisterPlaceholder(std::string name) {
    std::string a1       = Helper::checkPAPIName(name);
    bool        isremove = false;
    a1                   = Helper::checkPAPIName(a1);
    if (mGlobalPAPI.find(a1) != mGlobalPAPI.end()) {
        mGlobalPAPI.erase(a1);
        return true;
    } else {
        for (auto& i : mGlobalPAPI) {
            if (i.second.getPluginName() == name) {
                mGlobalPAPI.erase(Helper::checkPAPIName(i.first));
                isremove = true;
            }
        }
        if (isremove) return true;
    }
    return false;
}

std::string PlaceholderAPI::getValue(std::string a1, Player* sp) {
    a1 = Helper::checkPAPIName(a1);
    if (mGlobalPAPI.find(a1) != mGlobalPAPI.end()) {
        auto& papi = mGlobalPAPI.at(a1);
        if (!papi.mProcessParameters) {
            if (papi.mProcessPlayer) {
                return mGlobalPAPI.at(a1).mCallback(sp);
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
                auto [out, map] = Helper::FindPlaceholder(i.first, a1);
                if (out) {
                    return i.second.mCallbackWithParameters(sp, map);
                }
            }
        }
    }
    return a1;
}

std::string PlaceholderAPI::getValue(std::string a1) {
    a1 = Helper::checkPAPIName(a1);
    if (mGlobalPAPI.find(a1) != mGlobalPAPI.end()) {
        auto& papi = mGlobalPAPI.at(a1);
        if (!papi.mProcessParameters) {
            if (!papi.mProcessPlayer) {
                if (papi.mAutoUpdate) {
                    return papi.mValue;
                } else {
                    return papi.mCallback(nullptr);
                }
            } else return "Unknown Player";
        }
    } else {
        for (auto& i : mGlobalPAPI) {
            if (i.second.mProcessParameters) {
                auto [out, map] = Helper::FindPlaceholder(i.first, a1);
                if (out) {
                    return i.second.mCallbackWithParameters(nullptr, map);
                }
            }
        }
    }
    return a1;
}

void PlaceholderAPI::translateString(std::string& a0, Player* sp) {
    auto list = Helper::getPercentage(a0);
    for (auto& i : list) {
        ll::string_utils::replaceAll(a0, i, getValue(i, sp));
    }
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

} // namespace Server

void updatePlaceholder() {
    ll::schedule::GameTickScheduler Scheduler;
    using namespace ll::chrono_literals;

    Scheduler.add<ll::schedule::DelayTask>(20_tick, [] {
        for (auto& i : Server::mUpdatePlaceholders) {
            Server::PlaceholderAPI::Update(i.second);
        }
        Server::mUpdatePlaceholders.clear();
    });
}

} // namespace GMLIB
  //---------------------------------------RemoteCall------------------------------------------
  /*
  namespace RemoteCall {
  std::std::string GetValue(std::std::string const& from)
  {
  
      return	PlaceholderAPI::getValue(from);
  }
  std::std::string GetValueWithPlayer(std::std::string const& a1, std::std::string const& a2)
  {
      return	PlaceholderAPI::getValue(a1, ll::service::bedrock::getLevel()->getPlayer(a2));
  }
  
  std::std::string registerPlayerPlaceholder(std::std::string const& PluginName, std::std::string const& FuncName,
  std::std::string const&   PAPIName)
  {
      if (RemoteCall::hasFunc(PluginName, FuncName)) {
          if (Helper::isParameters(PAPIName)) {
              auto Call = RemoteCall::importAs<std::string(std::std::string const& arg, std::unordered_map<std::string,
  std::string>)>(PluginName, FuncName); PlaceholderAPI::registerPlayerPlaceholder(PAPIName, [Call](Player* sp,
  std::unordered_map<std::string, std::string> map) { return Call(sp->getXuid(), map);
                  }, PluginName);
          }
          else {
              auto Call = RemoteCall::importAs<std::string(std::std::string const& arg)>(PluginName, FuncName);
              PlaceholderAPI::registerPlayerPlaceholder(PAPIName, [Call](Player* sp) {
                      return Call(sp->getXuid());
                  }, PluginName);
          }
      }
      else {
          //logger.error("Function no find({}:{})", PluginName, FuncName);
          return "Function no find";
      }
      return "Register Success";
  }
  
  std::std::string registerServerPlaceholder(std::std::string const& PluginName, std::std::string const& FuncName,
  std::std::string const&   PAPIName)
  {
      if (RemoteCall::hasFunc(PluginName, FuncName)) {
          if (Helper::isParameters(PAPIName)) {
              auto Call = RemoteCall::importAs<std::string(std::unordered_map<std::string, std::string>)>(PluginName,
  FuncName);   PlaceholderAPI::registerServerPlaceholder(PAPIName, [Call](std::unordered_map<std::string, std::string>
  map)   {   return Call(map);
                  }, PluginName);
          }
          else {
              auto Call = RemoteCall::importAs<std::string()>(PluginName, FuncName);
              PlaceholderAPI::registerServerPlaceholder(PAPIName, [Call]() {
                      return Call();
                  }, PluginName);
          }
      }
      else {
          //logger.error("Function no find({}:{})", PluginName, FuncName);
          return "Function no find";
      }
      return "Register Success";
  }
  
  std::std::string registerStaticPlaceholder(std::std::string const& PluginName, std::std::string const& FuncName,
  std::std::string const&   PAPIName,int num)
  {
      if (RemoteCall::hasFunc(PluginName, FuncName)) {
          if (Helper::isParameters(PAPIName)) {
              auto Call = RemoteCall::importAs<std::string()>(PluginName, FuncName);
              if (num == -1) {
                  PlaceholderAPI::registerStaticPlaceholder(PAPIName, [Call] {
                          return Call();
                      }, PluginName);
              }
              else {
                  PlaceholderAPI::registerStaticPlaceholder(PAPIName, num, [Call] {
                          return Call();
                      }, PluginName);
              }
          }
      }
      else{
          //logger.error("Function no find({}:{})", PluginName, FuncName);
          return "Function no find";
      }
      return "Register Success";
  }
  
  std::std::string translatestd::String(std::std::string const& str, std::std::string const& xuid)
  {
      std::string _str = str;
      PlaceholderAPI::translatestd::String(_str,ll::service::bedrock::getLevel()->getPlayerByXuid(xuid));
      return _str;
  }
  
  bool unRegisterPlaceholder(std::std::string const& str)
  {
      return PlaceholderAPI::unRegisterPlaceholder(str);
  }
  }
  */