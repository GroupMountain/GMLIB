#include <GMLib/GMLib_ServerAPI.h>
#include <RemoteCallAPI.h>

std::unordered_set<int> ExperimentsList = {6, 7, 8, 9, 10, 12, 15};

void ExportGMLib_ModAPI() {
    //
}

void ExportGMLib_ServerAPI() {
    RemoteCall::exportAs("GMLib_ServerAPI", "setEducationFeatureEnabled", [](bool value) -> void { 
        GMLib_Server::setEducationFeatureEnabled(value);
    });
    RemoteCall::exportAs("GMLib_ServerAPI", "registerAbilityCommand", []() -> void { 
        GMLib_Server::registerAbilityCommand();
    });
    RemoteCall::exportAs("GMLib_ServerAPI", "sendAddFloatingTextPacket", [](std::string text, Vec3 pos, int dimid) -> int { 
        return GMLib_Server::sendAddFloatingTextPacket(text, pos, dimid);
    });
    RemoteCall::exportAs("GMLib_ServerAPI", "sendAddFloatingTextPacketToPlayer", [](std::string text, Vec3 pos, int dimid, Player* pl) -> int { 
        return GMLib_Server::sendAddFloatingTextPacketToPlayer(text, pos, dimid, (Player*)pl);
    });
    RemoteCall::exportAs("GMLib_ServerAPI", "sendDeleteFloatingTextPacket", [](int id) -> void { 
        GMLib_Server::sendDeleteFloatingTextPacket(id);
    });
    RemoteCall::exportAs("GMLib_ServerAPI", "setEnableAchievement", []() -> void { 
        GMLib_Server::setEnableAchievement();
    });
    RemoteCall::exportAs("GMLib_ServerAPI", "setForceTrustSkins", []() -> void { 
        GMLib_Server::setForceTrustSkins();
    });
    RemoteCall::exportAs("GMLib_ServerAPI", "enableCoResourcePack", []() -> void { 
        GMLib_Server::enableCoResourcePack();
    });
    RemoteCall::exportAs("GMLib_ServerAPI", "getLevelName", []() -> std::string { 
        return GMLib_Server::getLevelName();
    });
    RemoteCall::exportAs("GMLib_ServerAPI", "setLevelName", [](std::string name) -> void { 
        GMLib_Server::setLevelName(name);
    });
    RemoteCall::exportAs("GMLib_ServerAPI", "setFakeSeed", [](int64_t seed) -> void { 
        GMLib_Server::setFakeSeed(seed);
    });
    RemoteCall::exportAs("GMLib_ServerAPI", "spawnEntity", [](Vec3 pos, int dimid, std::string name) -> Actor* { 
        return GMLib_Server::spawnActor(pos, dimid, name);
    });
    RemoteCall::exportAs("GMLib_ServerAPI", "shootProjectile", [](Actor* owner, std::string name, float speed, float offset) -> Actor* { 
        return GMLib_Server::shootProjectile((Actor*)owner, name, speed, offset);
    });
    RemoteCall::exportAs("GMLib_ServerAPI", "throwEntity", [](Actor* owner, Actor* actor, float speed, float offset) -> bool { 
        return GMLib_Server::throwEntity((Actor*)owner, (Actor*)actor, speed, offset);
    });
    RemoteCall::exportAs("GMLib_ServerAPI", "PlayerToEntity", [](Player* player) -> Actor* { 
        return (Actor*)player;
    });
}

void ExportAPI() {
    ExportGMLib_ModAPI();
    ExportGMLib_ServerAPI();
}