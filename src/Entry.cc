#include "Entry.h"
#include "Global.h"

namespace GMLIB {

std::unique_ptr<GMLIB>& GMLIB::getInstance() {
    static std::unique_ptr<GMLIB> instance;
    return instance;
}

bool GMLIB::load() {
    loadLib();
    return true;
}

bool GMLIB::enable() {
    enableLib();
    return true;
}

bool GMLIB::disable() { return false; }

} // namespace GMLIB

LL_REGISTER_PLUGIN(GMLIB::GMLIB, GMLIB::GMLIB::getInstance());