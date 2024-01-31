#include "Global.h"
#include <GMLIB/Event/Player/PlayerLoginEvent.h>

namespace GMLIB::Event::PlayerEvent {

ServerNetworkHandler const& PlayerLoginBeforeEvent::getServerNetworkHandler() const { return mServerNetworkHandler; }
NetworkIdentifier const&    PlayerLoginBeforeEvent::getNetworkIdentifier() const { return mNetworkIdentifier; }

ServerNetworkHandler const& PlayerLoginAfterEvent::getServerNetworkHandler() const { return mServerNetworkHandler; }
NetworkIdentifier const&    PlayerLoginAfterEvent::getNetworkIdentifier() const { return mNetworkIdentifier; }
mce::UUID const&            PlayerLoginAfterEvent::getUuid() const { return mUuid; }
std::string const&          PlayerLoginAfterEvent::getServerAuthXuid() const { return mServerAuthXuid; }
std::string const&          PlayerLoginAfterEvent::getClientAuthXuid() const { return mClientAuthXuid; }
std::string const&          PlayerLoginAfterEvent::getRealName() const { return mRealName; }
std::string const&          PlayerLoginAfterEvent::getIpAndPort() const { return mIpAndPort; }

std::string const& PlayerLoginAfterEvent::getIp() const {
    auto ipAndPort = getIpAndPort();
    auto pos       = ipAndPort.find(":");
    return ipAndPort.substr(0, pos);
}

std::string const& PlayerLoginAfterEvent::getPort() const {
    auto ipAndPort = getIpAndPort();
    auto pos       = ipAndPort.find(":");
    return ipAndPort.substr(pos + 1);
}

void PlayerLoginAfterEvent::disConnectClient(std::string reason) const {
    ll::service::getServerNetworkHandler()
        ->disconnectClient(getNetworkIdentifier(), Connection::DisconnectFailReason::Kicked, reason, false);
}

LL_AUTO_TYPE_INSTANCE_HOOK(
    PlayerLoginHook,
    ll::memory::HookPriority::Normal,
    ServerNetworkHandler,
    "?handle@ServerNetworkHandler@@UEAAXAEBVNetworkIdentifier@@AEBVLoginPacket@@@Z",
    void,
    class NetworkIdentifier const& source,
    class LoginPacket const&       packet
) {
    auto beforeEvent = PlayerLoginBeforeEvent(*this, source);
    ll::event::EventBus::getInstance().publish(beforeEvent);
    if (beforeEvent.isCancelled()) {
        return;
    }
    origin(source, packet);
    auto cert           = packet.mConnectionRequest->getCertificate();
    auto uuid           = ExtendedCertificate::getIdentity(*cert);
    auto serverAuthXuid = ExtendedCertificate::getXuid(*cert, false);
    auto clientAuthXuid = ExtendedCertificate::getXuid(*cert, true);
    auto realName       = ExtendedCertificate::getIdentityName(*cert);
    auto ipAndPort      = source.getIPAndPort();
    auto afterEvent = PlayerLoginAfterEvent(*this, source, uuid, serverAuthXuid, clientAuthXuid, realName, ipAndPort);
    ll::event::EventBus::getInstance().publish(afterEvent);
}


} // namespace GMLIB::Event::PlayerEvent