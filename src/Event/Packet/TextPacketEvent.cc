#include "Global.h"
#include <GMLIB/Event/Packet/TextPacketEvent.h>

namespace GMLIB::Event::PacketEvent {

ServerNetworkHandler& TextPacketSendBeforeEvent::getServerNetworkHandler() const { return mServerNetworkHandler; }
NetworkIdentifier&    TextPacketSendBeforeEvent::getNetworkIdentifier() const { return mNetworkIdentifier; }
TextPacket&           TextPacketSendBeforeEvent::getPacket() const { return mPacket; }

ServerNetworkHandler& TextPacketSendAfterEvent::getServerNetworkHandler() const { return mServerNetworkHandler; }
NetworkIdentifier&    TextPacketSendAfterEvent::getNetworkIdentifier() const { return mNetworkIdentifier; }
TextPacket&           TextPacketSendAfterEvent::getPacket() const { return mPacket; }

LL_TYPE_INSTANCE_HOOK(
    TextPacketSendHook,
    HookPriority::High,
    ServerNetworkHandler,
    "?handle@ServerNetworkHandler@@UEAAXAEBVNetworkIdentifier@@AEBVTextPacket@@@Z",
    void,
    NetworkIdentifier& identifier,
    TextPacket&        packet
) {
    auto beforeEvent = TextPacketSendBeforeEvent(*this, identifier, packet);
    ll::event::EventBus::getInstance().publish(beforeEvent);
    if (beforeEvent.isCancelled()) {
        return;
    }
    origin(identifier, packet);
    auto afterEvent = TextPacketSendAfterEvent(*this, identifier, packet);
    ll::event::EventBus::getInstance().publish(afterEvent);
}

static std::unique_ptr<ll::event::EmitterBase> emitterFactory1(ll::event::ListenerBase&);
class TextPacketSendBeforeEventEmitter : public ll::event::Emitter<emitterFactory1, TextPacketSendBeforeEvent> {
    ll::memory::HookRegistrar<TextPacketSendHook> hook;
};

static std::unique_ptr<ll::event::EmitterBase> emitterFactory1(ll::event::ListenerBase&) {
    return std::make_unique<TextPacketSendBeforeEventEmitter>();
}

static std::unique_ptr<ll::event::EmitterBase> emitterFactory2(ll::event::ListenerBase&);
class TextPacketSendAfterEventEmitter : public ll::event::Emitter<emitterFactory2, TextPacketSendAfterEvent> {
    ll::memory::HookRegistrar<TextPacketSendHook> hook;
};

static std::unique_ptr<ll::event::EmitterBase> emitterFactory2(ll::event::ListenerBase&) {
    return std::make_unique<TextPacketSendAfterEventEmitter>();
}

TextPacket& TextPacketWriteBeforeEvent::getPacket() const { return mPacket; }

TextPacket& TextPacketWriteAfterEvent::getPacket() const { return mPacket; }

LL_TYPE_INSTANCE_HOOK(
    TextPacketWriteHook,
    HookPriority::High,
    TextPacket,
    "?write@TextPacket@@UEBAXAEAVBinaryStream@@@Z",
    void,
    class BinaryStream& stream
) {
    auto beforeEvent = TextPacketWriteBeforeEvent(*this);
    ll::event::EventBus::getInstance().publish(beforeEvent);
    if (beforeEvent.isCancelled()) {
        return;
    }
    origin(stream);
    auto afterEvent = TextPacketWriteBeforeEvent(*this);
    ll::event::EventBus::getInstance().publish(afterEvent);
}

static std::unique_ptr<ll::event::EmitterBase> emitterFactory3(ll::event::ListenerBase&);
class TextPacketWriteBeforeEventEmitter : public ll::event::Emitter<emitterFactory3, TextPacketWriteBeforeEvent> {
    ll::memory::HookRegistrar<TextPacketWriteHook> hook;
};

static std::unique_ptr<ll::event::EmitterBase> emitterFactory3(ll::event::ListenerBase&) {
    return std::make_unique<TextPacketWriteBeforeEventEmitter>();
}

static std::unique_ptr<ll::event::EmitterBase> emitterFactory4(ll::event::ListenerBase&);
class TextPacketWriteAfterEventEmitter : public ll::event::Emitter<emitterFactory4, TextPacketWriteAfterEvent> {
    ll::memory::HookRegistrar<TextPacketWriteHook> hook;
};

static std::unique_ptr<ll::event::EmitterBase> emitterFactory4(ll::event::ListenerBase&) {
    return std::make_unique<TextPacketWriteAfterEventEmitter>();
}

} // namespace GMLIB::Event::PacketEvent
