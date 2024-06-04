#include "Global.h"
#include <GMLIB/Utils/StringUtils.h>

namespace GMLIB::StringUtils {

std::string toSnakeCase(std::string_view str, bool withSpace) {
    auto result = ll::string_utils::toSnakeCase(str);
    return withSpace ? ll::string_utils::replaceAll(result, "_", " ") : result;
}

std::string toCamelCase(std::string_view str) {
    std::stringstream result;
    bool              capitalizeNext = true;
    for (char c : std::string(str)) {
        if (c == '_' || c == ' ') {
            capitalizeNext = true;
        } else {
            if (capitalizeNext) {
                result << static_cast<char>(toupper(c));
                capitalizeNext = false;
            } else {
                result << c;
            }
        }
    }
    return result.str();
}

} // namespace GMLIB::StringUtils