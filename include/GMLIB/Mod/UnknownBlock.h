#include "GMLIB/GMLIB.h"

class GMLIB_UnknownBlock {
public:
    GMLIB_API std::unordered_set<std::string> static getUnknownBlockLegacyNameList();

    GMLIB_API void static setAutoCleanUnknownBlockEnabled(bool value = true);
};
