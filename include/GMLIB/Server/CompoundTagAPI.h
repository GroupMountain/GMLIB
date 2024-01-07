#pragma once
#include "GMLIB/DllExport.h"

namespace GMLIB::CompoundTagHelper {

DataLoadHelper* getDataLoadHelper();

std::unique_ptr<CompoundTag> getNbt(Actor* ac);
std::unique_ptr<CompoundTag> getNbt(Player* pl);
std::unique_ptr<CompoundTag> getNbt(BlockActor* blac);
std::unique_ptr<CompoundTag> getNbt(ItemStack* item);

bool setNbt(Actor* ac, CompoundTag* nbt);
bool setNbt(Player* pl, CompoundTag* nbt);
bool setNbt(BlockActor* blac, CompoundTag* nbt);
bool setNbt(ItemStack* item, CompoundTag* nbt);

} // namespace GMLIB::CompoundTagHelper