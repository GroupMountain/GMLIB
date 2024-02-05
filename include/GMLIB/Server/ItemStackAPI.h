#pragma once
#include "GMLIB/GMLIB.h"
#include "mc/world/item/components/ItemLockMode.h"
#include "mc/world/item/registry/ItemStack.h"

class GMLIB_ItemStack : public ItemStack {
public:
    GMLIB_API std::unique_ptr<CompoundTag> getNbt();

    GMLIB_API void setNbt(CompoundTag& nbt);

    GMLIB_API void setItem(std::string name, int count = 1, short auxValue = 0);

    GMLIB_API void setItem(ItemStack* item);

    GMLIB_API void setCount(int value);

    GMLIB_API void addCount(int value);

    GMLIB_API void removeCount(int value);

    GMLIB_API void setAuxValue(short auxValue);

    GMLIB_API std::vector<const BlockLegacy*> getCanDestroy();

    GMLIB_API void setCanDestroy(std::vector<const BlockLegacy*> blocks);

    GMLIB_API std::optional<bool> setCanDestroy(std::vector<std::string>& blocks);

    GMLIB_API std::vector<const BlockLegacy*> getCanPlaceOn();

    GMLIB_API void setCanPlaceOn(std::vector<const BlockLegacy*> blocks);

    GMLIB_API std::optional<bool> setCanPlaceOn(std::vector<std::string>& blocks);

    GMLIB_API bool setShouldKeepOnDeath(bool value);

    GMLIB_API std::optional<bool> getShouldKeepOnDeath();

    GMLIB_API bool setItemLockMode(::ItemLockMode mode);

    GMLIB_API std::optional<ItemLockMode> getItemLockMode();
};