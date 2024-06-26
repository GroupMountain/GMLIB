#include "Global.h"
#include <GMLIB/Files/JsonFile.h>
#include <GMLIB/Files/Language/McLang.h>
#include <GMLIB/Files/Language/ResourceLanguage.h>
#include <GMLIB/Mod/Addons/CustomPacks.h>

namespace GMLIB::Files {

std::string defaultManifest = R"({
    "format_version": 2,
    "header": {
        "description": "Plugin language pack, this pack is automatically generated by GMLIB.",
        "platform_locked" : false,
        "min_engine_version": [1, 20, 80]
    },
    "modules": [
        {
            "description": "Plugin language pack, this pack is automatically generated by GMLIB.",
            "type": "resources"
        }
    ]
})";

ResourceLanguage::ResourceLanguage(
    std::filesystem::path const& directoryPath,
    std::string const&           pluginName,
    ushort                       versionMajor,
    ushort                       versionMinor,
    ushort                       versionPatch
)
: mPath(directoryPath),
  mPluginName(pluginName) {
    mLanguages = {};
    mVersion   = Version(versionMajor, versionMinor, versionPatch, "", "");
}

bool buildManisest(Version const& version, std::filesystem::path const& path, std::string const& name) {
    if (std::filesystem::exists(path)) {
        std::ifstream inputFile(path);
        std::string   fileContent((std::istreambuf_iterator<char>(inputFile)), std::istreambuf_iterator<char>());
        inputFile.close();
        auto oldJson    = nlohmann::ordered_json::parse(fileContent, nullptr, true, true);
        auto oldVersion = oldJson["header"]["version"].get<std::vector<int>>();
        if (oldVersion[0] == version.getMajor() && oldVersion[1] == version.getMinor()
            && oldVersion[2] == version.getPatch()) {
            return false;
        }
    }
    std::vector<int> ver          = {version.getMajor(), version.getMinor(), version.getPatch()};
    auto             json         = nlohmann::json::parse(defaultManifest, nullptr, true, true);
    json["header"]["uuid"]        = mce::UUID::random();
    json["header"]["version"]     = ver;
    json["header"]["name"]        = name + " Language Pack";
    json["modules"][0]["uuid"]    = mce::UUID::random();
    json["modules"][0]["version"] = ver;
    std::ofstream manifestFile(path);
    manifestFile << json.dump(4);
    manifestFile.close();
    return true;
}

void ResourceLanguage::initLanguage() {
    Mod::CustomPacks::addCustomPackPath(mPath);
    auto packPath = mPath / u8"language_pack";
    std::filesystem::create_directories(packPath);
    buildManisest(mVersion, packPath / u8"manifest.json", mPluginName);
    std::filesystem::create_directories(packPath / u8"texts");
    auto langJson = nlohmann::json::array();
    for (auto& [langCode, langData] : mLanguages) {
        langJson.push_back(langCode);
        auto langPath    = packPath / u8"texts" / (langCode + ".lang");
        auto newLangData = McLang::parse(langData);
        if (std::filesystem::exists(langPath)) {
            auto oldLangData = McLang::parse_file(langPath);
            newLangData.merge_patch(oldLangData);
        }
        newLangData.write_to_file(langPath);
    }
    auto infoPath = packPath / u8"texts" / u8"languages.json";
    if (std::filesystem::exists(infoPath)) {
        auto readPath    = infoPath;
        auto oldData     = GMLIB::Files::JsonFile::readFromFile(readPath);
        auto oldLangKeys = oldData.get<std::vector<std::string>>();
        for (auto& lk : oldLangKeys) {
            if (!mLanguages.count(lk)) {
                langJson.push_back(lk);
            }
        }
    }
    std::ofstream newLangJson(infoPath);
    newLangJson << langJson.dump(4);
    newLangJson.close();
}

void ResourceLanguage::addLanguage(std::string const& identifider, std::string const& language) {
    mLanguages[identifider] = language;
}

void ResourceLanguage::addLanguages(std::vector<std::pair<std::string, std::string>> const& languages) {
    for (auto& data : languages) {
        mLanguages[data.first] = data.second;
    }
}

} // namespace GMLIB::Files