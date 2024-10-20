#pragma once
#include "Macros.h"
#include "mc/util/Random.h"
#include "mc/world/level/BlockPos.h"
#include "mc/world/level/BlockSource.h"
#include "mc/world/level/levelgen/feature/helpers/RenderParams.h"
#include "nlohmann/json.hpp"

namespace gmlib::mod::generator {

class ICustomFeature {
public:
    virtual ~ICustomFeature() = default;

    virtual std::optional<BlockPos>
    place(class BlockSource& source, class BlockPos const& pos, class Random& random, class RenderParams& renderParams)
        const = 0;
};

class FeatureRegistry {
public:
    GMLIB_API static void registerFeature(std::string const& name, std::shared_ptr<ICustomFeature> feature);

    GMLIB_API static void registerFeatureRule(nlohmann::json const& json);
};

} // namespace gmlib::mod::generator