#include "Global.h"
#include <GMLIB/Server/CompoundTagAPI.h>

DataLoadHelper* GMLIB_CompoundTag::getDataLoadHelper() {
    return (DataLoadHelper*)ll::memory::resolveSymbol("??_7DefaultDataLoadHelper@@6B@");
}

std::unique_ptr<CompoundTag> GMLIB_CompoundTag::getNbt(Actor* ac) { return ac->save(); }

std::unique_ptr<CompoundTag> GMLIB_CompoundTag::getNbt(Player* pl) { return pl->save(); }

std::unique_ptr<CompoundTag> GMLIB_CompoundTag::getNbt(BlockActor* blac) { return blac->save(); }

std::unique_ptr<CompoundTag> GMLIB_CompoundTag::getNbt(ItemStack* item) { return item->save(); }

bool GMLIB_CompoundTag::setNbt(Actor* ac, CompoundTag* nbt) { return ac->load(*nbt); }

bool GMLIB_CompoundTag::setNbt(Player* pl, CompoundTag* nbt) { return pl->load(*nbt); }

void GMLIB_CompoundTag::setNbt(BlockActor* blac, CompoundTag* nbt) { blac->load(*nbt); }

void GMLIB_CompoundTag::setNbt(ItemStack* item, CompoundTag* nbt) { item->load(*nbt); }

std::unique_ptr<CompoundTag> GMLIB_CompoundTag::createCompoundTag() { return std::make_unique<CompoundTag>(); }