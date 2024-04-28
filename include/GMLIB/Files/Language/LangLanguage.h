#pragma once
#include "GMLIB/Files/Language/McLang.h"
#include "GMLIB/Macros.h"

namespace GMLIB::Files {

class LangLanguage : public McLang {
private:
    std::string mFilePath;

public:
    GMLIB_API LangLanguage(std::string const& filePath, std::string const& defaultLanguage);

public:
    GMLIB_API bool init();

    GMLIB_API bool reload();

    GMLIB_API bool save_file();
};

} // namespace GMLIB::Files