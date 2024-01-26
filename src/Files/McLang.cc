#include <GMLIB/Files/McLang.h>
#include <Global.h>

namespace GMLIB::Files {

McLang::McLang(std::map<std::string, std::string> data) : mData(data) {}

McLang McLang::prase_file(std::string filePath) {
    std::vector<std::string> lines;
    std::ifstream            file(filePath);
    auto                     result = McLang();
    if (file.is_open()) {
        std::string line;
        while (std::getline(file, line)) {
            if (!line.empty()) {
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
        file.close();
    }
    return result;
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
    return lines;
}

McLang McLang::prase(std::string data) {
    auto lines  = splitStringByNewline(data);
    auto result = McLang();
    for (auto& line : lines) {
        if (!line.empty()) {
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
    return result;
}

std::string McLang::dump() {
    std::string result;
    for (auto& key : mData) {
        result = result + key.first + "=" + key.second + "\n";
    }
    return result;
}

void McLang::write_to_file(std::string filePath) {
    std::ofstream newFile(filePath);
    newFile << dump();
    newFile.close();
}

bool McLang::has_value(std::string key) { return (bool)mData.count(key); }

void McLang::set(std::string key, std::string value) { mData[key] = value; }

void McLang::merge_patch(McLang newData) {
    auto result = newData;
    for (auto info : mData) {
        if (!newData.has_value(info.first)) {
            result.set(info.first, info.second);
        }
    }
    mData = result.mData;
}

std::optional<std::string> McLang::get(std::string key) {
    if (mData.count(key)) {
        return mData[key];
    }
    return {};
}

void McLang::erase(std::string key) {
    if (has_value(key)) {
        mData.erase(key);
    }
}

std::string McLang::translate(std::string key, std::vector<std::string> data, std::string translateKeys) {
    auto value = get(key);
    if (value.has_value()) {
        auto result = value.value();
        ll::utils::string_utils::replaceAll(result, "\\n", "\n");
        if (data.empty()) {
            return result;
        }
        for (int i = 0; i <= data.size() - 1; i++) {
            auto oldValue = translateKeys;
            ll::utils::string_utils::replaceAll(oldValue, "0", std::to_string(i + 1));
            ll::utils::string_utils::replaceAll(result, oldValue, data[i]);
        }
        return result;
    }
    return key;
}

} // namespace GMLIB::Files