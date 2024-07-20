#include "Global.h"
#include <GMLIB/Event/Player/HandleRequestActionEvent.h>
#include <mc/world/inventory/network/ItemStackRequestActionHandler.h>

namespace GMLIB::Event::PlayerEvent {

class ItemStackRequestAction& HandleRequestActionBeforeEvent::getRequestAction() const { return mRequestAction; }

class ItemStackRequestAction const& HandleRequestActionAfterEvent::getRequestAction() const { return mRequestAction; }

LL_TYPE_INSTANCE_HOOK(
    HandleRequestActionEventHook,
    HookPriority::Normal,
    ItemStackRequestActionHandler,
    "?handleRequestAction@ItemStackRequestActionHandler@@QEAA?AW4ItemStackNetResult@@AEBVItemStackRequestAction@@@Z",
    ::ItemStackNetResult,
    class ItemStackRequestAction& requestAction
) {
    HandleRequestActionBeforeEvent beforeEvent = HandleRequestActionBeforeEvent(mPlayer, requestAction);
    ll::event::EventBus::getInstance().publish(beforeEvent);
    if (beforeEvent.isCancelled()) {
        return ItemStackNetResult::Error;
    }
    auto                          result     = origin(requestAction);
    HandleRequestActionAfterEvent afterEvent = HandleRequestActionAfterEvent(mPlayer, requestAction);
    ll::event::EventBus::getInstance().publish(afterEvent);
    return result;
}

static std::unique_ptr<ll::event::EmitterBase> emitterFactory1(ll::event::ListenerBase&);
class HandleRequestActionBeforeEventEmitter
: public ll::event::Emitter<emitterFactory1, HandleRequestActionBeforeEvent> {
    ll::memory::HookRegistrar<HandleRequestActionEventHook> hook;
};

static std::unique_ptr<ll::event::EmitterBase> emitterFactory1(ll::event::ListenerBase&) {
    return std::make_unique<HandleRequestActionBeforeEventEmitter>();
}

static std::unique_ptr<ll::event::EmitterBase> emitterFactory2(ll::event::ListenerBase&);
class HandleRequestActionAfterEventEmitter : public ll::event::Emitter<emitterFactory2, HandleRequestActionAfterEvent> {
    ll::memory::HookRegistrar<HandleRequestActionEventHook> hook;
};

static std::unique_ptr<ll::event::EmitterBase> emitterFactory2(ll::event::ListenerBase&) {
    return std::make_unique<HandleRequestActionAfterEventEmitter>();
}

} // namespace GMLIB::Event::PlayerEvent