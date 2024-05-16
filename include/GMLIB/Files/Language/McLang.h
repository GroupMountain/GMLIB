#pragma once
#include "GMLIB/Macros.h"
#include <iostream>
#include <optional>
#include <unordered_map>

namespace GMLIB::Files {

class McLang {
private:
    std::unordered_map<std::string, std::string> mData;

public:
    GMLIB_NDAPI McLang(std::unordered_map<std::string, std::string> const& data);

    GMLIB_NDAPI McLang() = default;

public:
    virtual ~McLang() = default;

public:
    GMLIB_NDAPI static McLang parse(std::string const& data);

    GMLIB_NDAPI static McLang parse_file(std::filesystem::path const& filePath);

public:
    GMLIB_NDAPI std::optional<std::string> try_get(std::string const& key);

    GMLIB_NDAPI std::string
    translate(std::string const& key, std::vector<std::string> const& param = {}, std::string const& data = "%0$s");

    GMLIB_NDAPI std::string
    get(std::string const& key, std::vector<std::string> const& param = {}, std::string const& data = "%0$s");

    GMLIB_API void set_value(std::string const& key, std::string const& value);

    GMLIB_API void erase(std::string const& key);

    GMLIB_NDAPI bool has_value(std::string const& key);

    GMLIB_API void merge_patch(McLang const& data);

    GMLIB_NDAPI std::string dump();

    GMLIB_API bool write_to_file(std::filesystem::path const& filePath);

    GMLIB_NDAPI std::unordered_map<std::string, std::string>& getTranslationMap();
};

} // namespace GMLIB::Files