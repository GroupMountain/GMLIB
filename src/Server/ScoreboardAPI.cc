#include "Global.h"
#include <GMLIB/Server/PlayerAPI.h>
#include <GMLIB/Server/ScoreboardAPI.h>

GMLIB_Scoreboard* GMLIB_Scoreboard::getServerScoreboard() {
    return (GMLIB_Scoreboard*)&ll::service::bedrock::getLevel()->getScoreboard();
}

Objective* GMLIB_Scoreboard::addObjective(std::string name, std::string displayName) {
    return addObjective(name, displayName, *getCriteria("dummy"));
}

Objective* GMLIB_Scoreboard::addObjective(std::string name) { return addObjective(name, name, *getCriteria("dummy")); }

bool GMLIB_Scoreboard::removeObjective(std::string objective) {
    auto obj = getObjective(objective);
    if (obj) {
        return removeObjective(obj);
    }
    return false;
}

std::optional<int> GMLIB_Scoreboard::getScore(Objective* objective, ScoreboardId& scoreboardId) {
    if (objective && scoreboardId.isValid()) {
        auto scores = getIdScores(scoreboardId);
        for (auto& score : scores) {
            if (score.mObjective == objective) {
                return score.mScore;
            }
        }
    }
    return {};
}

ScoreboardId GMLIB_Scoreboard::getPlayerScoreboardId(std::string serverid) {
    auto nbt = GMLIB_Player::getOfflineNbt(serverid);
    if (nbt && nbt->contains("UniqueID")) {
        auto auid = nbt->getInt64("UniqueID");
        auto psid = PlayerScoreboardId(auid);
        return getScoreboardId(psid);
    }
    return ScoreboardId::INVALID;
}

ScoreboardId GMLIB_Scoreboard::getPlayerScoreboardId(mce::UUID& uuid) {
    auto serverid = GMLIB_Player::getServeridFromUuid(uuid);
    return getPlayerScoreboardId(serverid);
}

std::optional<int> GMLIB_Scoreboard::getScore(std::string objective, std::string name) {
    auto id  = getScoreboardId(name);
    auto obj = getObjective(objective);
    return getScore(obj, id);
}

std::optional<int> GMLIB_Scoreboard::getScore(std::string objective, Player* pl) {
    auto id  = getScoreboardId(*pl);
    auto obj = getObjective(objective);
    return getScore(obj, id);
}

std::optional<int> GMLIB_Scoreboard::getScore(std::string objective, Actor* ac) {
    auto id  = getScoreboardId(*ac);
    auto obj = getObjective(objective);
    return getScore(obj, id);
}

std::optional<int> GMLIB_Scoreboard::getPlayerScore(std::string objective, std::string serverid) {
    auto id  = getPlayerScoreboardId(serverid);
    auto obj = getObjective(objective);
    return getScore(obj, id);
}

std::optional<int> GMLIB_Scoreboard::getPlayerScore(std::string objective, mce::UUID& uuid) {
    auto id  = getPlayerScoreboardId(uuid);
    auto obj = getObjective(objective);
    return getScore(obj, id);
}

std::optional<int> GMLIB_Scoreboard::getPlayerScore(std::string objective, Player* pl) {
    return getScore(objective, pl);
}

std::optional<int>
GMLIB_Scoreboard::setScore(Objective* objective, ScoreboardId& scoreboardId, int value, PlayerScoreSetFunction action) {
    if (objective && scoreboardId.isValid()) {
        bool success = true;
        return modifyPlayerScore(success, scoreboardId, *objective, value, action);
    }
    return {};
}

std::optional<int>
GMLIB_Scoreboard::setScore(std::string objective, std::string name, int value, PlayerScoreSetFunction action) {
    auto id  = getScoreboardId(name);
    auto obj = getObjective(objective);
    if (!obj) {
        obj = addObjective(objective);
    }
    return setScore(obj, id, value, action);
}

std::optional<int>
GMLIB_Scoreboard::setScore(std::string objective, Player* pl, int value, PlayerScoreSetFunction action) {
    auto id  = getScoreboardId(*pl);
    auto obj = getObjective(objective);
    if (!obj) {
        obj = addObjective(objective);
    }
    return setScore(obj, id, value, action);
}

