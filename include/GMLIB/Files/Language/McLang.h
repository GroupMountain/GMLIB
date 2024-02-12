#pragma once
#include "GMLIB/GMLIB.h"

namespace GMLIB::Files {

class McLang {
private:
    std::map<std::string, std::string> mData;

public:
    GMLIB_API McLang(std::map<std::string, std::string> data);

    GMLIB_API McLang() = default;

public:
    virtual ~McLang() = default;

public:
    GMLIB_API static McLang prase(std::string data);

    GMLIB_API static McLang prase_file(std::string filePath);

public:
    GMLIB_API std::optional<std::string> get(std::string key);

    GMLIB_API std::string
              translate(std::string key, std::vector<std::string> data = {}, std::string translateKeys = "%0$s");

    GMLIB_API void set(std::string key, std::string value);

    GMLIB_API void erase(std::string key);

    GMLIB_API bool has_value(std::string key);

    GMLIB_API void merge_patch(McLang data);

    GMLIB_API std::string dump();

    GMLIB_API bool write_to_file(std::string filePath);
};

} // namespace GMLIB::Files