#pragma once
#include "GMLIB/DllExport.h"

namespace GMLIB::ScoreboardAPI {

GMLIB_API Objective* addObjective(std::string& name, std::string& displayName);
GMLIB_API Objective* getObjective(std::string& name);

GMLIB_API ScoreboardId getOfflinePlayerScoreboardId(std::string& serverid);
GMLIB_API ScoreboardId getOfflinePlayerScoreboardId(mce::UUID& uuid);

GMLIB_API std::optional<int> getScore(Objective* objective, ScoreboardId& scoreboardId);
GMLIB_API std::optional<int> getScore(std::string& objective, std::string& name);
GMLIB_API std::optional<int> getScore(std::string& objective, Player* pl);
GMLIB_API std::optional<int> getScore(std::string& objective, Actor* ac);

GMLIB_API std::optional<int> getPlayerScore(std::string& objective, std::string& serverid);
GMLIB_API std::optional<int> getPlayerScore(std::string& objective, mce::UUID& uuid);
GMLIB_API std::optional<int> getPlayerScore(std::string& objective, Player* pl);

GMLIB_API std::optional<int>
          setScore(Objective* objective, ScoreboardId& scoreboardId, int value, PlayerScoreSetFunction action);
GMLIB_API std::optional<int>
          setScore(std::string& objective, std::string& name, int value, PlayerScoreSetFunction action);
GMLIB_API std::optional<int> setScore(std::string& objective, Player* pl, int value, PlayerScoreSetFunction action);
GMLIB_API std::optional<int> setScore(std::string& objective, Actor* ac, int value, PlayerScoreSetFunction action);

GMLIB_API std::optional<int>
          setPlayerScore(std::string& objective, std::string& serverid, int value, PlayerScoreSetFunction action);
GMLIB_API std::optional<int>
          setPlayerScore(std::string& objective, mce::UUID& uuid, int value, PlayerScoreSetFunction action);
GMLIB_API std::optional<int>
          setPlayerScore(std::string& objective, Player* pl, int value, PlayerScoreSetFunction action);

} // namespace GMLIB::ScoreboardAPI