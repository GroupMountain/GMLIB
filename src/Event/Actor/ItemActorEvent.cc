#include "Global.h"
#include <GMLIB/Event/Entity/ItemActorEvent.h>

namespace GMLIB::Event::EntityEvent {

ItemActor& ItemActorEvent::getItemActor() const { return static_cast<ItemActor&>(ActorEvent::self()); }

} // namespace GMLIB::Event::EntityEvent
