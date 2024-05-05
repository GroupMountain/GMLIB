#include "Global.h"
#include <GMLIB/Server/StorageAPI.h>

StorageAPI* StorageAPI::getInstance() { return (StorageAPI*)GMLIB::Global<DBStorage>; }


bool StorageAPI::hasData(std::string_view key) { return hasKey(key, DBHelpers::Category::All); }

std::string_view StorageAPI::get(std::string_view key) {
    std::string result;
    loadData(key, result, DBHelpers::Category::All);
    return std::move(result);
}

void StorageAPI::set(std::string_view key, std::string_view data) {
    saveData(std::string(key), std::string(data), DBHelpers::Category::All);
}

std::unique_ptr<CompoundTag> StorageAPI::getData(std::string_view key) {
    return getCompoundTag(std::string(key), DBHelpers::Category::All);
}

void StorageAPI::setData(std::string_view key, CompoundTag& nbt) {
    saveData(std::string(key), nbt.toBinaryNbt(), DBHelpers::Category::All);
}