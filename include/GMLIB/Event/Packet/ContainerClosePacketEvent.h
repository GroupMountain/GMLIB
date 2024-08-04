#pragma once
#include "GMLIB/Macros.h"
#include "ll/api/event/Cancellable.h"
#include "mc/network/ServerNetworkHandler.h"
#include "mc/network/packet/ContainerClosePacket.h"

namespace GMLIB::Event::PacketEvent {

class ContainerClosePacketSendBeforeEvent final : public ll::event::Cancellable<ll::event::Event> {
protected:
    ServerNetworkHandler& mServerNetworkHandler;
    NetworkIdentifier&    mNetworkIdentifier;
    ContainerClosePacket& mPacket;

public:
    constexpr explicit ContainerClosePacketSendBeforeEvent(
        ServerNetworkHandler& serverNetworkHandler,
        NetworkIdentifier&    networkIdentifier,
        ContainerClosePacket& packet
    )
    : Cancellable(),
      mServerNetworkHandler(serverNetworkHandler),
      mNetworkIdentifier(networkIdentifier),
      mPacket(packet) {}

    GMLIB_API ServerNetworkHandler& getServerNetworkHandler() const;
    GMLIB_API NetworkIdentifier&    getNetworkIdentifier() const;
    GMLIB_API ContainerClosePacket& getPacket() const;
};

class ContainerClosePacketSendAfterEvent final : public ll::event::Event {
protected:
    ServerNetworkHandler& mServerNetworkHandler;
    NetworkIdentifier&    mNetworkIdentifier;
    ContainerClosePacket& mPacket;

public:
    constexpr explicit ContainerClosePacketSendAfterEvent(
        ServerNetworkHandler& serverNetworkHandler,
        NetworkIdentifier&    networkIdentifier,
        ContainerClosePacket& packet
    )
    : mServerNetworkHandler(serverNetworkHandler),
      mNetworkIdentifier(networkIdentifier),
      mPacket(packet) {}

    GMLIB_API ServerNetworkHandler& getServerNetworkHandler() const;
    GMLIB_API NetworkIdentifier&    getNetworkIdentifier() const;
    GMLIB_API ContainerClosePacket& getPacket() const;
};

class ContainerClosePacketWriteBeforeEvent final : public ll::event::Cancellable<ll::event::Event> {
protected:
    ContainerClosePacket& mPacket;

public:
    constexpr explicit ContainerClosePacketWriteBeforeEvent(ContainerClosePacket& packet)
    : Cancellable(),
      mPacket(packet) {}

    GMLIB_API ContainerClosePacket& getPacket() const;
};

class ContainerClosePacketWriteAfterEvent final : public ll::event::Event {
protected:
    ContainerClosePacket& mPacket;

public:
    constexpr explicit ContainerClosePacketWriteAfterEvent(ContainerClosePacket& packet) : mPacket(packet) {}

    GMLIB_API ContainerClosePacket& getPacket() const;
};

} // namespace GMLIB::Event::PacketEvent