#include "Global.h"
#include "Server/PlaceholderAPI/Helper.h"
#include <GMLIB/Server/LevelAPI.h>
#include <GMLIB/Server/PlaceholderAPI.h>
#include <GMLIB/Server/PlayerAPI.h>
#include <mc/server/common/PropertiesSettings.h>
#include <mc/world/attribute/Attribute.h>
#include <mc/world/attribute/AttributeInstance.h>
#include <mc/world/level/LayeredAbilities.h>

namespace GMLIB {

using namespace Server;

std::time_t startTime = 0;

void regPlayerPAPI() {

    PlaceholderAPI::registerPlayerPlaceholder("player_realname", [](Player* sp) { return sp->getRealName(); });

    PlaceholderAPI::registerPlayerPlaceholder("player_name", [](Player* sp) { return sp->getName(); });

    PlaceholderAPI::registerPlayerPlaceholder("player_uuid", [](Player* sp) { return sp->getUuid().asString(); });

    PlaceholderAPI::registerPlayerPlaceholder("player_ping", [](Player* sp) {
        auto pl = (GMLIB_Player*)sp;
        return S(pl->getAvgPing());
    });

    PlaceholderAPI::registerPlayerPlaceholder("player_health", [](Player* sp) { return S(sp->getHealth()); });

    PlaceholderAPI::registerPlayerPlaceholder("player_max_health", [](Player* sp) { return S(sp->getMaxHealth()); });

    PlaceholderAPI::registerPlayerPlaceholder("player_gamemode", [](Player* sp) {
        auto type = magic_enum::enum_name(sp->getPlayerGameType());
        return std::string(type);
    });

    PlaceholderAPI::registerPlayerPlaceholder("player_x", [](Player* sp) { return S(sp->getPosition().x); });

    PlaceholderAPI::registerPlayerPlaceholder("player_y", [](Player* sp) { return S(sp->getPosition().y); });

    PlaceholderAPI::registerPlayerPlaceholder("player_z", [](Player* sp) { return S(sp->getPosition().z); });

    PlaceholderAPI::registerPlayerPlaceholder("player_pos", [](Player* sp) { return sp->getPosition().toString(); });

    PlaceholderAPI::registerPlayerPlaceholder("player_dimid", [](Player* sp) { return S(sp->getDimensionId()); });

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
        auto item = sp->getCarriedItem();
        return item.getName();
    });

    PlaceholderAPI::registerPlayerPlaceholder("player_ip", [](Player* sp) { return sp->getIPAndPort(); });

    PlaceholderAPI::registerPlayerPlaceholder("player_device", [](Player* sp) {
        auto device = magic_enum::enum_name(sp->getPlatform());
        return std::string(device);
    });

    PlaceholderAPI::registerPlayerPlaceholder("player_language", [](Player* sp) {
        auto pl = (GMLIB_Player*)sp;
        return std::string(pl->getLanguageCode());
    });

    PlaceholderAPI::registerPlayerPlaceholder("player_xuid", [](Player* sp) { return sp->getXuid(); });

    PlaceholderAPI::registerPlayerPlaceholder("player_bed_x", [](Player* sp) { return S(sp->getSpawnPosition().x); });

    PlaceholderAPI::registerPlayerPlaceholder("player_bed_y", [](Player* sp) { return S(sp->getSpawnPosition().y); });

    PlaceholderAPI::registerPlayerPlaceholder("player_bed_z", [](Player* sp) { return S(sp->getSpawnPosition().z); });

    PlaceholderAPI::registerPlayerPlaceholder("player_bed_pos", [](Player* sp) {
        return sp->getSpawnPosition().toString();
    });

    PlaceholderAPI::registerPlayerPlaceholder("player_hunger", [](Player* sp) {
        return S((int)sp->getAttribute(Player::HUNGER).getCurrentValue());
    });

    PlaceholderAPI::registerPlayerPlaceholder("player_max_hunger", [](Player* sp) {
        return S((int)sp->getAttribute(Player::HUNGER).getMaxValue());
    });
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

    PlaceholderAPI::registerServerPlaceholder("server_online", []() {
        return S(ll::service::getServerNetworkHandler()->_getActiveAndInProgressPlayerCount(mce::UUID::EMPTY));
    });

    PlaceholderAPI::registerServerPlaceholder("server_max_players", []() {
        return S(*((int*)ll::service::getServerNetworkHandler().as_ptr() + 192));
    });

    PlaceholderAPI::registerServerPlaceholder("server_version", []() { return GMLIB::Version::getBdsVersion(); });

    PlaceholderAPI::registerServerPlaceholder("server_protocol_version", []() {
        return S(GMLIB::Version::getProtocolVersion());
    });

    PlaceholderAPI::registerServerPlaceholder("server_total_entities", []() {
        return S(GMLIB_Level::getLevel()->getAllEntities().size());
    });

    PlaceholderAPI::registerServerPlaceholder("server_world_name", []() {
        return ll::service::getLevel()->getLevelData().getLevelName();
    });
    PlaceholderAPI::registerServerPlaceholder("server_difficulty", []() {
        auto res = magic_enum::enum_name(ll::service::getPropertiesSettings()->getDifficulty());
        return std::string(res);
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

} // namespace GMLIB