#pragma once
#include "GMLIB/Macros.h"
#include "ll/api/event/Cancellable.h"
#include "mc/network/packet/TextPacket.h"

namespace GMLIB::Event::PacketEvent {

class TextPacketSendBeforeEvent final : public ll::event::Cancellable<ll::event::Event> {
protected:
    ServerNetworkHandler& mServerNetworkHandler;
    NetworkIdentifier&    mNetworkIdentifier;
    TextPacket&           mPacket;

public:
    constexpr explicit TextPacketSendBeforeEvent(
        ServerNetworkHandler& serverNetworkHandler,
        NetworkIdentifier&    networkIdentifier,
        TextPacket&           packet
    )
    : Cancellable(),
      mServerNetworkHandler(serverNetworkHandler),
      mNetworkIdentifier(networkIdentifier),
      mPacket(packet) {}

    GMLIB_API ServerNetworkHandler& getServerNetworkHandler() const;
    GMLIB_API NetworkIdentifier&    getNetworkIdentifier() const;
    GMLIB_API TextPacket&           getPacket() const;
};

class TextPacketSendAfterEvent final : public ll::event::Event {
protected:
    ServerNetworkHandler& mServerNetworkHandler;
    NetworkIdentifier&    mNetworkIdentifier;
    TextPacket&           mPacket;

public:
    constexpr explicit TextPacketSendAfterEvent(
        ServerNetworkHandler& serverNetworkHandler,
        NetworkIdentifier&    networkIdentifier,
        TextPacket&           packet
    )
    : mServerNetworkHandler(serverNetworkHandler),
      mNetworkIdentifier(networkIdentifier),
      mPacket(packet) {}

    GMLIB_API ServerNetworkHandler& getServerNetworkHandler() const;
    GMLIB_API NetworkIdentifier&    getNetworkIdentifier() const;
    GMLIB_API TextPacket&           getPacket() const;
};

class TextPacketWriteBeforeEvent final : public ll::event::Cancellable<ll::event::Event> {
protected:
    TextPacket& mPacket;

public:
    constexpr explicit TextPacketWriteBeforeEvent(TextPacket& packet) : Cancellable(), mPacket(packet) {}

    GMLIB_API TextPacket& getPacket() const;
};

class TextPacketWriteAfterEvent final : public ll::event::Event {
protected:
    TextPacket& mPacket;

public:
    constexpr explicit TextPacketWriteAfterEvent(TextPacket& packet) : mPacket(packet) {}

    GMLIB_API TextPacket& getPacket() const;
};

} // namespace GMLIB::Event::PacketEvent