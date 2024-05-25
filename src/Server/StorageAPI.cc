#include "Global.h"
#include <GMLIB/Server/StorageAPI.h>

optional_ref<StorageAPI> StorageAPI::getInstance() { return (StorageAPI*)GMLIB::Global<DBStorage>; }


bool StorageAPI::hasKey(std::string_view key) { return hasKey(key, DBHelpers::Category::All); }

std::string_view StorageAPI::getData(std::string_view key) {
    std::string result;
    loadData(key, result, DBHelpers::Category::All);
    return std::move(result);
}

void StorageAPI::saveData(std::string_view key, std::string_view data) {
    saveData(std::string(key), std::string(data), DBHelpers::Category::All);
}

std::unique_ptr<CompoundTag> StorageAPI::getCompoundTag(std::string_view key) {
    return getCompoundTag(std::string(key), DBHelpers::Category::All);
}

void StorageAPI::saveCompoundTag(std::string_view key, CompoundTag& nbt) {
    saveData(std::string(key), nbt.toBinaryNbt(), DBHelpers::Category::All);
}

void StorageAPI::forEachKey(std::function<void(std::string_view key, std::string_view data)> func) {
    forEachKeyWithPrefix("", func);
}

void StorageAPI::forEachCompoundTag(std::function<void(std::string_view key, CompoundTag& data)> func) {
    auto function = [&](std::string_view key, std::string_view data) {
        auto nbt = CompoundTag::fromBinaryNbt(data);
        if (nbt) {
            func(key, *nbt);
        }
    };
    forEachKey(function);
}

void StorageAPI::forEachKeyWithPrefix(
    std::string_view                                                 prefix,
    std::function<void(std::string_view key, std::string_view data)> func
) {
    forEachKeyWithPrefix(prefix, DBHelpers::Category::All, func);
}