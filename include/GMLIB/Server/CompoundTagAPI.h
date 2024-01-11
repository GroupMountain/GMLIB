#pragma once
#include "GMLIB/GMLIB.h"

class GMLIB_CompoundTag : public CompoundTag {
public:
    GMLIB_API static DataLoadHelper* getDataLoadHelper();

    GMLIB_API static std::unique_ptr<CompoundTag> createCompoundTag();

    GMLIB_API static std::unique_ptr<CompoundTag> getNbt(Actor* ac);

    GMLIB_API static std::unique_ptr<CompoundTag> getNbt(Player* pl);

    GMLIB_API static std::unique_ptr<CompoundTag> getNbt(BlockActor* blac);

    GMLIB_API static std::unique_ptr<CompoundTag> getNbt(ItemStack* item);

    GMLIB_API static bool setNbt(Actor* ac, CompoundTag* nbt);

    GMLIB_API static bool setNbt(Player* pl, CompoundTag* nbt);

    GMLIB_API static void setNbt(BlockActor* blac, CompoundTag* nbt);

    GMLIB_API static void setNbt(ItemStack* item, CompoundTag* nbt);
};