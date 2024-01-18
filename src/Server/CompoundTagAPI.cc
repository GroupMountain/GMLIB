#include "Global.h"
#include <GMLIB/Server/CompoundTagAPI.h>

DataLoadHelper* GMLIB_CompoundTag::getDataLoadHelper() {
    return (DataLoadHelper*)ll::memory::resolveSymbol("??_7DefaultDataLoadHelper@@6B@");
}

std::unique_ptr<CompoundTag> GMLIB_CompoundTag::createCompoundTag() { return std::make_unique<CompoundTag>(); }

std::unique_ptr<CompoundTag> GMLIB_CompoundTag::getFromActor(Actor* ac) {
    auto nbt = std::make_unique<CompoundTag>();
    ac->save(*nbt);
    return std::move(nbt);
}

std::unique_ptr<CompoundTag> GMLIB_CompoundTag::getFromPlayer(Player* pl) {
    auto nbt = std::make_unique<CompoundTag>();
    pl->save(*nbt);
    return std::move(nbt);
}

std::unique_ptr<CompoundTag> GMLIB_CompoundTag::getFromBlockActor(BlockActor* blac) {
    auto nbt = std::make_unique<CompoundTag>();
    blac->save(*nbt);
    return std::move(nbt);
}

// IDA: Block::Block()
// CompoundTag::CompoundTag((CompoundTag *)(a1 + 160));
std::unique_ptr<CompoundTag> GMLIB_CompoundTag::getFromBlock(Block* block) {
    auto nbt = (CompoundTag*)((uintptr_t)block + 160);
    return nbt->clone();
}

std::unique_ptr<CompoundTag> GMLIB_CompoundTag::getFromItemStack(ItemStack* item) { return item->save(); }

bool GMLIB_CompoundTag::setToActor(Actor* ac, CompoundTag* nbt) { return ac->load(*nbt); }

bool GMLIB_CompoundTag::setToPlayer(Player* pl, CompoundTag* nbt) { return pl->load(*nbt); }

void GMLIB_CompoundTag::setToBlockActor(BlockActor* blac, CompoundTag* nbt) {
    blac->load(ll::service::getLevel(), *nbt);
}

void GMLIB_CompoundTag::setToItemStack(ItemStack* item, CompoundTag* nbt) { item->load(*nbt); }

void GMLIB_CompoundTag::setToBlock(Block* block, CompoundTag* nbt) {
    auto ctag = (CompoundTag*)((uintptr_t)block + 160);
    ctag->deepCopy(*nbt);
}