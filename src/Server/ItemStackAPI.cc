#include "Global.h"
#include <GMLIB/Server/ItemStackAPI.h>
#include <mc/world/item/components/ItemContext.h>

void GMLIB_ItemStack::setCount(int value) { this->mCount = value; }

void GMLIB_ItemStack::addCount(int value) { this->mCount += value; }

void GMLIB_ItemStack::removeCount(int value) { this->mCount -= value; }

std::unique_ptr<CompoundTag> GMLIB_ItemStack::getNbt() { return save(); }

void GMLIB_ItemStack::setNbt(CompoundTag& nbt) { load(nbt); }

void GMLIB_ItemStack::setAuxValue(short auxValue) { this->mAuxValue = auxValue; }

void GMLIB_ItemStack::setItem(std::string name, int count, short auxValue) {
    auto item = ItemStack{name, count, auxValue};
    setItem(&item);
}

void GMLIB_ItemStack::setItem(ItemStack* item) {
    auto itemContext = (ItemContext*)this;
    itemContext->setItem(*item);
}

std::vector<const BlockLegacy*> GMLIB_ItemStack::getCanDestroy() { return mCanDestroy; }

void GMLIB_ItemStack::setCanDestroy(std::vector<const BlockLegacy*> blocks) { mCanDestroy = blocks; }

std::optional<bool> GMLIB_ItemStack::setCanDestroy(std::vector<std::string>& blocks) {
    auto itemContext = (ItemContext*)this;
    return itemContext->setCanDestroy(blocks);
}

std::vector<const BlockLegacy*> GMLIB_ItemStack::getCanPlaceOn() { return mCanPlaceOn; }

void GMLIB_ItemStack::setCanPlaceOn(std::vector<const BlockLegacy*> blocks) { mCanPlaceOn = blocks; }

std::optional<bool> GMLIB_ItemStack::setCanPlaceOn(std::vector<std::string>& blocks) {
    auto itemContext = (ItemContext*)this;
    return itemContext->setCanPlaceOn(blocks);
}

bool GMLIB_ItemStack::setShouldKeepOnDeath(bool value) {
    auto itemContext = (ItemContext*)this;
    return itemContext->setKeepOnDeath(value);
}

std::optional<bool> GMLIB_ItemStack::getShouldKeepOnDeath() {
    auto itemContext = (ItemContext*)this;
    return itemContext->getKeepOnDeath();
}

bool GMLIB_ItemStack::setItemLockMode(::ItemLockMode mode) {
    auto itemContext = (ItemContext*)this;
    return itemContext->setLockMode(mode);
}

std::optional<ItemLockMode> GMLIB_ItemStack::getItemLockMode() {
    auto itemContext = (ItemContext*)this;
    return itemContext->getLockMode();
}