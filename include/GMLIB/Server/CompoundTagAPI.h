#pragma once
#include "GMLIB/DllExport.h"

namespace GMLIB::CompoundTagHelper {

GMLIB_API DataLoadHelper* getDataLoadHelper();

GMLIB_API std::unique_ptr<CompoundTag> getNbt(Actor* ac);
GMLIB_API std::unique_ptr<CompoundTag> getNbt(Player* pl);
GMLIB_API std::unique_ptr<CompoundTag> getNbt(BlockActor* blac);
GMLIB_API std::unique_ptr<CompoundTag> getNbt(ItemStack* item);

GMLIB_API bool setNbt(Actor* ac, CompoundTag* nbt);
GMLIB_API bool setNbt(Player* pl, CompoundTag* nbt);
GMLIB_API void setNbt(BlockActor* blac, CompoundTag* nbt);
GMLIB_API void setNbt(ItemStack* item, CompoundTag* nbt);

} // namespace GMLIB::CompoundTagHelper