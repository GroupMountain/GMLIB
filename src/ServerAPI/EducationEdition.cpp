/**
 * @file EducationEdition.cpp
 * @author Tsubasa6848
 * @brief 教育版相关API
 * @version 0.1
 * @date 2023-09-30
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#include "../GlobalHeader.h"
#include <GMLib/GMLib_ServerAPI.h>
#include <mc/AbilityCommand.hpp>
#include <mc/LevelData.hpp>

bool EduFeatureInited = false;
bool EnableEducation = false;
bool RegAbilityCmd = false;

void GMLib_Server::setEducationFeatureEnabled(bool value) {
    EduFeatureInited = true;
    EnableEducation = value;
}

void GMLib_Server::registerAbilityCommand() {
    RegAbilityCmd = true;
}

// 开启Edu
LL_AUTO_TYPED_INSTANCE_HOOK(
    EduInit,
    LevelData,
    HookPriority::Normal, 
    "?educationFeaturesEnabled@LevelData@@QEBA_NXZ", 
    bool)
{
    auto res = origin();
    if (!CanLoad) {
        return res;
    }
    if (EduFeatureInited) {
        this->setEducationFeaturesEnabled(EnableEducation);
        return EnableEducation;
    }
    EnableEducation = res;
    return res;
}

// 注册ability
LL_AUTO_INSTANCE_HOOK(
    RegAbility,
    HookPriority::Normal, 
    "?setup@ChangeSettingCommand@@SAXAEAVCommandRegistry@@@Z", 
    void,
    void* a1)
{
    if (!CanLoad) {
        return origin(a1);
    }
    if (EnableEducation == false && RegAbilityCmd == true) {
        AbilityCommand::setup(*(CommandRegistry*)this);
    }
    return origin(a1);
}