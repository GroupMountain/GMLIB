#include "Global.h"
#include <GMLIB/Server/CompoundTagAPI.h>

namespace GMLIB::CompoundTagHelper {

GMLIB_API DataLoadHelper* getDataLoadHelper() {
    return (DataLoadHelper*)ll::memory::resolveSymbol("??_7DefaultDataLoadHelper@@6B@");
}

GMLIB_API std::unique_ptr<CompoundTag> getNbt(Actor* ac) { return ac->save(); }

GMLIB_API std::unique_ptr<CompoundTag> getNbt(Player* pl) { return pl->save(); }

GMLIB_API std::unique_ptr<CompoundTag> getNbt(BlockActor* blac) { return blac->save(); }

GMLIB_API std::unique_ptr<CompoundTag> getNbt(ItemStack* item) { return item->save(); }

GMLIB_API bool setNbt(Actor* ac, CompoundTag* nbt) { return ac->load(*nbt); }

GMLIB_API bool setNbt(Player* pl, CompoundTag* nbt) { return pl->load(*nbt); }

GMLIB_API void setNbt(BlockActor* blac, CompoundTag* nbt) { blac->load(*nbt); }

GMLIB_API void setNbt(ItemStack* item, CompoundTag* nbt) { item->load(*nbt); }

} // namespace GMLIB::CompoundTagHelper
