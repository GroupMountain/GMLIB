#pragma once
#include "GMLIB/GMLIB.h"
#include "ll/api/event/Cancellable.h"
#include "ll/api/event/Event.h"
#include "mc/deps/core/mce/UUID.h"
#include "mc/network/NetworkIdentifier.h"
#include "mc/network/ServerNetworkHandler.h"

namespace GMLIB::Event::PacketEvent {

class ClientLoginBeforeEvent : public ll::event::Cancellable<ll::event::Event> {
    ServerNetworkHandler const& mServerNetworkHandler;
    NetworkIdentifier const&    mNetworkIdentifier;

public:
    constexpr explicit ClientLoginBeforeEvent(
        ServerNetworkHandler const& serverNetworkHandler,
        NetworkIdentifier const&    networkIdentifier
    )
    : Cancellable(),
      mServerNetworkHandler(serverNetworkHandler),
      mNetworkIdentifier(networkIdentifier) {}

    GMLIB_API ServerNetworkHandler const& getServerNetworkHandler() const;
    GMLIB_API NetworkIdentifier const&    getNetworkIdentifier() const;
};

class ClientLoginAfterEvent : public ll::event::Event {
    ServerNetworkHandler const& mServerNetworkHandler;
    NetworkIdentifier const&    mNetworkIdentifier;
    mce::UUID const             mUuid;
    std::string const           mServerAuthXuid;
    std::string const           mClientAuthXuid;
    std::string const           mRealName;
    std::string const           mIpAndPort;

public:
    constexpr explicit ClientLoginAfterEvent(
        ServerNetworkHandler const& serverNetworkHandler,
        NetworkIdentifier const&    networkIdentifier,
        mce::UUID const&            uuid,
        std::string const           serverAuthXuid,
        std::string const           clientAuthXuid,
        std::string const           realName,
        std::string const           ipAndPort
    )
    : mServerNetworkHandler(serverNetworkHandler),
      mNetworkIdentifier(networkIdentifier),
      mUuid(uuid),
      mServerAuthXuid(serverAuthXuid),
      mClientAuthXuid(clientAuthXuid),
      mRealName(realName),
      mIpAndPort(ipAndPort) {}

    GMLIB_API ServerNetworkHandler const& getServerNetworkHandler() const;
    GMLIB_API NetworkIdentifier const&    getNetworkIdentifier() const;
    GMLIB_API mce::UUID const getUuid() const;
    GMLIB_API std::string const getServerAuthXuid() const;
    GMLIB_API std::string const getClientAuthXuid() const;
    GMLIB_API std::string const getRealName() const;
    GMLIB_API std::string const getIpAndPort() const;
    GMLIB_API std::string const getIp() const;
    GMLIB_API std::string const getPort() const;
    GMLIB_API void              disConnectClient(std::string reason = "") const;
};

} // namespace GMLIB::Event::PacketEvent