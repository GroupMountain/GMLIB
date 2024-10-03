#pragma once
#include "GMLIB/Macros.h"
#include <mc/dataloadhelper/DataLoadHelper.h>
#include <mc/nbt/CompoundTag.h>
#include <mc/world/actor/Actor.h>
#include <mc/world/actor/player/Player.h>


class GMLIB_CompoundTag : public CompoundTag {
public:
    GMLIB_CompoundTag() = default;

    GMLIB_CompoundTag(TagMap tags) : CompoundTag(tags) {}

    GMLIB_CompoundTag(std::initializer_list<TagMap::value_type> tags) : CompoundTag(tags) {}

    GMLIB_CompoundTag(GMLIB_CompoundTag const&)            = default;
    GMLIB_CompoundTag& operator=(GMLIB_CompoundTag const&) = default;
    GMLIB_CompoundTag(GMLIB_CompoundTag&&)                 = default;
    GMLIB_CompoundTag& operator=(GMLIB_CompoundTag&&)      = default;

public:
    GMLIB_NDAPI static std::unique_ptr<DataLoadHelper> getDataLoadHelper();

    GMLIB_NDAPI static std::unique_ptr<CompoundTag> getFromActor(Actor& ac);

    GMLIB_NDAPI static std::unique_ptr<CompoundTag> getFromPlayer(Player& pl);

    GMLIB_NDAPI static std::unique_ptr<CompoundTag> getFromBlockActor(BlockActor& blac);

    GMLIB_NDAPI static std::unique_ptr<CompoundTag> getFromItemStack(ItemStack& item);

    GMLIB_NDAPI static std::unique_ptr<CompoundTag> getFromBlock(Block& block);

    GMLIB_NDAPI static std::optional<CompoundTag> readFromFile(std::filesystem::path const& path, bool isBinary = true);

    GMLIB_API static bool saveToFile(std::filesystem::path const& path, CompoundTag& nbt, bool isBinary = true);

    GMLIB_API static void
    writeNbtTags(CompoundTag& originNbt, CompoundTag& dataNbt, const std::vector<std::string>& tags);

public:
    GMLIB_API bool setToActor(Actor& ac);

    GMLIB_API bool setToPlayer(Player& pl);

    GMLIB_API void setToBlockActor(BlockActor& blac);

    GMLIB_API void setToItemStack(ItemStack& item);

    GMLIB_API void setToBlock(Block& block);
};