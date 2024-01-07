#include "Global.h"
#include <GMLIB/Server/CompoundTagAPI.h>

namespace GMLIB::CompoundTagHelper {

DataLoadHelper* getDataLoadHelper() {
    return (DataLoadHelper*)ll::memory::resolveSymbol("??_7DefaultDataLoadHelper@@6B@");
}

std::unique_ptr<CompoundTag> getNbt(Actor* ac) {
    return ac->saveToNbt();
}

std::unique_ptr<CompoundTag> getNbt(Player* pl) {
    return pl->saveToNbt();
}

std::unique_ptr<CompoundTag> getNbt(BlockActor* blac) {
    return blac->saveToNbt();
}

std::unique_ptr<CompoundTag> getNbt(ItemStack* item) {
    return item->save();
}

bool setNbt(Actor* ac, CompoundTag* nbt) {
    return ac->load(*nbt, *getDataLoadHelper());
}

bool setNbt(Player* pl, CompoundTag* nbt) {
    return pl->load(*nbt, *getDataLoadHelper());
}

bool setNbt(BlockActor* blac, CompoundTag* nbt) {
    return blac->load(*nbt, *getDataLoadHelper());
}

bool setNbt(ItemStack* item, CompoundTag* nbt) {
    return item->load(*nbt, *getDataLoadHelper());
}

} // namespace GMLIB::CompoundTagHelper