std::optional<int>
GMLIB_Scoreboard::setScore(std::string objective, Actor* ac, int value, PlayerScoreSetFunction action) {
    auto id  = getScoreboardId(*ac);
    auto obj = getObjective(objective);
    if (!obj) {
        obj = addObjective(objective);
    }
    return setScore(obj, id, value, action);
}

std::optional<int> GMLIB_Scoreboard::setPlayerScore(
    std::string            objective,
    std::string            serverid,
    int                    value,
    PlayerScoreSetFunction action
) {
    auto id  = getPlayerScoreboardId(serverid);
    auto obj = getObjective(objective);
    if (!obj) {
        obj = addObjective(objective);
    }
    return setScore(obj, id, value, action);
}

std::optional<int>
GMLIB_Scoreboard::setPlayerScore(std::string objective, mce::UUID& uuid, int value, PlayerScoreSetFunction action) {
    auto id  = getPlayerScoreboardId(uuid);
    auto obj = getObjective(objective);
    if (!obj) {
        obj = addObjective(objective);
    }
    return setScore(obj, id, value, action);
}

std::optional<int>
GMLIB_Scoreboard::setPlayerScore(std::string objective, Player* pl, int value, PlayerScoreSetFunction action) {
    return setScore(objective, pl, value, action);
}

bool GMLIB_Scoreboard::resetScore(Objective* objective, ScoreboardId& scoreboardId) {
    if (objective && scoreboardId.isValid()) {
        return resetPlayerScore(scoreboardId, *objective);
    }
    return false;
}

bool GMLIB_Scoreboard::resetScore(std::string objective, std::string name) {
    auto id  = getScoreboardId(name);
    auto obj = getObjective(objective);
    return resetScore(obj, id);
}

bool GMLIB_Scoreboard::resetScore(std::string objective, Player* pl) {
    auto id  = getScoreboardId(*pl);
    auto obj = getObjective(objective);
    return resetScore(obj, id);
}

bool GMLIB_Scoreboard::resetScore(std::string objective, Actor* ac) {
    auto id  = getScoreboardId(*ac);
    auto obj = getObjective(objective);
    return resetScore(obj, id);
}

bool GMLIB_Scoreboard::resetPlayerScore(std::string objective, std::string serverid) {
    auto id  = getPlayerScoreboardId(serverid);
    auto obj = getObjective(objective);
    return resetScore(obj, id);
}

bool GMLIB_Scoreboard::resetPlayerScore(std::string objective, mce::UUID& uuid) {
    auto id  = getPlayerScoreboardId(uuid);
    auto obj = getObjective(objective);
    return resetScore(obj, id);
}

bool GMLIB_Scoreboard::resetPlayerScore(std::string objective, Player* pl) { return resetScore(objective, pl); }

bool GMLIB_Scoreboard::resetAllScores(ScoreboardId& scoreboardId) {
    if (scoreboardId.isValid()) {
        resetPlayerScore(scoreboardId);
        return true;
    }
    return false;
}

bool GMLIB_Scoreboard::resetAllScores(std::string name) {
    auto id = getScoreboardId(name);
    return resetAllScores(id);
}

bool GMLIB_Scoreboard::resetAllScores(Player* pl) {
    auto id = getScoreboardId(*pl);
    return resetAllScores(id);
}

bool GMLIB_Scoreboard::resetAllScores(Actor* ac) {
    auto id = getScoreboardId(*ac);
    return resetAllScores(id);
}

bool GMLIB_Scoreboard::resetPlayerAllScores(std::string serverid) {
    auto id = getPlayerScoreboardId(serverid);
    return resetAllScores(id);
}

bool GMLIB_Scoreboard::resetPlayerAllScores(mce::UUID& uuid) {
    auto id = getPlayerScoreboardId(uuid);
    return resetAllScores(id);
}

bool GMLIB_Scoreboard::resetPlayerAllScores(Player* pl) { return resetAllScores(pl); }

std::vector<ScoreboardId> GMLIB_Scoreboard::getAllScoreboardIds() { return getTrackedIds(); }

std::optional<std::string> GMLIB_Scoreboard::getObjectiveDisplayName(std::string objective) {
    auto obj = getServerScoreboard()->getObjective(objective);
    if (obj) {
        return obj->getDisplayName();
    }
    return {};
}

bool GMLIB_Scoreboard::setObjectiveDisplayName(std::string objective, std::string newName) {
    auto obj = getServerScoreboard()->getObjective(objective);
    if (obj) {
        obj->mDisplayName = newName;
        return true;
    }
    return false;
}