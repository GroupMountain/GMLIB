#include "Entry.h"
#include "Global.h"

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
    return true;
}

bool Entry::disable() { return true; }

LL_REGISTER_PLUGIN(Entry, Entry::getInstance());

} // namespace GMLIB
