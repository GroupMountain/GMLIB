#pragma once
#include "Macros.h"
#include "mc/deps/core/threading/Scheduler.h"
#include "mc/world/level/dimension/Dimension.h"
#include "mc/world/level/levelgen/GeneratorType.h"


namespace gmlib::mod::dimension {

struct DimensionFactoryInfo {
    ::ILevel&            mLevel;
    ::Scheduler&         mScheduler;
    ::CompoundTag const& mData;
    DimensionType        mDimId;
};

using DimensionFactoryT = std::shared_ptr<::Dimension>(DimensionFactoryInfo const&);

class DimensionManager {
protected:
    GMLIB_NDAPI std::optional<DimensionType> addDimension(
        std::string const&                      dimName,
        std::function<DimensionFactoryT> const& factory,
        std::function<::CompoundTag()> const&   data
    );

public:
    GMLIB_NDAPI static DimensionManager& getInstance();

    GMLIB_NDAPI static DimensionType getDimensionIdFromName(std::string const& dimName);

    static void disableClientChunkGeneration();

public:
    GMLIB_API bool isDimensionLoaded(DimensionType dimId);
    GMLIB_API bool isDimensionLoaded(std::string const& dimName);

    template <std::derived_from<::Dimension> T>
    std::optional<DimensionType>
    addDimension(std::string const& dimName, std::function<::CompoundTag()> const& data = {}) {
        return addDimension(
            dimName,
            [dimName](DimensionFactoryInfo const& info) -> std::shared_ptr<::Dimension> {
                return std::make_shared<T>(dimName, info);
            },
            std::move(data)
        );
    }

    GMLIB_API std::optional<DimensionType>
    addSimpleDimension(std::string const& dimName, GeneratorType generatorType, uint seed, bool hasWeather = false);

    GMLIB_API std::optional<DimensionType> addSimpleDimension(
        std::string const& dimName,
        GeneratorType      generatorType = GeneratorType::Overworld,
        bool               hasWeather    = false
    );

    GMLIB_API bool removeDimension(DimensionType dimId);

    GMLIB_API bool exportDimension(
        DimensionType                fromDimId,
        DimensionType                toDimId,
        std::filesystem::path const& path,
        bool                         removeDimension  = false,
        bool                         replaceDirectory = true
    );

    GMLIB_API bool exportDimension(
        DimensionType                fromDimId,
        std::filesystem::path const& path,
        bool                         removeDimension  = false,
        bool                         replaceDirectory = true
    );

    GMLIB_API bool importDimension( // Broken ?
        DimensionType                toDimId,
        std::filesystem::path const& path,
        DimensionType                fromDimId,
        bool                         replaceOldChunk = true,
        bool                         removeDirectory = false
    );

    GMLIB_API bool importDimension( // Broken ?
        DimensionType                toDimId,
        std::filesystem::path const& path,
        bool                         replaceOldChunk = true,
        bool                         removeDirectory = false
    );
};

} // namespace gmlib::mod::dimension
