#include "Global.h"
#include <GMLIB/Server/ItemStackAPI.h>
#include <mc/world/item/ItemLockHelper.h>
#include <mc/world/item/enchanting/EnchantUtils.h>

GMLIB_ItemStack::GMLIB_ItemStack(std::string_view name, int count, int auxValue, class CompoundTag const* userData)
: ItemStack(name, count, auxValue, userData) {}

GMLIB_ItemStack::GMLIB_ItemStack(ItemStack const& item) : ItemStack(item) {}

GMLIB_ItemStack::GMLIB_ItemStack() : ItemStack() {}

GMLIB_ItemStack::GMLIB_ItemStack(class ItemInstance const& item) : ItemStack(item) {}

GMLIB_ItemStack::GMLIB_ItemStack(class RecipeIngredient const& ingredient) : ItemStack(ingredient) {}

GMLIB_ItemStack::GMLIB_ItemStack(class BlockLegacy const& blockLegacy, int count) : ItemStack(blockLegacy, count) {}

GMLIB_ItemStack::GMLIB_ItemStack(class Block const& block, int count, class CompoundTag const* userData)
: ItemStack(block, count, userData) {}

GMLIB_ItemStack::GMLIB_ItemStack(class Item const& item, int count, int auxValue, class CompoundTag const* userData)
: ItemStack(item, count, auxValue, userData) {}

int GMLIB_ItemStack::getCount() { return mCount; }

void GMLIB_ItemStack::setCount(int value) { mCount = value; }

void GMLIB_ItemStack::addCount(int value) { mCount += value; }

void GMLIB_ItemStack::removeCount(int value) { mCount -= value; }

std::unique_ptr<CompoundTag> GMLIB_ItemStack::getNbt() { return save(); }

void GMLIB_ItemStack::setNbt(CompoundTag& nbt) { load(nbt); }

short GMLIB_ItemStack::getAuxValue() { return mAuxValue; }

void GMLIB_ItemStack::setAuxValue(short auxValue) { mAuxValue = auxValue; }

void GMLIB_ItemStack::setItem(std::string const& name, int count, short auxValue) { reinit(name, count, auxValue); }

std::vector<const BlockLegacy*> GMLIB_ItemStack::getCanDestroy() { return mCanDestroy; }

void GMLIB_ItemStack::setCanDestroy(std::vector<const BlockLegacy*> blocks) { mCanDestroy = blocks; }

void GMLIB_ItemStack::setCanDestroy(std::vector<std::string> const& blocks) {
    std::vector<const BlockLegacy*> data;
    for (auto& name : blocks) {
        if (auto block = BlockLegacy::tryGetFromRegistry(name)) {
            data.push_back(block.as_ptr());
        }
    }
    mCanDestroy = data;
}

std::vector<const BlockLegacy*> GMLIB_ItemStack::getCanPlaceOn() { return mCanPlaceOn; }

void GMLIB_ItemStack::setCanPlaceOn(std::vector<const BlockLegacy*> blocks) { mCanPlaceOn = blocks; }

void GMLIB_ItemStack::setCanPlaceOn(std::vector<std::string> const& blocks) {
    std::vector<const BlockLegacy*> data;
    for (auto& name : blocks) {
        if (auto block = BlockLegacy::tryGetFromRegistry(name)) {
            data.push_back(block.as_ptr());
        }
    }
    mCanPlaceOn = data;
}

bool GMLIB_ItemStack::isUnbreakable() {
    auto nbt = getNbt();
    if (auto tag = nbt->getCompound("tag")) {
        return (bool)tag->getByte("Unbreakable");
    }
    return false;
}

void GMLIB_ItemStack::setUnbreakable(bool value) {
    auto nbt = getNbt();
    auto tag = nbt->getCompound("tag");
    if (!tag) {
        nbt->putCompound("tag", CompoundTag{});
    }
    nbt->getCompound("tag")->putByte("Unbreakable", (byte)value);
    setNbt(*nbt);
}

void GMLIB_ItemStack::setShouldKeepOnDeath(bool value) { ItemLockHelper::setKeepOnDeath(*this, value); }

bool GMLIB_ItemStack::getShouldKeepOnDeath() { return ItemLockHelper::shouldKeepOnDeath(*this); }

void GMLIB_ItemStack::setItemLockMode(::ItemLockMode mode) { ItemLockHelper::setItemLockMode(*this, mode); }

::ItemLockMode GMLIB_ItemStack::getItemLockMode() { return ItemLockHelper::getItemLockMode(*this); }