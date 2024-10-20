#pragma once
#include "Macros.h"
#include "mc/world/level/biome/Parameter.h"

namespace gmlib::mod::generator {

enum class BiomeType {
    Surface     = 0, // Surface Biome
    Underground = 1  // Underground Biome
};

struct BiomeData {
    BiomeType               mType;
    ClimateUtils::Parameter mTemperatureRange;
    ClimateUtils::Parameter mHumidityRange;
    ClimateUtils::Parameter mContinentalnessRange;
    ClimateUtils::Parameter mErosionRange;
    ClimateUtils::Parameter mWeirdnessRange;
    float                   mOffset;
};

class BiomeRegistry {
public:
    GMLIB_API static void registerBiomeClimates(std::string const& id, BiomeData data);

    GMLIB_API static void registerEmptyBiome(std::string const& id, BiomeData data);
};

} // namespace gmlib::mod::generator