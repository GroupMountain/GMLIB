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
    GMLIB_API McLang(std::unordered_map<std::string, std::string> const& data);

    GMLIB_API McLang() = default;

public:
    virtual ~McLang() = default;

public:
    GMLIB_API static McLang parse(std::string const& data);

    GMLIB_API static McLang parse_file(std::string const& filePath);

public:
    GMLIB_API std::optional<std::string> try_get(std::string const& key);

    GMLIB_API std::string
    translate(std::string const& key, std::vector<std::string> const& param = {}, std::string const& data = "%0$s");

    GMLIB_API std::string
              get(std::string const& key, std::vector<std::string> const& param = {}, std::string const& data = "%0$s");

    GMLIB_API void set_value(std::string const& key, std::string const& value);

    GMLIB_API void erase(std::string const& key);

    GMLIB_API bool has_value(std::string const& key);

    GMLIB_API void merge_patch(McLang const& data);

    GMLIB_API std::string dump();

    GMLIB_API bool write_to_file(std::string const& filePath);

    GMLIB_API std::unordered_map<std::string, std::string>& getTranslationMap();
};

} // namespace GMLIB::Files