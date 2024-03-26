#include "Global.h"
#include <GMLIB/Server/ItemStackAPI.h>
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

void GMLIB_ItemStack::setItem(std::string name, int count, short auxValue) { reinit(name, count, auxValue); }

std::vector<const BlockLegacy*> GMLIB_ItemStack::getCanDestroy() { return mCanDestroy; }

void GMLIB_ItemStack::setCanDestroy(std::vector<const BlockLegacy*> blocks) { mCanDestroy = blocks; }

void GMLIB_ItemStack::setCanDestroy(std::vector<std::string> blocks) {
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

void GMLIB_ItemStack::setCanPlaceOn(std::vector<std::string> blocks) {
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

void GMLIB_ItemStack::setShouldKeepOnDeath(bool value) {
    auto nbt = getNbt();
    auto tag = nbt->getCompound("tag");
    if (!tag) {
        nbt->putCompound("tag", CompoundTag{});
    }
    nbt->getCompound("tag")->putByte("minecraft:keep_on_death", (byte)value);
    setNbt(*nbt);
}

bool GMLIB_ItemStack::getShouldKeepOnDeath() {
    auto nbt = getNbt();
    if (auto tag = nbt->getCompound("tag")) {
        return (bool)tag->getByte("minecraft:keep_on_death");
    }
    return false;
}

void GMLIB_ItemStack::setItemLockMode(::ItemLockMode mode) {
    auto nbt = getNbt();
    auto tag = nbt->getCompound("tag");
    if (!tag) {
        nbt->putCompound("tag", CompoundTag{});
    }
    nbt->getCompound("tag")->putByte("minecraft:item_lock", (byte)mode);
    setNbt(*nbt);
}

::ItemLockMode GMLIB_ItemStack::getItemLockMode() {
    auto nbt = getNbt();
    if (auto tag = nbt->getCompound("tag")) {
        return (ItemLockMode)tag->getByte("minecraft:item_lock");
    }
    return (ItemLockMode)0;
}

EnchantmentCheckResult GMLIB_ItemStack::isEnchantedIllegally() {
    if (isEnchanted()) {
        std::vector<int> legalEnchants = EnchantUtils::getLegalEnchants(getItem());
        std::vector<int> itemEnchants;
        std::sort(legalEnchants.begin(), legalEnchants.end());
        for (int enchantId = 0; enchantId <= 40; enchantId++) {
            auto ench = (Enchant::Type)enchantId;
            if (EnchantUtils::hasEnchant(ench, *this)) {
                auto minLevel = Enchant::getEnchant(ench)->getMinLevel();
                auto maxLevel = Enchant::getEnchant(ench)->getMaxLevel();
                auto nowLevel = EnchantUtils::getEnchantLevel(ench, *this);
                if (nowLevel > maxLevel) {
                    return EnchantmentCheckResult::InvalidHighLevel;
                }
                if (!std::binary_search(legalEnchants.begin(), legalEnchants.end(), enchantId)) {
                    return EnchantmentCheckResult::InvalidType;
                }
                itemEnchants.push_back(enchantId);
                for (auto& enchant : itemEnchants) {
                    if (!Enchant::getEnchant(ench)->isCompatibleWith((Enchant::Type)enchant)) {
                        return EnchantmentCheckResult::NotCompatible;
                    }
                }
            }
            if (itemEnchants.empty()) {
                return EnchantmentCheckResult::EnchantedWithoutLevel;
            }
        }
    }
    return EnchantmentCheckResult::NoError;
}