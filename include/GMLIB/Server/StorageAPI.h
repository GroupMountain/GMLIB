#pragma once
#include "GMLIB/Macros.h"
#include "mc/nbt/CompoundTag.h"
#include "mc/world/level/storage/DBStorage.h"

class StorageAPI : public DBStorage {
public:
    using DBStorage::forEachKeyWithPrefix;
    using DBStorage::getCompoundTag;
    using DBStorage::hasKey;
    using DBStorage::saveData;

public:
    GMLIB_API static StorageAPI* getInstance();

public:
    GMLIB_API bool hasKey(std::string_view key);

    GMLIB_API std::string_view getData(std::string_view key);

    GMLIB_API void saveData(std::string_view key, std::string_view data);

    GMLIB_API std::unique_ptr<CompoundTag> getCompoundTag(std::string_view key);

    GMLIB_API void saveCompoundTag(std::string_view key, CompoundTag& nbt);

    GMLIB_API void forEachKey(std::function<void(std::string_view key, std::string_view data)> func);

    GMLIB_API void forEachCompoundTag(std::function<void(std::string_view key, CompoundTag& data)> func);

    GMLIB_API void forEachKeyWithPrefix(
        std::string_view                                                 prefix,
        std::function<void(std::string_view key, std::string_view data)> func
    );
};