#pragma once
#include "GMLIB/Macros.h"
#include "nlohmann/json.hpp"
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

    GMLIB_NDAPI static McLang from_json(std::string const& jsonString);

    GMLIB_NDAPI static McLang from_json(nlohmann::json const& json);

public:
    GMLIB_NDAPI std::optional<std::string> try_get(std::string const& key);

    GMLIB_NDAPI std::string
    translate(std::string const& key, std::vector<std::string> const& param = {}, std::string const& data = "%0$s");

    GMLIB_NDAPI std::string
    get(std::string const& key, std::vector<std::string> const& param = {}, std::string const& data = "%0$s");

    GMLIB_API void set(std::string const& key, std::string const& value);

    GMLIB_API bool erase(std::string const& key);

    GMLIB_API void erase(std::vector<std::string> const& keys);

    GMLIB_NDAPI bool has_value(std::string const& key);

    GMLIB_API void merge_patch(McLang const& data);

    GMLIB_NDAPI std::string dump();

    GMLIB_NDAPI std::string to_json_string(int format = 4);

    GMLIB_NDAPI nlohmann::json to_json();

    GMLIB_API bool write_to_file(std::filesystem::path const& filePath);

    GMLIB_NDAPI std::unordered_map<std::string, std::string>& getTranslationMap();
};

} // namespace GMLIB::Files