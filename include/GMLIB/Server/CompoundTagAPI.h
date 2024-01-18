#pragma once
#include "GMLIB/GMLIB.h"

class GMLIB_CompoundTag : public CompoundTag {
public:
    GMLIB_API static DataLoadHelper* getDataLoadHelper();

    GMLIB_API static std::unique_ptr<CompoundTag> createCompoundTag();

    GMLIB_API static std::unique_ptr<CompoundTag> getFromActor(Actor* ac);

    GMLIB_API static std::unique_ptr<CompoundTag> getFromPlayer(Player* pl);

    GMLIB_API static std::unique_ptr<CompoundTag> getFromBlockActor(BlockActor* blac);

    GMLIB_API static std::unique_ptr<CompoundTag> getFromItemStack(ItemStack* item);

    GMLIB_API static std::unique_ptr<CompoundTag> getFromBlock(Block* block);

public:
    GMLIB_API bool setToActor(Actor* ac, CompoundTag* nbt);

    GMLIB_API bool setToPlayer(Player* pl, CompoundTag* nbt);

    GMLIB_API void setToBlockActor(BlockActor* blac, CompoundTag* nbt);

    GMLIB_API void setToItemStack(ItemStack* item, CompoundTag* nbt);

    GMLIB_API void setToBlock(Block* block, CompoundTag* nbt);
};