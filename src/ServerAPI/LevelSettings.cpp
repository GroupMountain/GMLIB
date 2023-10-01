/**
 * @file LevelSettings.cpp
 * @author Tsubasa6848
 * @brief LevelSetting 相关API
 * 服务器Level设置
 * 
 * @version 0.1
 * @date 2023-09-30
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#include "../GlobalHeader.h"
#include <GMLib/GMLib_ServerAPI.h>
#include <mc/LevelData.hpp>
#include <mc/SerializedSkin.hpp>

bool EnableAchievement = false;
bool TrustSkins = false;
bool CoResourcePack = false;
bool EnableFakeSeed = false;
int64_t FakeSeed = -1;

void GMLib_Server::setEnableAchievement() {
    EnableAchievement = true;
}

void GMLib_Server::setForceTrustSkins() {
    TrustSkins = true;
}

void GMLib_Server::enableCoResourcePack() {
    CoResourcePack = true;
}

void GMLib_Server::setFakeSeed(int64_t seed) {
    EnableFakeSeed = true;
    FakeSeed = seed;
}

std::string GMLib_Server::getLevelName() {
    return Global<Level>->getLevelData().getLevelName();
}

void GMLib_Server::setLevelName(std::string name) {
    Global<Level>->getLevelData().setLevelName(name);
}

LL_AUTO_INSTANCE_HOOK(
    Achieve1,
    HookPriority::Normal, 
    "?achievementsWillBeDisabledOnLoad@LevelData@@QEBA_NXZ", 
    bool)
{
    if (!CanLoad) {
        return origin();
    }
    if (EnableAchievement) {
        return false;
    }
    return origin();
}

LL_AUTO_INSTANCE_HOOK(
    Achieve2,
    HookPriority::Normal, 
    "?hasAchievementsDisabled@LevelData@@QEBA_NXZ", 
    bool)
{
    if (!CanLoad) {
        return origin();
    }
    if (EnableAchievement) {
        return false;
    }
    return origin();
}

LL_AUTO_INSTANCE_HOOK(
    AllowCheatsSetting,
    HookPriority::Normal, 
    "?allowCheats@PropertiesSettings@@QEBA_NXZ", 
    bool)
{
    if (!CanLoad) {
        return origin();
    }
    if (EnableAchievement) {
        return false;
    }
    return origin();
}

LL_AUTO_INSTANCE_HOOK(
    isTrustSkin,
    HookPriority::Normal, 
    "?isTrustedSkin@SerializedSkin@@QEBA_NXZ", 
    bool)
{
    if (!CanLoad) {
        return origin();
    }
    if (TrustSkins) {
        return true;
    }
    return origin();
}

LL_AUTO_INSTANCE_HOOK(
    SendResource,
    HookPriority::Normal, 
    "?write@ResourcePacksInfoPacket@@UEBAXAEAVBinaryStream@@@Z", 
    void,
    void* a1)
{
    if (!CanLoad) {
        return origin(a1);
    }
    if (CoResourcePack) {
        dAccess<bool, 48>(this) = true;
    }
    return origin(a1);
}

LL_AUTO_INSTANCE_HOOK(
    ResourcePackRequire,
    HookPriority::Normal, 
    "?texturePackRequired@PropertiesSettings@@QEBA_NXZ", 
    bool)
{
    if (!CanLoad) {
        return origin();
    }
    if (CoResourcePack) {
        return false;
    }
    return origin();
}

LL_AUTO_INSTANCE_HOOK(
    StartGamePacket,
    HookPriority::Normal, 
    "?write@StartGamePacket@@UEBAXAEAVBinaryStream@@@Z", 
    void,
    void* a1)
{
    if (!CanLoad) {
        return origin(a1);
    }
    if (EnableFakeSeed) {
        dAccess<int64_t, 48>(this) = FakeSeed;
    }
    return origin(a1);
}