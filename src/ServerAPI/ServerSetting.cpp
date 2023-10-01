/**
 * @file ServerSetting.cpp
 * @author EP
 * @brief ServerSetting相关内容
 * 
 * @version 0.1
 * @date 2023-09-30
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#include "../GlobalHeader.h"
#include <llapi/FormUI.h>
#include <llapi/impl/FormPacketHelper.h>
#include <llapi/mc/ServerNetworkHandler.hpp>
#include <llapi/mc/ServerSettingsResponsePacket.hpp>
#include <llapi/mc/ServerSettingsRequestPacket.hpp>
#include <GMLib/GMLib_ServerAPI.h>

using namespace std;
ServerNetworkHandler* SERVER_NETWORK_HANDLER;
std::optional<Form::CustomForm> fm;

class ServerSetting :Form::CustomForm {
public:
	inline string getSString() {
		return this->serialize();
	}
};

void SendServerSettings(Player* pl, unsigned int fmid, string data) {
    BinaryStream wp;
    wp.reserve(8 + data.size());
    wp.writeUnsignedVarInt(fmid);
    wp.writeString(data);
    auto pkt = ServerSettingsResponsePacket();
    pkt.read(wp);
    pl->sendNetworkPacket(pkt);
}

unsigned int RandomId() {
    return (unsigned)((rand() << 16) + rand());
}

void SettingSendto(Form::CustomForm fm, Player* player, Form::CustomForm::Callback callback) {
    unsigned id = RandomId();
    fm.callback = callback;
    SetCustomFormBuilderData(id, make_shared<Form::CustomForm>(fm));
    string str = ((ServerSetting*)&fm)->getSString();
    if (str.empty()) {
        return;
    }
    SendServerSettings(player, id, str);
}

void GMLib_Server::setServerSetting(Form::CustomForm cfm) {
    fm = cfm;
}

LL_AUTO_TYPED_INSTANCE_HOOK(
    GetHandler,
    ServerNetworkHandler,
    HookPriority::Normal,
    "?allowIncomingPacketId@ServerNetworkHandler@@UEAA_NAEBVNetworkIdentifier@@W4MinecraftPacketIds@@@Z",
    bool,
    __int64 a1,
    signed int a2
)
{
    if (!CanLoad) {
        return origin(a1, a2);
    }
    SERVER_NETWORK_HANDLER = this;
    GetHandler::unhook();
    return origin(a1, a2);
}

LL_AUTO_INSTANCE_HOOK(
    OpenSetting,
    HookPriority::Normal,
    "?handle@?$PacketHandlerDispatcherInstance@VServerSettingsRequestPacket@@$0A@@@UEBAXAEBVNetworkIdentifier@@AEAVNetEventCallback@@AEAV?$shared_ptr@VPacket@@@std@@@Z",
    __int64,
    NetworkIdentifier* a1, void* a2, std::shared_ptr<class ServerSettingsRequestPacket> a3
)
{
    if (!CanLoad) {
        return origin(a1, a2, a3);
    }
    auto pl = (Player*)a3->getPlayerFromPacket(SERVER_NETWORK_HANDLER, a1);
    Schedule::delay([pl]() {
        if (pl) {
            if (fm.has_value()) {
                SettingSendto(fm.value(), pl,fm.value().callback);
            }
        }
    }, 20);
    return origin(a1, a2, a3);
}