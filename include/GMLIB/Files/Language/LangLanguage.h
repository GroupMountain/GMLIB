#pragma once
#include "GMLIB/GMLIB.h"
#include "GMLIB/Files/Language/McLang.h"

namespace GMLIB::Files {

class LangLanguage : public McLang {
private:
    std::string mFilePath;

public:
    GMLIB_API LangLanguage(std::string filePath, std::string defaultLanguage);

public:
    GMLIB_API bool init();
};

} // namespace GMLIB::Files