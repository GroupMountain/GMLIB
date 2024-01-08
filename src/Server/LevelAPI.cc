#include "GMLIB/Server/LevelAPI.h"
#include "Global.h"

namespace GMLIB::LevelAPI {

bool                          mFakeLevelNameEnabled     = false;
std::string                   mFakeLevelName            = "";
bool                          mFakeSeedEnabled          = false;
int64_t                       mFakeSeed                 = 0;
bool                          mForceTrustSkin           = false;
bool                          mCoResourcePack           = false;
std::vector<::AllExperiments> mExperimentsRequireList   = {};
bool                          mForceAchievementsEnabled = false;
bool                          mRegAbilityCommand        = false;
bool                          mEducationEditionEnabled  = false;

BlockSource* getBlockSource(DimensionType dimid) {
    return &ll::service::getLevel()->getDimension(dimid)->getBlockSourceFromMainChunkSource();
}

std::vector<Actor*> getAllEntities() { return ll::service::getLevel()->getRuntimeActorList(); }

std::string getLevelName() { return ll::service::getLevel()->getLevelData().getLevelName(); }

void setLevelName(std::string newName) { ll::service::getLevel()->getLevelData().setLevelName(newName); }

void setFakeLevelName(std::string fakeName) {
    mFakeLevelNameEnabled = true;
    mFakeLevelName        = fakeName;
}

int64_t getSeed() { return ll::service::getLevel()->getLevelData().getSeed().mValue; }

void setFakeSeed(int64_t fakeSeed) {
    mFakeSeedEnabled = true;
    mFakeSeed        = fakeSeed;
}

void setCoResourcePack(bool enabled) { mCoResourcePack = enabled; }

void setForceTrustSkin(bool enabled) { mForceTrustSkin = enabled; }

bool getExperimentEnabled(::AllExperiments experiment, bool enabled) {
    return ll::service::getLevel()->getLevelData().getExperiments().isExperimentEnabled(experiment);
}

void setExperimentEnabled(::AllExperiments experiment, bool enabled) {
    ll::service::getLevel()->getLevelData().getExperiments().setExperimentEnabled(experiment, enabled);
}

void addExperimentsRequire(::AllExperiments experiment) {
    if (std::find(mExperimentsRequireList.begin(), mExperimentsRequireList.end(), experiment)
        == mExperimentsRequireList.end()) {
        mExperimentsRequireList.push_back(experiment);
    }
}

void setForceAchievementsEnabled() { mForceAchievementsEnabled = true; }

void forceEnableAbilityCommand() { mRegAbilityCommand = true; }

void addEducationEditionRequired() { mEducationEditionEnabled = true; }

} // namespace GMLIB::LevelAPI

LL_AUTO_INSTANCE_HOOK(
    Achieve1,
    ll::memory::HookPriority::Normal,
    "?achievementsWillBeDisabledOnLoad@LevelData@@QEBA_NXZ",
    bool
) {
    if (GMLIB::LevelAPI::mForceAchievementsEnabled) {
        return false;
    }
    return origin();
}

LL_AUTO_INSTANCE_HOOK(
    Achieve2,
    ll::memory::HookPriority::Normal,
    "?hasAchievementsDisabled@LevelData@@QEBA_NXZ",
    bool
) {
    if (GMLIB::LevelAPI::mForceAchievementsEnabled) {
        return false;
    }
    return origin();
}

LL_AUTO_INSTANCE_HOOK(
    AllowCheatsSetting,
    ll::memory::HookPriority::Normal,
    "?allowCheats@PropertiesSettings@@QEBA_NXZ",
    bool
) {
    if (GMLIB::LevelAPI::mForceAchievementsEnabled) {
        return false;
    }
    return origin();
}

void initExperiments(LevelData* leveldat) {
    if (GMLIB::LevelAPI::mExperimentsRequireList.size() >= 1) {
        for (auto exp : GMLIB::LevelAPI::mExperimentsRequireList) {
            leveldat->getExperiments().setExperimentEnabled(exp, true);
        }
    }
}

LL_AUTO_INSTANCE_HOOK(
    LevelDatInit,
    ll::memory::HookPriority::Normal,
    "?getLevelData@ExternalFileLevelStorageSource@@UEBA?AVLevelData@@AEBV?$basic_string@DU?$char_traits@D@std@@V?$"
    "allocator@D@2@@std@@@Z",
    LevelData*,
    std::string& a1
) {
    auto res = origin(a1);
    initExperiments(res);
    return res;
}

LL_AUTO_INSTANCE_HOOK(isTrustSkin, ll::memory::HookPriority::Normal, "?isTrustedSkin@SerializedSkin@@QEBA_NXZ", bool) {
    if (GMLIB::LevelAPI::mForceTrustSkin) {
        return true;
    }
    return origin();
}

LL_AUTO_TYPED_INSTANCE_HOOK(
    ResourcePacksInfoPacketWrite,
    ll::memory::HookPriority::Normal,
    ResourcePacksInfoPacket,
    "?write@ResourcePacksInfoPacket@@UEBAXAEAVBinaryStream@@@Z",
    void,
    class BinaryStream& stream
) {
    if (GMLIB::LevelAPI::mCoResourcePack) {
        this->mData.mResourcePackRequired    = true;
        this->mData.mForceServerPacksEnabled = false;
    }
    return origin(stream);
}

LL_AUTO_TYPED_INSTANCE_HOOK(
    StartGamePacketWrite,
    ll::memory::HookPriority::Normal,
    StartGamePacket,
    "?write@StartGamePacket@@UEBAXAEAVBinaryStream@@@Z",
    void,
    class BinaryStream& stream
) {
    if (GMLIB::LevelAPI::mFakeSeedEnabled) {
        this->mSettings.mSeed.mValue = GMLIB::LevelAPI::mFakeSeed;
    }
    if (GMLIB::LevelAPI::mFakeLevelNameEnabled) {
        this->mLevelName = GMLIB::LevelAPI::mFakeLevelName;
    }
    return origin(stream);
}

LL_AUTO_TYPED_INSTANCE_HOOK(
    EduInit,
    ll::memory::HookPriority::Normal,
    LevelData,
    "?educationFeaturesEnabled@LevelData@@QEBA_NXZ",
    bool
) {
    auto res = origin();
    if (GMLIB::LevelAPI::mEducationEditionEnabled) {
        this->setEducationFeaturesEnabled(true);
        return true;
    }
    GMLIB::LevelAPI::mEducationEditionEnabled = res;
    return res;
}

LL_AUTO_TYPED_INSTANCE_HOOK(
    RegAbility,
    ll::memory::HookPriority::Normal,
    ChangeSettingCommand,
    "?setup@ChangeSettingCommand@@SAXAEAVCommandRegistry@@@Z",
    void,
    class CommandRegistry& registry
) {
    if (GMLIB::LevelAPI::mEducationEditionEnabled == false && GMLIB::LevelAPI::mRegAbilityCommand == true) {
        AbilityCommand::setup(registry);
    }
    return origin(registry);
}