#include "Global.h"
#include <GMLIB/Server/CompoundTagAPI.h>
#include <ll/api/io/FileUtils.h>
#include <mc/world/level/block/actor/BlockActor.h>

std::unique_ptr<DataLoadHelper> GMLIB_CompoundTag::getDataLoadHelper() {
    return std::make_unique<DefaultDataLoadHelper>();
}

std::unique_ptr<CompoundTag> GMLIB_CompoundTag::getFromActor(Actor& ac) {
    auto nbt = std::make_unique<CompoundTag>();
    ac.save(*nbt);
    return std::move(nbt);
}

std::unique_ptr<CompoundTag> GMLIB_CompoundTag::getFromPlayer(Player& pl) {
    auto nbt = std::make_unique<CompoundTag>();
    pl.save(*nbt);
    return std::move(nbt);
}

std::unique_ptr<CompoundTag> GMLIB_CompoundTag::getFromBlockActor(BlockActor& blac) {
    auto nbt = std::make_unique<CompoundTag>();
    blac.save(*nbt);
    return std::move(nbt);
}

// IDA: Block::Block()
// CompoundTag::CompoundTag((CompoundTag *)(a1 + 160));
std::unique_ptr<CompoundTag> GMLIB_CompoundTag::getFromBlock(Block& block) {
    auto nbt = (CompoundTag*)((uintptr_t)&block + 160);
    return nbt->clone();
}

std::unique_ptr<CompoundTag> GMLIB_CompoundTag::getFromItemStack(ItemStack& item) { return item.save(); }

bool GMLIB_CompoundTag::setToActor(Actor& ac) { return ac.load(*this, *getDataLoadHelper()); }

bool GMLIB_CompoundTag::setToPlayer(Player& pl) { return pl.load(*this, *getDataLoadHelper()); }

void GMLIB_CompoundTag::setToBlockActor(BlockActor& blac) {
    blac.load(ll::service::getLevel(), *this, *getDataLoadHelper());
}

void GMLIB_CompoundTag::setToItemStack(ItemStack& item) { item.load(*this); }

void GMLIB_CompoundTag::setToBlock(Block& block) {
    auto ctag = (CompoundTag*)((uintptr_t)&block + 160);
    ctag->deepCopy(*this);
}

void GMLIB_CompoundTag::writeNbtTags(
    CompoundTag&                    originNbt,
    CompoundTag&                    dataNbt,
    const std::vector<std::string>& tags
) {
    for (auto& tag : tags) {
        if (dataNbt.get(tag)) {
            originNbt.put(tag, dataNbt.get(tag)->copy());
        }
    }
}

std::optional<CompoundTag> GMLIB_CompoundTag::readFromFile(std::filesystem::path const& path, bool isBinary) {
    if (auto fileData = ll::file_utils::readFile(path, isBinary)) {
        if (isBinary) {
            auto nbt = CompoundTag::fromBinaryNbt(fileData.value());
            return nbt.value();
        } else {
            auto nbt = CompoundTag::fromSnbt(fileData.value());
            return nbt.value();
        }
    }
    return {};
}

bool GMLIB_CompoundTag::saveToFile(std::filesystem::path const& path, CompoundTag& nbt, bool isBinary) {
    std::string fileData;
    if (isBinary) {
        fileData = nbt.toBinaryNbt();
    } else {
        fileData = nbt.toSnbt(SnbtFormat::ForceQuote, 0);
    }
    return ll::file_utils::writeFile(path, fileData, isBinary);
}