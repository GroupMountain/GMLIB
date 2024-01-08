#include "Global.h"
#include <GMLIB/Server/CompoundTagAPI.h>

namespace GMLIB::CompoundTagHelper {

GMLIB_API DataLoadHelper* getDataLoadHelper() {
    return (DataLoadHelper*)ll::memory::resolveSymbol("??_7DefaultDataLoadHelper@@6B@");
}

GMLIB_API std::unique_ptr<CompoundTag> getNbt(Actor* ac) { return ac->saveToNbt(); }

GMLIB_API std::unique_ptr<CompoundTag> getNbt(Player* pl) { return pl->saveToNbt(); }

GMLIB_API std::unique_ptr<CompoundTag> getNbt(BlockActor* blac) { return blac->saveToNbt(); }

GMLIB_API std::unique_ptr<CompoundTag> getNbt(ItemStack* item) { return item->save(); }

GMLIB_API bool setNbt(Actor* ac, CompoundTag* nbt) { return ac->load(*nbt, *getDataLoadHelper()); }

GMLIB_API bool setNbt(Player* pl, CompoundTag* nbt) { return pl->load(*nbt, *getDataLoadHelper()); }

GMLIB_API void setNbt(BlockActor* blac, CompoundTag* nbt) {
    blac->load(*ll::service::bedrock::getLevel(), *nbt, *getDataLoadHelper());
}

GMLIB_API void setNbt(ItemStack* item, CompoundTag* nbt) { item->load(*nbt); }

} // namespace GMLIB::CompoundTagHelper
