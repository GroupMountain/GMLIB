#pragma once
#include "GMLIB/Macros.h"

namespace GMLIB::Files {

class McLang {
private:
    std::unordered_map<std::string, std::string> mData;

public:
    GMLIB_API McLang(std::unordered_map<std::string, std::string> data);

    GMLIB_API McLang() = default;

public:
    virtual ~McLang() = default;

public:
    GMLIB_API static McLang parse(std::string data);

    GMLIB_API static McLang parse_file(std::string filePath);

public:
    GMLIB_API std::optional<std::string> get_value(std::string key);

    GMLIB_API std::string
              translate(std::string key, std::vector<std::string> data = {}, std::string translateKeys = "%0$s");

    GMLIB_API std::string get(std::string key, std::vector<std::string> data = {}, std::string translateKeys = "%0$s");

    GMLIB_API void set_value(std::string key, std::string value);

    GMLIB_API void erase(std::string key);

    GMLIB_API bool has_value(std::string key);

    GMLIB_API void merge_patch(McLang data);

    GMLIB_API std::string dump();

    GMLIB_API bool write_to_file(std::string filePath);
};

} // namespace GMLIB::Files