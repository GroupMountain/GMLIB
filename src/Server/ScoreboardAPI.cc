#include "Global.h"
#include <GMLIB/Server/PlayerAPI.h>
#include <GMLIB/Server/ScoreboardAPI.h>

namespace GMLIB::ScoreboardAPI {

Scoreboard* getServerScoreboard() { return &ll::service::bedrock::getLevel()->getScoreboard(); }

Objective* addObjective(std::string& name, std::string& displayName) {
    return getServerScoreboard()->addObjective(name, displayName, *getServerScoreboard()->getCriteria("dummy"));
}

Objective* getObjective(std::string& name) { return getServerScoreboard()->getObjective(name); }

bool removeObjective(Objective* objective) { return getServerScoreboard()->removeObjective(objective); }

bool removeObjective(std::string& objective) {
    auto obj = getObjective(objective);
    if (obj) {
        return getServerScoreboard()->removeObjective(obj);
    }
    return false;
}

std::vector<Objective const*> getObjectives() {
    return getServerScoreboard()->getObjectives();
}

std::vector<std::string> getObjectiveNames() {
    return getServerScoreboard()->getObjectiveNames();
}

std::optional<int> getScore(Objective* objective, ScoreboardId& scoreboardId) {
    if (objective && scoreboardId.isValid()) {
        auto scores = getServerScoreboard()->getIdScores(scoreboardId);
        for (auto& score : scores) {
            if (score.mObjective == objective) {
                return score.mScore;
            }
        }
    }
    return {};
}

ScoreboardId getPlayerScoreboardId(std::string& serverid) {
    auto nbt = GMLIB::PlayerAPI::getOfflineNbt(serverid);
    if (nbt && nbt->contains("UniqueID")) {
        auto auid = nbt->getInt64("UniqueID");
        auto psid = PlayerScoreboardId(auid);
        return getServerScoreboard()->getScoreboardId(psid);
    }
    return ScoreboardId::INVALID;
}

ScoreboardId getPlayerScoreboardId(mce::UUID& uuid) {
    auto serverid = GMLIB::PlayerAPI::getServeridFromUuid(uuid);
    return getPlayerScoreboardId(serverid);
}

std::optional<int> getScore(std::string& objective, std::string& name) {
    auto id  = getServerScoreboard()->getScoreboardId(name);
    auto obj = getServerScoreboard()->getObjective(objective);
    return getScore(obj, id);
}

std::optional<int> getScore(std::string& objective, Player* pl) {
    auto id  = getServerScoreboard()->getScoreboardId(*pl);
    auto obj = getServerScoreboard()->getObjective(objective);
    return getScore(obj, id);
}

std::optional<int> getScore(std::string& objective, Actor* ac) {
    auto id  = getServerScoreboard()->getScoreboardId(*ac);
    auto obj = getServerScoreboard()->getObjective(objective);
    return getScore(obj, id);
}

std::optional<int> getPlayerScore(std::string& objective, std::string& serverid) {
    auto id  = getPlayerScoreboardId(serverid);
    auto obj = getServerScoreboard()->getObjective(objective);
    return getScore(obj, id);
}

std::optional<int> getPlayerScore(std::string& objective, mce::UUID& uuid) {
    auto id  = getPlayerScoreboardId(uuid);
    auto obj = getServerScoreboard()->getObjective(objective);
    return getScore(obj, id);
}

std::optional<int> getPlayerScore(std::string& objective, Player* pl) { return getScore(objective, pl); }

std::optional<int>
setScore(Objective* objective, ScoreboardId& scoreboardId, int value, PlayerScoreSetFunction action) {
    if (objective && scoreboardId.isValid()) {
        bool success = true;
        return getServerScoreboard()->modifyPlayerScore(success, scoreboardId, *objective, value, action);
    }
    return {};
}

std::optional<int> setScore(std::string& objective, std::string& name, int value, PlayerScoreSetFunction action) {
    auto id  = getServerScoreboard()->getScoreboardId(name);
    auto obj = getServerScoreboard()->getObjective(objective);
    return setScore(obj, id, value, action);
}

std::optional<int> setScore(std::string& objective, Player* pl, int value, PlayerScoreSetFunction action) {
    auto id  = getServerScoreboard()->getScoreboardId(*pl);
    auto obj = getServerScoreboard()->getObjective(objective);
    return setScore(obj, id, value, action);
}

std::optional<int> setScore(std::string& objective, Actor* ac, int value, PlayerScoreSetFunction action) {
    auto id  = getServerScoreboard()->getScoreboardId(*ac);
    auto obj = getServerScoreboard()->getObjective(objective);
    return setScore(obj, id, value, action);
}

std::optional<int>
setPlayerScore(std::string& objective, std::string& serverid, int value, PlayerScoreSetFunction action) {
    auto id  = getPlayerScoreboardId(serverid);
    auto obj = getServerScoreboard()->getObjective(objective);
    return setScore(obj, id, value, action);
}

std::optional<int> setPlayerScore(std::string& objective, mce::UUID& uuid, int value, PlayerScoreSetFunction action) {
    auto id  = getPlayerScoreboardId(uuid);
    auto obj = getServerScoreboard()->getObjective(objective);
    return setScore(obj, id, value, action);
}

std::optional<int> setPlayerScore(std::string& objective, Player* pl, int value, PlayerScoreSetFunction action) {
    return setScore(objective, pl, value, action);
}

bool resetScore(Objective* objective, ScoreboardId& scoreboardId) {
    if (objective && scoreboardId.isValid()) {
        return getServerScoreboard()->resetPlayerScore(scoreboardId, *objective);
    }
    return false;
}

bool resetScore(std::string& objective, std::string& name) {
    auto id  = getServerScoreboard()->getScoreboardId(name);
    auto obj = getServerScoreboard()->getObjective(objective);
    return resetScore(obj, id);
}

bool resetScore(std::string& objective, Player* pl) {
    auto id  = getServerScoreboard()->getScoreboardId(*pl);
    auto obj = getServerScoreboard()->getObjective(objective);
    return resetScore(obj, id);
}

bool resetScore(std::string& objective, Actor* ac) {
    auto id  = getServerScoreboard()->getScoreboardId(*ac);
    auto obj = getServerScoreboard()->getObjective(objective);
    return resetScore(obj, id);
}

bool resetPlayerScore(std::string& objective, std::string& serverid) {
    auto id  = getPlayerScoreboardId(serverid);
    auto obj = getServerScoreboard()->getObjective(objective);
    return resetScore(obj, id);
}

bool resetPlayerScore(std::string& objective, mce::UUID& uuid) {
    auto id  = getPlayerScoreboardId(uuid);
    auto obj = getServerScoreboard()->getObjective(objective);
    return resetScore(obj, id);
}

bool resetPlayerScore(std::string& objective, Player* pl) {
    return resetScore(objective, pl);
}

bool resetAllScores(ScoreboardId& scoreboardId) {
    if (scoreboardId.isValid()) {
        getServerScoreboard()->resetPlayerScore(scoreboardId);
        return true;
    }
    return false;
}

bool resetAllScores(std::string& name) {
    auto id  = getServerScoreboard()->getScoreboardId(name);
    return resetAllScores(id);
}

bool resetAllScores(Player* pl) {
    auto id  = getServerScoreboard()->getScoreboardId(*pl);
    return resetAllScores(id);
}

bool resetAllScores(Actor* ac) {
    auto id  = getServerScoreboard()->getScoreboardId(*ac);
    return resetAllScores(id);
}

bool resetPlayerAllScores(std::string& serverid) {
    auto id  = getPlayerScoreboardId(serverid);
    return resetAllScores(id);
}

bool resetPlayerAllScores(mce::UUID& uuid) {
    auto id  = getPlayerScoreboardId(uuid);
    return resetAllScores(id);
}

bool resetPlayerAllScores(Player* pl) { return resetAllScores(pl); }

std::vector<ScoreboardId> getAllScoreboardIds() { return getServerScoreboard()->getTrackedIds(); }

} // namespace GMLIB::ScoreboardAPI