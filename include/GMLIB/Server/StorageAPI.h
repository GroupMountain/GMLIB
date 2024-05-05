#pragma once
#include "GMLIB/Macros.h"
#include "mc/nbt/CompoundTag.h"
#include "mc/world/level/storage/DBStorage.h"

class StorageAPI : public DBStorage {
public:
    GMLIB_API static StorageAPI* getInstance();

public:
    GMLIB_API bool hasData(std::string_view key);

    GMLIB_API std::string_view get(std::string_view key);

    GMLIB_API void set(std::string_view key, std::string_view data);

    GMLIB_API std::unique_ptr<CompoundTag> getData(std::string_view key);

    GMLIB_API void setData(std::string_view key, CompoundTag& nbt);
};