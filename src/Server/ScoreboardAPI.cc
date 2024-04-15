#include "Global.h"
#include <GMLIB/Server/PlayerAPI.h>
#include <GMLIB/Server/ScoreboardAPI.h>
#include <mc/world/scores/ScoreInfo.h>

GMLIB_Scoreboard* GMLIB_Scoreboard::getInstance() {
    return (GMLIB_Scoreboard*)&ll::service::bedrock::getLevel()->getScoreboard();
}

ServerScoreboard* GMLIB_Scoreboard::getServerScoreboard() { return (ServerScoreboard*)this; }

std::unordered_map<std::string, DisplayObjective> GMLIB_Scoreboard::getDisplayObjectives() {
    return mDisplayObjectives;
}

IdentityDictionary* GMLIB_Scoreboard::getIdentityDictionary() { return &mIdentityDict; }

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

ScoreboardId GMLIB_Scoreboard::getPlayerScoreboardId(std::string serverId) {
    if (auto player = ll::service::bedrock::getLevel()->getPlayerFromServerId(serverId)) {
        return getScoreboardId(*player);
    }
    auto uniqueId = GMLIB_Player::getPlayerUniqueID(serverId);
    if (uniqueId.id != ActorUniqueID::INVALID_ID.id) {
        auto psid = PlayerScoreboardId(uniqueId.id);
        return getScoreboardId(psid);
    }
    return ScoreboardId::INVALID;
}

