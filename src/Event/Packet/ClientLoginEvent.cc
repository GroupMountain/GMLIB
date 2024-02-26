#include "Global.h"
#include <GMLIB/Event/Packet/ClientLoginEvent.h>
#include <mc/network/packet/LoginPacket.h>

namespace GMLIB::Event::PacketEvent {

ServerNetworkHandler const& ClientLoginBeforeEvent::getServerNetworkHandler() const { return mServerNetworkHandler; }
NetworkIdentifier const&    ClientLoginBeforeEvent::getNetworkIdentifier() const { return mNetworkIdentifier; }

ServerNetworkHandler const& ClientLoginAfterEvent::getServerNetworkHandler() const { return mServerNetworkHandler; }
NetworkIdentifier const&    ClientLoginAfterEvent::getNetworkIdentifier() const { return mNetworkIdentifier; }
mce::UUID const             ClientLoginAfterEvent::getUuid() const { return mUuid; }
std::string const           ClientLoginAfterEvent::getServerAuthXuid() const { return mServerAuthXuid; }
std::string const           ClientLoginAfterEvent::getClientAuthXuid() const { return mClientAuthXuid; }
std::string const           ClientLoginAfterEvent::getRealName() const { return mRealName; }
std::string const           ClientLoginAfterEvent::getIpAndPort() const { return mIpAndPort; }

std::string const ClientLoginAfterEvent::getIp() const {
    auto ipAndPort = getIpAndPort();
    auto pos       = ipAndPort.find(":");
    return ipAndPort.substr(0, pos);
}

std::string const ClientLoginAfterEvent::getPort() const {
    auto ipAndPort = getIpAndPort();
    auto pos       = ipAndPort.find(":");
    return ipAndPort.substr(pos + 1);
}

void ClientLoginAfterEvent::disConnectClient(std::string reason) const {
    ll::service::getServerNetworkHandler()
        ->disconnectClient(getNetworkIdentifier(), Connection::DisconnectFailReason::Kicked, reason, false);
}

LL_TYPE_INSTANCE_HOOK(
    PlayerLoginEventHook,
    ll::memory::HookPriority::Normal,
    ServerNetworkHandler,
    "?handle@ServerNetworkHandler@@UEAAXAEBVNetworkIdentifier@@AEBVLoginPacket@@@Z",
    void,
    class NetworkIdentifier const& source,
    class LoginPacket const&       packet
) {
    auto beforeEvent = ClientLoginBeforeEvent(*this, source);
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
    auto afterEvent = ClientLoginAfterEvent(*this, source, uuid, serverAuthXuid, clientAuthXuid, realName, ipAndPort);
    ll::event::EventBus::getInstance().publish(afterEvent);
}

static std::unique_ptr<ll::event::EmitterBase> emitterFactory1(ll::event::ListenerBase&);
class ClientLoginBeforeEventEmitter : public ll::event::Emitter<emitterFactory1, ClientLoginBeforeEvent> {
    ll::memory::HookRegistrar<PlayerLoginEventHook> hook;
};

static std::unique_ptr<ll::event::EmitterBase> emitterFactory1(ll::event::ListenerBase&) {
    return std::make_unique<ClientLoginBeforeEventEmitter>();
}

static std::unique_ptr<ll::event::EmitterBase> emitterFactory2(ll::event::ListenerBase&);
class ClientLoginAfterEventEmitter : public ll::event::Emitter<emitterFactory2, ClientLoginAfterEvent> {
    ll::memory::HookRegistrar<PlayerLoginEventHook> hook;
};

static std::unique_ptr<ll::event::EmitterBase> emitterFactory2(ll::event::ListenerBase&) {
    return std::make_unique<ClientLoginAfterEventEmitter>();
}

} // namespace GMLIB::Event::PacketEvent