#pragma once
#include "GMLIB/GMLIB.h"

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

class GMLIB_CustomBiome {
public:
    GMLIB_API static void registerBiomeClimates(std::string id, BiomeData data);

    GMLIB_API static void registerEmptyBiome(std::string id, BiomeData data) ;
};