#pragma once
#include "GMLIB/GMLIB.h"
#include "ll/api/event/Cancellable.h"
#include "mc/network/packet/TextPacket.h"

namespace GMLIB::Event::PacketEvent {

class TextPacketSendBeforeEvent : public ll::event::Cancellable<ll::event::Event> {
    ServerNetworkHandler const& mServerNetworkHandler;
    NetworkIdentifier const&    mNetworkIdentifier;
    TextPacket&                 mPacket;

public:
    constexpr explicit TextPacketSendBeforeEvent(
        ServerNetworkHandler const& serverNetworkHandler,
        NetworkIdentifier const&    networkIdentifier,
        TextPacket&                 packet
    )
    : Cancellable(),
      mServerNetworkHandler(serverNetworkHandler),
      mNetworkIdentifier(networkIdentifier),
      mPacket(packet) {}

    GMLIB_API ServerNetworkHandler const& getServerNetworkHandler() const;
    GMLIB_API NetworkIdentifier const&    getNetworkIdentifier() const;
    GMLIB_API TextPacket const&           getPacket() const;
    GMLIB_API TextPacket&                 getPacket();
};

class TextPacketSendAfterEvent : public ll::event::Event {
    ServerNetworkHandler const& mServerNetworkHandler;
    NetworkIdentifier const&    mNetworkIdentifier;
    TextPacket const&           mPacket;

public:
    constexpr explicit TextPacketSendAfterEvent(
        ServerNetworkHandler const& serverNetworkHandler,
        NetworkIdentifier const&    networkIdentifier,
        TextPacket const&           packet
    )
    : mServerNetworkHandler(serverNetworkHandler),
      mNetworkIdentifier(networkIdentifier),
      mPacket(packet) {}

    GMLIB_API ServerNetworkHandler const& getServerNetworkHandler() const;
    GMLIB_API NetworkIdentifier const&    getNetworkIdentifier() const;
    GMLIB_API TextPacket const&           getPacket() const;
};

class TextPacketWriteBeforeEvent : public ll::event::Cancellable<ll::event::Event> {
    TextPacket& mPacket;

public:
    constexpr explicit TextPacketWriteBeforeEvent(TextPacket& packet) : Cancellable(), mPacket(packet) {}

    GMLIB_API TextPacket const& getPacket() const;
    GMLIB_API TextPacket&       getPacket();
};

class TextPacketWriteAfterEvent : public ll::event::Event {
    TextPacket const& mPacket;

public:
    constexpr explicit TextPacketWriteAfterEvent(TextPacket const& packet) : mPacket(packet) {}

    GMLIB_API TextPacket const& getPacket() const;
};

} // namespace GMLIB::Event::PacketEvent