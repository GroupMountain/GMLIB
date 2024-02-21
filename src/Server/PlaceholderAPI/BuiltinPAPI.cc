#include "Global.h"
#include "Server/PlaceholderAPI/Helper.h"
#include <GMLIB/Server/LevelAPI.h>
#include <GMLIB/Server/PlaceholderAPI.h>
#include <mc/server/common/PropertiesSettings.h>
#include <mc/world/attribute/AttributeInstance.h>
#include <mc/world/level/LayeredAbilities.h>

namespace GMLIB {

using namespace Server;

void regPlayerPAPI() {

    PlaceholderAPI::registerPlayerPlaceholder("player_realname", [](Player* sp) { return sp->getRealName(); });

    PlaceholderAPI::registerPlayerPlaceholder("player_uuid", [](Player* sp) { return sp->getUuid().asString(); });

    // PlaceholderAPI::registerPlayerPlaceholder("player_ping", [](Player* sp) { return S(sp->getAvgPing()); });

    PlaceholderAPI::registerPlayerPlaceholder("player_health", [](Player* sp) { return S(sp->getHealth()); });

    PlaceholderAPI::registerPlayerPlaceholder("player_max_health", [](Player* sp) { return S(sp->getMaxHealth()); });

    PlaceholderAPI::registerPlayerPlaceholder("player_gamemode", [](Player* sp) {
        return S((int)sp->getPlayerGameType());
    });

    PlaceholderAPI::registerPlayerPlaceholder("player_x", [](Player* sp) { return S(sp->getPosition().x); });

    PlaceholderAPI::registerPlayerPlaceholder("player_y", [](Player* sp) { return S(sp->getPosition().y); });

    PlaceholderAPI::registerPlayerPlaceholder("player_z", [](Player* sp) { return S(sp->getPosition().z); });

    PlaceholderAPI::registerPlayerPlaceholder("player_direction", [](Player* sp) { return S(sp->getDirection()); });

    PlaceholderAPI::registerPlayerPlaceholder("player_exp_to_next", [](Player* sp) {
        return S(sp->getXpNeededForNextLevel());
    });

    PlaceholderAPI::registerPlayerPlaceholder("player_exp_level", [](Player* sp) { return S(sp->getPlayerLevel()); });

    PlaceholderAPI::registerPlayerPlaceholder("player_speed", [](Player* sp) { return S(sp->getSpeed()); });

    PlaceholderAPI::registerPlayerPlaceholder("player_flying", [](Player* sp) {
        auto abi = sp->getAbilities();
        return S(abi.getBool(AbilitiesIndex::Flying));
    });

    PlaceholderAPI::registerPlayerPlaceholder("player_can_fly", [](Player* sp) { return S(sp->canFly()); });

    PlaceholderAPI::registerPlayerPlaceholder("player_displayname", [](Player* sp) { return sp->getNameTag(); });

    PlaceholderAPI::registerPlayerPlaceholder("player_is_op", [](Player* sp) { return S(sp->isOperator()); });

    PlaceholderAPI::registerPlayerPlaceholder("player_item_in_hand", [](Player* sp) {
        auto        item = sp->getCarriedItem();
        std::string str;
        return item.getName();
    });

    PlaceholderAPI::registerPlayerPlaceholder("player_ip", [](Player* sp) { return sp->getIPAndPort(); });

    // PlaceholderAPI::registerPlayerPlaceholder("player_device", [](Player* sp) { return sp->getDeviceName(); });

    // PlaceholderAPI::registerPlayerPlaceholder("player_language", [](Player* sp) { return sp->getLanguageCode(); });

    PlaceholderAPI::registerPlayerPlaceholder("player_xuid", [](Player* sp) { return sp->getXuid(); });

    PlaceholderAPI::registerPlayerPlaceholder("player_bed_x", [](Player* sp) { return S(sp->getSpawnPosition().x); });

    PlaceholderAPI::registerPlayerPlaceholder("player_bed_y", [](Player* sp) { return S(sp->getSpawnPosition().y); });

    PlaceholderAPI::registerPlayerPlaceholder("player_bed_z", [](Player* sp) { return S(sp->getSpawnPosition().z); });

    /*
    PlaceholderAPI::registerPlayerPlaceholder("player_hunger", [](Player* sp) {
        return S((int)sp->getAttribute(Player::HUNGER).getCurrentValue());
    });
    PlaceholderAPI::registerPlayerPlaceholder("player_max_hunger", [](Player* sp) {
        try {
            std::string    maxHunger = "";
            auto           plnbt     = sp->getNbt();
            nlohmann::json plnJ      = nlohmann::json::parse(plnbt.get()->toJson(0));
            for (auto& nbt : plnJ["Attributes"]) {
                if (nbt["Name"] == "minecraft:player.hunger") {
                    maxHunger = std::to_string((double)nbt["Max"]);
                    maxHunger = maxHunger.substr(0, maxHunger.find("."));
                }
            }
            return maxHunger;
        } catch (...) {
            return (std::string) "20";
        }
    });
    */
}

void regServerPAPI() {

    PlaceholderAPI::registerServerPlaceholder(
        "server_time_<format>_s",
        [](std::unordered_map<std::string, std::string> map) {
            if (map.find("<format>") != map.end()) {
                if ("<format>" != map["<format>"]) return Helper::getTime(map["<format>"]);
            }

            return Helper::getTime("H:M:S");
        }
    );
    PlaceholderAPI::registerServerPlaceholder(
        "server_time_<format>",
        [](std::unordered_map<std::string, std::string> map) {
            if (map.find("<format>") != map.end()) {
                if ("<format>" != map["<format>"]) return Helper::getTime(map["<format>"]);
            }

            return Helper::getTime("H:M:S");
        }
    );
    // PlaceholderAPI::registerServerPlaceholder("server_online", []() {
    //     return S(Global<ServerNetworkHandler>->getActiveAndInProgressPlayerCount(mce::UUID::EMPTY));
    // });

    // PlaceholderAPI::registerServerPlaceholder("server_max_players", []() {
    //     return S(*((int*)Global<ServerNetworkHandler> + 192));
    // });

    // PlaceholderAPI::registerServerPlaceholder("server_version", []() { return ll::getBdsVersion(); });
    // PlaceholderAPI::registerServerPlaceholder("server_protocol_version", []() {
    // return S(ll::getServerProtocolVersion());
    //});
    // PlaceholderAPI::registerServerPlaceholder("server_total_entities", []() {
    // return S(Level::getAllEntities().size());
    //});
    PlaceholderAPI::registerServerPlaceholder("server_world_name", []() {
        return ll::service::getLevel()->getLevelData().getLevelName();
    });
    PlaceholderAPI::registerServerPlaceholder("server_difficulty", []() {
        return S((int)ll::service::getPropertiesSettings()->getDifficulty());
    });
    PlaceholderAPI::registerServerPlaceholder("server_on_allowlist", []() {
        return S(ll::service::getPropertiesSettings()->useAllowList());
    });
    PlaceholderAPI::registerServerPlaceholder("server_port", []() {
        return S(ll::service::getPropertiesSettings()->getServerPort());
    });
    PlaceholderAPI::registerServerPlaceholder("server_port_v6", []() {
        return S(ll::service::getPropertiesSettings()->getServerPortv6());
    });

    /*
    PlaceholderAPI::registerServerPlaceholder("server_uptime", []() { return S(std::time(0) - startTime); });
    PlaceholderAPI::registerServerPlaceholder(
        "server_start_time_<format>_s",
        [](std::unordered_map<std::string, std::string> map) {
            if (map.find("<format>") != map.end()) {
                if ("<format>" != map["<format>"]) return Helper::getTime(map["<format>"], startTime);
            }

            return Helper::getTime("H:M:S", startTime);
        }
    );
    PlaceholderAPI::registerServerPlaceholder(
        "server_start_time_<format>",
        [](std::unordered_map<std::string, std::string> map) {
            if (map.find("<format>") != map.end()) {
                if ("<format>" != map["<format>"]) return Helper::getTime(map["<format>"], startTime);
            }

            return Helper::getTime("H:M:S", startTime);
        }
    );
    */

    PlaceholderAPI::registerServerPlaceholder("server_name", []() {
        return ll::service::getPropertiesSettings()->getMotd();
    });
    PlaceholderAPI::registerServerPlaceholder("server_has_whitelist", []() {
        return S(ll::service::getPropertiesSettings()->useAllowList());
    });
    PlaceholderAPI::registerServerPlaceholder("server_total_chunks", []() { return ""; });

    PlaceholderAPI::registerServerPlaceholder("server_ram_bds_used", []() {
        auto ram = Helper::getRam();
        return ram["bdsused"];
    });

    PlaceholderAPI::registerServerPlaceholder("server_ram_free", []() {
        auto ram = Helper::getRam();
        return ram["canuse"];
    });

    PlaceholderAPI::registerServerPlaceholder("server_ram_used", []() {
        auto ram = Helper::getRam();
        return ram["used"];
    });

    PlaceholderAPI::registerServerPlaceholder("server_ram_max", []() {
        auto ram = Helper::getRam();
        return ram["all"];
    });
    PlaceholderAPI::registerServerPlaceholder("server_tps", []() {
        if (GMLIB_Level::getLevel()) {
            return S(GMLIB_Level::getLevel()->getServerCurrentTps());
        }
        return S(0);
    });
    PlaceholderAPI::registerServerPlaceholder("server_mspt", []() {
        if (GMLIB_Level::getLevel()) {
            return S(GMLIB_Level::getLevel()->getServerMspt());
        }
        return S(0);
    });
}

/*
LARGE_INTEGER freq_;
auto          INITPERFORMANCE = QueryPerformanceFrequency(&freq_);

LARGE_INTEGER begin_time;
LARGE_INTEGER end_time;
inline double ns_time() { return (end_time.QuadPart - begin_time.QuadPart) * 1000000.0 / freq_.QuadPart; }

#define TestLogTime(func, ...)                                                                                         \
    QueryPerformanceCounter(&begin_time);                                                                              \
    func(__VA_ARGS__);                                                                                                 \
    QueryPerformanceCounter(&end_time);                                                                                \
    logger.warn("  {}\t time: {}ns", #func, ns_time());
constexpr int TEST_COUNT = 1000000;


void getValue() { PlaceholderAPI::getValue("%server_tps%"); }

void translateString() {
    std::string str = "hi%server_tps%";
    PlaceholderAPI::translateString(str);
}

void translateString2() {
    std::string str =
        "hi%server_test3%%server_test%%server_test_21231%%server_test_21231%%server_test_21231%%server_test_"
        "21231%%server_test_21231%%server_test_21231%%server_test_21231%";
    PlaceholderAPI::translateString(str);
}

void translateString3() {
    std::string str = "hi%server_time_y%%server_tps%";
    PlaceholderAPI::translateString(str);
}
*/

} // namespace GMLIB