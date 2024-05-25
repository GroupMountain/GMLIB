#include "Global.h"
#include <GMLIB/Files/Language/McLang.h>

namespace GMLIB::Files {

McLang::McLang(std::unordered_map<std::string, std::string> const& data) : mData(data) {}

McLang McLang::parse_file(std::filesystem::path const& filePath) {
    std::vector<std::string> lines;
    std::ifstream            file(filePath);
    auto                     result = McLang();
    if (file.is_open()) {
        std::string line;
        while (std::getline(file, line)) {
            if (!line.empty()) {
                ll::string_utils::replaceAll(line, "\t", "");
                ll::utils::string_utils::replaceAll(line, "\\n", "\n");
                size_t equalPos = line.find("=");
                if (equalPos != std::string::npos) {
                    auto key = line.substr(0, equalPos);
                    ll::string_utils::replaceAll(key, " ", "");
                    auto   right_key  = line.substr(equalPos + 1, line.size());
                    size_t commentPos = right_key.find("#");
                    auto   value      = right_key.substr(0, commentPos);
                    while (value.ends_with(" ")) {
                        value.erase(value.size() - 1);
                    }
                    if (!key.empty() && !value.empty()) {
                        result.mData[key] = value;
                    }
                }
            }
        }
        file.close();
    }
    return std::move(result);
}

std::vector<std::string> splitStringByNewline(const std::string& input) {
    std::vector<std::string> lines;
    size_t                   startPos = 0;
    while (startPos < input.size()) {
        size_t endPos = input.find_first_of("\n\r", startPos);
        if (endPos == std::string::npos) {
            lines.push_back(input.substr(startPos));
            break;
        }
        lines.push_back(input.substr(startPos, endPos - startPos));
        startPos = endPos + 1;
    }
    return std::move(lines);
}

McLang McLang::parse(std::string const& data) {
    auto lines  = splitStringByNewline(data);
    auto result = McLang();
    for (auto& line : lines) {
        if (!line.empty()) {
            ll::string_utils::replaceAll(line, "\t", "");
            ll::utils::string_utils::replaceAll(line, "\\n", "\n");
            size_t equalPos = line.find("=");
            auto   key      = line.substr(0, equalPos);
            ll::string_utils::replaceAll(key, " ", "");
            auto   right_key  = line.substr(equalPos + 1, line.size());
            size_t commentPos = right_key.find("#");
            auto   value      = right_key.substr(0, commentPos);
            while (value.ends_with(" ")) {
                value.erase(value.size() - 1);
            }
            if (!key.empty() && !value.empty()) {
                result.mData[key] = value;
            }
        }
    }
    return std::move(result);
}

McLang McLang::from_json(std::string const& jsonString) {
    auto json = nlohmann::json::parse(jsonString, nullptr, true, true);
    return from_json(json);
}

McLang McLang::from_json(nlohmann::json const& json) {
    std::unordered_map<std::string, std::string> languageMap;
    for (nlohmann::json::const_iterator it = json.begin(); it != json.end(); ++it) {
        if (it.value().is_string()) {
            languageMap[it.key()] = it.value().get<std::string>();
        }
    }
    return McLang(languageMap);
}

std::string McLang::dump() {
    std::map<std::string, std::string> map;
    for (auto& [key, lang] : mData) {
        map[key] = lang;
        ll::utils::string_utils::replaceAll(map[key], "\n", "\\n");
    }
    std::string result;
    for (auto& key : map) {
        result = result + key.first + "=" + key.second + "\n";
    }
    return std::move(result);
}

std::string McLang::to_json_string(int format) { return to_json().dump(format); }

nlohmann::json McLang::to_json() {
    nlohmann::json json;
    for (auto& [key, val] : mData) {
        json[key] = val;
    }
    return std::move(json);
}

bool McLang::write_to_file(std::filesystem::path const& filePath) {
    std::ofstream newFile(filePath);
    if (newFile.is_open()) {
        newFile << dump();
        newFile.close();
        return true;
    }
    return false;
}

bool McLang::has_value(std::string const& key) { return (bool)mData.count(key); }

void McLang::set(std::string const& key, std::string const& value) { mData[key] = value; }

void McLang::merge_patch(McLang const& newData) {
    for (auto& key : newData.mData) {
        mData[key.first] = key.second;
    }
}

std::optional<std::string> McLang::try_get(std::string const& key) {
    if (mData.contains(key)) {
        return mData[key];
    }
    return {};
}

bool McLang::erase(std::string const& key) {
    if (has_value(key)) {
        mData.erase(key);
        return true;
    }
    return false;
}

void McLang::erase(std::vector<std::string> const& keys) {
    for (auto& key : keys) {
        mData.erase(key);
    }
}

std::string
McLang::translate(std::string const& key, std::vector<std::string> const& data, std::string const& translateKeys) {
    auto value = try_get(key);
    if (value.has_value()) {
        auto result = value.value();
        if (data.empty()) {
            return result;
        }
        if (translateKeys == "%0$s" && data.size() == 1) {
            ll::utils::string_utils::replaceAll(result, "%s", translate(data[0]));
        }
        for (int i = 0; i <= data.size() - 1; i++) {
            auto oldValue = translateKeys;
            ll::utils::string_utils::replaceAll(oldValue, "0", std::to_string(i + 1));
            ll::utils::string_utils::replaceAll(result, oldValue, translate(data[i]));
        }
        return result;
    }
    return key;
}

std::string
McLang::get(std::string const& key, std::vector<std::string> const& data, std::string const& translateKeys) {
    return translate(key, data, translateKeys);
}

std::unordered_map<std::string, std::string>& McLang::getTranslationMap() { return mData; }

} // namespace GMLIB::Files