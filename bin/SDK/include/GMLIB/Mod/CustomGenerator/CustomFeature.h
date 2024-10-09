#pragma once
#include "GMLIB/Macros.h"
#include "mc/util/Random.h"
#include "mc/world/level/BlockPos.h"
#include "mc/world/level/BlockSource.h"
#include "mc/world/level/levelgen/feature/helpers/RenderParams.h"
#include "nlohmann/json.hpp"

namespace GMLIB::Mod {

class GMLIB_Feature {
public:
    virtual ~GMLIB_Feature();
    virtual std::optional<BlockPos>
    place(class BlockSource& source, class BlockPos const& pos, class Random& random, class RenderParams& renderParams)
        const = 0;
};
class CustomFeature {
public:
    GMLIB_API static void registerFeature(std::string const& name, std::shared_ptr<GMLIB_Feature> feature);
    GMLIB_API static void registerFeatureRule(nlohmann::json const& json);
};

} // namespace GMLIB::Mod