ScoreboardId GMLIB_Scoreboard::getPlayerScoreboardId(mce::UUID const& uuid) {
    auto serverId = GMLIB_Player::getServerIdFromUuid(uuid);
    return getPlayerScoreboardId(serverId);
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

std::optional<int> GMLIB_Scoreboard::getScore(std::string objective, ActorUniqueID uniqueId, bool isPlayer) {
    auto id = getScoreboardId(uniqueId);
    if (isPlayer) {
        auto psid = PlayerScoreboardId(uniqueId.id);
        id        = getScoreboardId(psid);
    }
    auto obj = getObjective(objective);
    return getScore(obj, id);
}

std::optional<int> GMLIB_Scoreboard::getPlayerScore(std::string objective, std::string serverId) {
    auto id  = getPlayerScoreboardId(serverId);
    auto obj = getObjective(objective);
    return getScore(obj, id);
}

std::optional<int> GMLIB_Scoreboard::getPlayerScore(std::string objective, mce::UUID const& uuid) {
    auto id  = getPlayerScoreboardId(uuid);
    auto obj = getObjective(objective);
    return getScore(obj, id);
}

std::optional<int> GMLIB_Scoreboard::getPlayerScore(std::string objective, ActorUniqueID uniqueId) {
    return getScore(objective, uniqueId, true);
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
    auto id = getScoreboardId(name);
    if (!id.isValid()) {
        id = getServerScoreboard()->createScoreboardId(name);
    }
    auto obj = getObjective(objective);
    if (!obj) {
        obj = addObjective(objective);
    }
    return setScore(obj, id, value, action);
}

std::optional<int>
GMLIB_Scoreboard::setScore(std::string objective, Player* pl, int value, PlayerScoreSetFunction action) {
    auto id = getScoreboardId(*pl);
    if (!id.isValid()) {
        id = getServerScoreboard()->createScoreboardId(*pl);
    }
    auto obj = getObjective(objective);
    if (!obj) {
        obj = addObjective(objective);
    }
    return setScore(obj, id, value, action);
}

std::optional<int>
GMLIB_Scoreboard::setScore(std::string objective, Actor* ac, int value, PlayerScoreSetFunction action) {
    auto id = getScoreboardId(*ac);
    if (!id.isValid()) {
        id = getServerScoreboard()->createScoreboardId(*ac);
    }
    auto obj = getObjective(objective);
    if (!obj) {
        obj = addObjective(objective);
    }
    return setScore(obj, id, value, action);
}

std::optional<int> GMLIB_Scoreboard::setScore(
    std::string            objective,
    ActorUniqueID          uniqueId,
    int                    value,
    PlayerScoreSetFunction action,
    bool                   isPlayer
) {
    auto id = getScoreboardId(uniqueId);
    if (isPlayer) {
        auto psid = PlayerScoreboardId(uniqueId.id);
        id        = getScoreboardId(psid);
        if (!id.isValid()) {
            auto fakeId = getServerScoreboard()->createScoreboardId(S(psid.mActorUniqueId));
            if (uniqueId.id != ActorUniqueID::INVALID_ID.id) {
                id = getIdentityDictionary()->convertFakeToReal(fakeId, psid);
            }
        }
    }
    auto obj = getObjective(objective);
    if (!obj) {
        obj = addObjective(objective);
    }
    return setScore(obj, id, value, action);
}

std::optional<int> GMLIB_Scoreboard::setPlayerScore(
    std::string            objective,
    std::string            serverId,
    int                    value,
    PlayerScoreSetFunction action
) {
    if (serverId.empty()) {
        return {};
    }
    auto id = getPlayerScoreboardId(serverId);
    if (!id.isValid()) {
        auto fakeId   = getServerScoreboard()->createScoreboardId(serverId);
        auto uniqueId = GMLIB_Player::getPlayerUniqueID(serverId);
        if (uniqueId.id != ActorUniqueID::INVALID_ID.id) {
            auto psid = PlayerScoreboardId(uniqueId.id);
            id        = getIdentityDictionary()->convertFakeToReal(fakeId, psid);
        }
    }
    auto obj = getObjective(objective);
    if (!obj) {
        obj = addObjective(objective);
    }
    return setScore(obj, id, value, action);
}

std::optional<int> GMLIB_Scoreboard::setPlayerScore(
    std::string            objective,
    mce::UUID const&       uuid,
    int                    value,
    PlayerScoreSetFunction action
) {
    auto serverId = GMLIB_Player::getServerIdFromUuid(uuid);
    return setPlayerScore(objective, serverId, value, action);
}

std::optional<int> GMLIB_Scoreboard::setPlayerScore(
    std::string            objective,
    ActorUniqueID          uniqueId,
    int                    value,
    PlayerScoreSetFunction action
) {
    return setScore(objective, uniqueId, value, action, true);
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

bool GMLIB_Scoreboard::resetScore(std::string objective, ActorUniqueID uniqueId, bool isPlayer) {
    auto obj = getObjective(objective);
    auto id  = getScoreboardId(uniqueId);
    if (isPlayer) {
        auto psid = PlayerScoreboardId(uniqueId.id);
        id        = getScoreboardId(psid);
    }
    return resetScore(obj, id);
}

bool GMLIB_Scoreboard::resetPlayerScore(std::string objective, std::string serverId) {
    auto id  = getPlayerScoreboardId(serverId);
    auto obj = getObjective(objective);
    return resetScore(obj, id);
}

bool GMLIB_Scoreboard::resetPlayerScore(std::string objective, mce::UUID const& uuid) {
    auto id  = getPlayerScoreboardId(uuid);
    auto obj = getObjective(objective);
    return resetScore(obj, id);
}

bool GMLIB_Scoreboard::resetPlayerScore(std::string objective, ActorUniqueID uniqueId) {
    return resetScore(objective, uniqueId, true);
}

bool GMLIB_Scoreboard::resetPlayerScore(std::string objective, Player* pl) { return resetScore(objective, pl); }

bool GMLIB_Scoreboard::resetScore(ScoreboardId& scoreboardId) {
    if (scoreboardId.isValid()) {
        resetPlayerScore(scoreboardId);
        return true;
    }
    return false;
}

bool GMLIB_Scoreboard::resetScore(std::string name) {
    auto id = getScoreboardId(name);
    return resetScore(id);
}

bool GMLIB_Scoreboard::resetScore(Player* pl) {
    auto id = getScoreboardId(*pl);
    return resetScore(id);
}

bool GMLIB_Scoreboard::resetScore(Actor* ac) {
    auto id = getScoreboardId(*ac);
    return resetScore(id);
}

bool GMLIB_Scoreboard::resetScore(ActorUniqueID uniqueId, bool isPlayer) {
    auto id = getScoreboardId(uniqueId);
    if (isPlayer) {
        auto psid = PlayerScoreboardId(uniqueId.id);
        id        = getScoreboardId(psid);
    }
    return resetScore(id);
}

bool GMLIB_Scoreboard::resetPlayerScore(std::string serverId) {
    auto id = getPlayerScoreboardId(serverId);
    return resetScore(id);
}

bool GMLIB_Scoreboard::resetPlayerScore(mce::UUID const& uuid) {
    auto id = getPlayerScoreboardId(uuid);
    return resetScore(id);
}

bool GMLIB_Scoreboard::resetPlayerScore(ActorUniqueID uniqueId) { return resetScore(uniqueId, true); }

bool GMLIB_Scoreboard::resetPlayerScore(Player* pl) { return resetScore(pl); }

std::vector<ScoreboardId> GMLIB_Scoreboard::getAllScoreboardIds() { return getTrackedIds(); }

std::vector<ScoreboardId> GMLIB_Scoreboard::getAllScoreboardIds(IdentityDefinition::Type type) {
    std::vector<ScoreboardId> ids = getTrackedIds();
    std::vector<ScoreboardId> result;
    for (auto& id : ids) {
        if (id.getIdentityDef().getIdentityType() == type) {
            result.push_back(id);
        }
    }
    return result;
}

std::vector<ScoreboardId> GMLIB_Scoreboard::getObjectiveTrackedScoreboardIds(Objective* objective) {
    std::vector<ScoreboardId> ids = getTrackedIds();
    std::vector<ScoreboardId> result;
    for (auto& id : ids) {
        if (getScore(objective, id).has_value()) {
            result.push_back(id);
        }
    }
    return result;
}

std::vector<ScoreboardId>
GMLIB_Scoreboard::getObjectiveTrackedScoreboardIds(Objective* objective, IdentityDefinition::Type type) {
    std::vector<ScoreboardId> ids = getTrackedIds();
    std::vector<ScoreboardId> result;
    for (auto& id : ids) {
        if (id.getIdentityDef().getIdentityType() == type) {
            if (getScore(objective, id).has_value()) {
                result.push_back(id);
            }
        }
    }
    return result;
}

std::optional<std::string> GMLIB_Scoreboard::getObjectiveDisplayName(std::string objective) {
    auto obj = getObjective(objective);
    if (obj) {
        return obj->getDisplayName();
    }
    return {};
}

bool GMLIB_Scoreboard::setObjectiveDisplayName(std::string objective, std::string newName) {
    auto obj = getObjective(objective);
    if (obj) {
        obj->mDisplayName = newName;
        return true;
    }
    return false;
}

void GMLIB_Scoreboard::setObjectiveDisplay(Objective* objctive, std::string displaySlot, ObjectiveSortOrder order) {
    setDisplayObjective(displaySlot, *objctive, order);
}

void GMLIB_Scoreboard::setObjectiveDisplay(std::string objective, std::string displaySlot, ObjectiveSortOrder order) {
    auto obj = getObjective(objective);
    setDisplayObjective(displaySlot, *obj, order);
}

void GMLIB_Scoreboard::clearObjectiveDisplay(std::string displaySlot) { clearDisplayObjective(displaySlot); }

std::vector<ActorUniqueID> GMLIB_Scoreboard::getAllPlayers() {
    std::vector<ScoreboardId>  sids = getAllScoreboardIds(IdentityDefinition::Type::Player);
    std::vector<ActorUniqueID> result;
    for (auto& sid : sids) {
        auto uniqueId = ActorUniqueID(sid.getIdentityDef().getPlayerId().mActorUniqueId);
        result.push_back(uniqueId);
    }
    return result;
}

std::vector<std::string> GMLIB_Scoreboard::getAllPlayerServerIds() {
    std::vector<ScoreboardId>              sids = getAllScoreboardIds(IdentityDefinition::Type::Player);
    std::vector<std::string>               result;
    std::unordered_map<int64, std::string> map = GMLIB_Player::getUniqueIdToServerIdMap();
    for (auto& sid : sids) {
        auto uniqueId = sid.getIdentityDef().getPlayerId().mActorUniqueId;
        if (map.count(uniqueId)) {
            result.push_back(map[uniqueId]);
        }
    }
    return result;
}

std::vector<mce::UUID> GMLIB_Scoreboard::getAllPlayerUuids() {
    std::vector<ScoreboardId>            sids = getAllScoreboardIds(IdentityDefinition::Type::Player);
    std::vector<mce::UUID>               result;
    std::unordered_map<int64, mce::UUID> map = GMLIB_Player::getUniqueIdToUuidMap();
    for (auto& sid : sids) {
        auto uniqueId = sid.getIdentityDef().getPlayerId().mActorUniqueId;
        if (map.count(uniqueId)) {
            result.push_back(map[uniqueId]);
        }
    }
    return result;
}

std::vector<ActorUniqueID> GMLIB_Scoreboard::getAllEntities() {
    std::vector<ScoreboardId>  sids = getAllScoreboardIds(IdentityDefinition::Type::Entity);
    std::vector<ActorUniqueID> result;
    for (auto& sid : sids) {
        auto uniqueId = sid.getIdentityDef().getEntityId();
        result.push_back(uniqueId);
    }
    return result;
}

std::vector<std::string> GMLIB_Scoreboard::getAllFakePlayers() {
    std::vector<ScoreboardId> sids = getAllScoreboardIds(IdentityDefinition::Type::FakePlayer);
    std::vector<std::string>  result;
    for (auto& sid : sids) {
        auto name = sid.getIdentityDef().getFakePlayerName();
        result.push_back(name);
    }
    return result;
}