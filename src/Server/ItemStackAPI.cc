#include "Global.h"
#include <GMLIB/Server/ItemStackAPI.h>

void GMLIB_ItemStack::setCount(int value) { this->mCount = value; }

void GMLIB_ItemStack::addCount(int value) { this->mCount += value; }

void GMLIB_ItemStack::removeCount(int value) { this->mCount -= value; }

std::unique_ptr<CompoundTag> GMLIB_ItemStack::getNbt() { return save(); }

void GMLIB_ItemStack::setNbt(CompoundTag* nbt) { load(*nbt); }

void GMLIB_ItemStack::setAuxValue(short auxValue) { this->mAuxValue = auxValue; }