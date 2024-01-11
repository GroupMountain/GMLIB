#pragma once
#include "Global.h"
#include <GMLIB/Server/FakeListAPI.h>

namespace GMLIB::FakeListAPI {

inline std::unordered_set<std::string>                  mInvisibleMap;
inline std::unordered_map<std::string, std::string>     mReplaceMap;
inline std::unordered_map<std::string, PlayerListEntry> mFakeListMap;
inline bool                                             mSimulatedPlayerOptList = false;

} // namespace GMLIB::FakeListAPI