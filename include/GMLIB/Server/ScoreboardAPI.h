#pragma once
#include "GMLIB/GMLIB.h"
#include "mc/world/scores/Scoreboard.h"

class GMLIB_Scoreboard : public Scoreboard {
public:
    using Scoreboard::addObjective;
    using Scoreboard::removeObjective;
    using Scoreboard::resetPlayerScore;

public:
    GMLIB_API static GMLIB_Scoreboard* getServerScoreboard();

    GMLIB_API static std::optional<std::string> getObjectiveDisplayName(std::string objective);

    GMLIB_API static bool setObjectiveDisplayName(std::string objective, std::string newName);

public:
    GMLIB_API Objective* addObjective(std::string name);

    GMLIB_API Objective* addObjective(std::string name, std::string displayName);

    GMLIB_API bool removeObjective(std::string objective);

    GMLIB_API ScoreboardId getPlayerScoreboardId(std::string serverid);

    GMLIB_API ScoreboardId getPlayerScoreboardId(mce::UUID& uuid);

    GMLIB_API std::optional<int> getScore(Objective* objective, ScoreboardId& scoreboardId);

    GMLIB_API std::optional<int> getScore(std::string objective, std::string name);

    GMLIB_API std::optional<int> getScore(std::string objective, Player* pl);

    GMLIB_API std::optional<int> getScore(std::string objective, Actor* ac);

    GMLIB_API std::optional<int> getPlayerScore(std::string objective, std::string serverid);

    GMLIB_API std::optional<int> getPlayerScore(std::string objective, mce::UUID& uuid);

    GMLIB_API std::optional<int> getPlayerScore(std::string objective, Player* pl);

    GMLIB_API std::optional<int> setScore(
        Objective*             objective,
        ScoreboardId&          scoreboardId,
        int                    value,
        PlayerScoreSetFunction action = PlayerScoreSetFunction::Set
    );

    GMLIB_API std::optional<int> setScore(
        std::string            objective,
        std::string            name,
        int                    value,
        PlayerScoreSetFunction action = PlayerScoreSetFunction::Set
    );

    GMLIB_API std::optional<int>
    setScore(std::string objective, Player* pl, int value, PlayerScoreSetFunction action = PlayerScoreSetFunction::Set);

    GMLIB_API std::optional<int>
    setScore(std::string objective, Actor* ac, int value, PlayerScoreSetFunction action = PlayerScoreSetFunction::Set);

    GMLIB_API std::optional<int> setPlayerScore(
        std::string            objective,
        std::string            serverid,
        int                    value,
        PlayerScoreSetFunction action = PlayerScoreSetFunction::Set
    );

    GMLIB_API std::optional<int> setPlayerScore(
        std::string            objective,
        mce::UUID&             uuid,
        int                    value,
        PlayerScoreSetFunction action = PlayerScoreSetFunction::Set
    );

    GMLIB_API std::optional<int> setPlayerScore(
        std::string            objective,
        Player*                pl,
        int                    value,
        PlayerScoreSetFunction action = PlayerScoreSetFunction::Set
    );

    GMLIB_API bool resetScore(Objective* objective, ScoreboardId& scoreboardId);

    GMLIB_API bool resetScore(std::string objective, std::string name);

    GMLIB_API bool resetScore(std::string objective, Player* pl);

    GMLIB_API bool resetScore(std::string objective, Actor* ac);

    GMLIB_API bool resetPlayerScore(std::string objective, std::string serverid);

    GMLIB_API bool resetPlayerScore(std::string objective, mce::UUID& uuid);

    GMLIB_API bool resetPlayerScore(std::string objective, Player* pl);

    GMLIB_API bool resetAllScores(ScoreboardId& scoreboardId);

    GMLIB_API bool resetAllScores(std::string name);

    GMLIB_API bool resetAllScores(Player* pl);

    GMLIB_API bool resetAllScores(Actor* ac);

    GMLIB_API bool resetPlayerAllScores(std::string serverid);

    GMLIB_API bool resetPlayerAllScores(mce::UUID& uuid);

    GMLIB_API bool resetPlayerAllScores(Player* pl);

    GMLIB_API std::vector<ScoreboardId> getAllScoreboardIds();
};