#pragma once
#include "GMLIB/DllExport.h"

namespace GMLIB::ScoreboardAPI {

GMLIB_API Scoreboard* getServerScoreboard();

GMLIB_API Objective* getObjective(std::string& name);
GMLIB_API Objective* addObjective(std::string& name, std::string& displayName);
GMLIB_API bool       removeObjective(Objective* objective);
GMLIB_API bool       removeObjective(std::string& objective);

GMLIB_API std::vector<Objective const*> getObjectives();
GMLIB_API std::vector<std::string> getObjectiveNames();

GMLIB_API ScoreboardId getPlayerScoreboardId(std::string& serverid);
GMLIB_API ScoreboardId getPlayerScoreboardId(mce::UUID& uuid);

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

GMLIB_API bool resetScore(Objective* objective, ScoreboardId& scoreboardId);
GMLIB_API bool resetScore(std::string& objective, std::string& name);
GMLIB_API bool resetScore(std::string& objective, Player* pl);
GMLIB_API bool resetScore(std::string& objective, Actor* ac);

GMLIB_API bool resetPlayerScore(std::string& objective, std::string& serverid);
GMLIB_API bool resetPlayerScore(std::string& objective, mce::UUID& uuid);
GMLIB_API bool resetPlayerScore(std::string& objective, Player* pl);

GMLIB_API bool resetAllScores(ScoreboardId& scoreboardId);
GMLIB_API bool resetAllScores(std::string& name);
GMLIB_API bool resetAllScores(Player* pl);
GMLIB_API bool resetAllScores(Actor* ac);

GMLIB_API bool resetPlayerAllScores(std::string& serverid);
GMLIB_API bool resetPlayerAllScores(mce::UUID& uuid);
GMLIB_API bool resetPlayerAllScores(Player* pl);

GMLIB_API std::vector<ScoreboardId> getAllScoreboardIds();

} // namespace GMLIB::ScoreboardAPI