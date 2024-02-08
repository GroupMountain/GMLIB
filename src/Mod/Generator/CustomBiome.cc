#include "Global.h"
#include <GMLIB/Mod/CustomBiome.h>
#include <GMLIB/Server/LevelAPI.h>
#include <utility>

namespace GMLIB::Mod {

std::unordered_map<std::string, std::vector<BiomeData>> mClimates;
std::vector<std::string>                                mNewBiomes;
bool                                                    mCustomBiomeEnabled = false;

void CustomBiome::registerBiomeClimates(std::string id, BiomeData data) {
    mClimates[id].push_back(data);
    GMLIB_Level::addExperimentsRequire((AllExperiments)7);
    if (!mCustomBiomeEnabled) {
        ll::memory::HookRegistrar<OverworldBiomeBuilderHook>().hook();
        ll::memory::HookRegistrar<BiomeRegistryHook>().hook();
        mCustomBiomeEnabled = true;
    }
}

// 6 | ExperimentalHoliday
// 7 | ExperimentalBiomes
// 8 | ExperimentalCreatorFeatures
// 9 | ExperimentalGameTest
// 10 | ExperimentalMolangFeatures
// 12 | ExperimentalCameras
// 15 | ExperimentalVillagerTradesRebalance
// 16 | ExperimentalUpdateAnnounced2023

void CustomBiome::registerEmptyBiome(std::string id, BiomeData data) {
    mNewBiomes.push_back(id);
    CustomBiome::registerBiomeClimates(id, data);
}

LL_TYPE_INSTANCE_HOOK(
    OverworldBiomeBuilderHook,
    HookPriority::Highest,
    OverworldBiomeBuilder,
    "?addBiomes@OverworldBiomeBuilder@@QEBAXAEAV?$vector@UBiomeNoiseTarget@@V?$"
    "allocator@UBiomeNoiseTarget@@@std@@@std@@AEBVBiomeRegistry@@@Z",
    void,
    std::vector<BiomeNoiseTarget>* a1,
    BiomeRegistry*                 a2
) {
    origin(a1, a2);
    if (mCustomBiomeEnabled) {
        for (auto& [name, climates] : mClimates) {
            for (auto& data : climates) {
                if (data.mType == BiomeType::Surface) {
                    this->_addSurfaceBiome(
                        *a1,
                        data.mTemperatureRange,
                        data.mHumidityRange,
                        data.mContinentalnessRange,
                        data.mErosionRange,
                        data.mWeirdnessRange,
                        data.mOffset,
                        a2->lookupByName(name)
                    );
                } else if (data.mType == BiomeType::Underground) {
                    this->_addUndergroundBiome(
                        *a1,
                        data.mTemperatureRange,
                        data.mHumidityRange,
                        data.mContinentalnessRange,
                        data.mErosionRange,
                        data.mWeirdnessRange,
                        data.mOffset,
                        a2->lookupByName(name)
                    );
                }
                std::unreachable();
            }
        }
        mClimates.clear();
    }
}

LL_STATIC_HOOK(
    BiomeRegistryHook,
    HookPriority::Highest,
    "?initBiomes@VanillaBiomes@@SAXAEAVBiomeRegistry@@AEBUSpawnSettings@"
    "@AEBVBaseGameVersion@@AEBVExperiments@@@Z",
    void,
    BiomeRegistry* a1,
    void*          a2,
    void*          a3,
    void*          a4
) {
    if (mCustomBiomeEnabled) {
        for (auto& name : mNewBiomes) {
            a1->registerBiome(name);
        }
    }
    return origin(a1, a2, a3, a4);
}

LL_AUTO_INSTANCE_HOOK(ClientGen, HookPriority::Highest, "?isClientSideGenEnabled@PropertiesSettings@@QEBA_NXZ", bool) {
    if (mCustomBiomeEnabled) {
        return false;
    }
    return origin();
}

} // namespace GMLIB::Mod
