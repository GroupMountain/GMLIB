#include "Global.h"
#include <GMLIB/Mod/CustomGenerator/CustomBiome.h>
#include <GMLIB/Server/LevelAPI.h>
#include <mc/world/level/biome/registry/BiomeRegistry.h>
#include <mc/world/level/levelgen/v1/OverworldBiomeBuilder.h>

namespace GMLIB::Mod {

phmap::flat_hash_map<std::string, std::vector<BiomeData>> mClimates;
std::vector<std::string>                                  mNewBiomes;

// 6 | Experimentalholiday
// 7 | Experimentalbiomes
// 8 | ExperimentalCreatorFeatures
// 9 | ExperimentalGameTest
// 10 | ExperimentalMolangFeatures
// 13 | ExperimentalCameras
// 16 | ExperimentalVillagerTradesRebalance
// 17 | ExperimentalUpdateAnnounced2023
// 18 | ExperimentalArmadillo

void CustomBiome::registerEmptyBiome(std::string const& id, BiomeData data) {
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
    for (auto& name : mNewBiomes) {
        a1->registerBiome(name);
    }
    return origin(a1, a2, a3, a4);
}

LL_INSTANCE_HOOK(ClientGenHook, HookPriority::Highest, "?isClientSideGenEnabled@PropertiesSettings@@QEBA_NXZ", bool) {
    return false;
}

struct CustomBiome_Impl {
    ll::memory::HookRegistrar<OverworldBiomeBuilderHook, BiomeRegistryHook, ClientGenHook> r;
};

std::unique_ptr<CustomBiome_Impl> impl;

void CustomBiome::registerBiomeClimates(std::string const& id, BiomeData data) {
    mClimates[id].push_back(data);
    GMLIB_Level::addExperimentsRequire((AllExperiments)7);
    if (!impl) {
        impl = std::make_unique<CustomBiome_Impl>();
    }
}

} // namespace GMLIB::Mod
