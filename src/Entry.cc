#include "Entry.h"
#include "Global.h"

#include "include_all.h"

namespace GMLIB {

std::unique_ptr<Entry>& Entry::getInstance() {
    static std::unique_ptr<Entry> instance;
    return instance;
}

bool Entry::load() {
    loadLib();
    return true;
}

bool Entry::enable() {
    enableLib();
    ll::event::EventBus::getInstance().emplaceListener<ll::event::PlayerChatEvent>([](ll::event::PlayerChatEvent& ev) {
        auto pl = (GMLIB_Player*)&ev.self();
        if (ev.message() == "give1") {
            pl->giveItem("minecraft:totem", 100, 0, true);
        }
        if (ev.message() == "give2") {
            pl->giveItem("minecraft:invisible_bedrock", 100, 0, true);
        }
        if (ev.message() == "give3") {
            pl->giveItem("minecraft:diamond", 100, 0, true);
        }
        if (ev.message() == "clear1") {
            pl->clearItem("minecraft:diamond");
        }
        if (ev.message() == "clear2") {
            pl->clearAllItems();
        }
        if (ev.message() == "check1") {
            pl->hasItem("minecraft:diamond");
        }
        if (ev.message() == "check2") {
            pl->hasItem("minecraft:diamond_helmet");
        }
    });
    return true;
}

bool Entry::disable() { return true; }

} // namespace GMLIB

LL_REGISTER_PLUGIN(GMLIB::Entry, GMLIB::Entry::getInstance());