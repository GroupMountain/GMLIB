#include "Global.h"
#include <GMLIB/Server/ActorAPI.h>
#include <GMLIB/Server/BinaryStreamAPI.h>
#include <GMLIB/Server/FormAPI/NpcDialogueForm.h>
#include <mc/entity/utilities/ActorDataIDs.h>
#include <mc/enums/DataItemType.h>
#include <mc/network/MinecraftPackets.h>
#include <mc/network/packet/AddActorPacket.h>
#include <mc/network/packet/NpcDialoguePacket.h>
#include <mc/network/packet/NpcRequestPacket.h>
#include <mc/network/packet/RemoveActorPacket.h>
#include <mc/util/Random.h>

namespace GMLIB::Server::Form {

std::unordered_map<uint64, NpcDialogueForm*> mRuntimeNpcFormList;

int genRandomNumber() { return Random::getThreadLocal().nextInt(0, 99999999); }

int getNextNpcId() {
    auto result = genRandomNumber();
    while (ll::service::getLevel()->fetchEntity(ActorUniqueID(result))) {
        result = genRandomNumber();
    }
    return result;
}

std::string npcData =
    R"({"picker_offsets":{"scale":[1.70,1.70,1.70],"translate":[0,20,0]},"portrait_offsets":{"scale":[1.750,1.750,1.750],"translate":[-7,50,0]},"skin_list":[{"variant":0},{"variant":1},{"variant":2},{"variant":3},{"variant":4},{"variant":5},{"variant":6},{"variant":7},{"variant":8},{"variant":9},{"variant":10},{"variant":11},{"variant":12},{"variant":13},{"variant":14},{"variant":15},{"variant":16},{"variant":17},{"variant":18},{"variant":19},{"variant":25},{"variant":26},{"variant":27},{"variant":28},{"variant":29},{"variant":30},{"variant":31},{"variant":32},{"variant":33},{"variant":34},{"variant":20},{"variant":21},{"variant":22},{"variant":23},{"variant":24},{"variant":35},{"variant":36},{"variant":37},{"variant":38},{"variant":39},{"variant":40},{"variant":41},{"variant":42},{"variant":43},{"variant":44},{"variant":50},{"variant":51},{"variant":52},{"variant":53},{"variant":54},{"variant":45},{"variant":46},{"variant":47},{"variant":48},{"variant":49},{"variant":55},{"variant":56},{"variant":57},{"variant":58},{"variant":59}]})";

NpcDialogueForm::NpcDialogueForm(std::string const& npcName, std::string const& sceneName, std::string const& dialogue)
: mNpcName(npcName),
  mSceneName(sceneName),
  mDialogue(dialogue) {
    mActionJSON    = nlohmann::ordered_json::array();
    mFormRuntimeId = getNextNpcId();
}

NpcDialogueForm::~NpcDialogueForm() {
    auto pkt = RemoveActorPacket(ActorUniqueID(mFormRuntimeId));
    pkt.sendToClients();
    mRuntimeNpcFormList.erase(mFormRuntimeId);
}

int NpcDialogueForm::addAction(
    std::string const&              name,
    NpcDialogueFormAction           type,
    std::vector<std::string> const& cmds
) {
    std::string                         text;
    std::vector<nlohmann::ordered_json> data;
    for (auto cmd : cmds) {
        text = text + cmd + "\n";
        data.push_back({
            {"cmd_line", cmd},
            {"cmd_ver",  36 }
        });
    }
    nlohmann::ordered_json json;
    json["button_name"] = name;
    json["data"]        = data;
    json["mode"]        = (int)type;
    json["text"]        = text;
    json["type"]        = 1;
    mActionJSON.push_back(json);
    return mActionJSON.size() - 1;
}

void NpcDialogueForm::sendTo(
    Player*                                                                     pl,
    std::function<void(Player* pl, int id, NpcRequestPacket::RequestType type)> callback
) {
    auto               actionJson = mActionJSON.dump(4);
    GMLIB_BinaryStream bs1;
    bs1.writePacketHeader(MinecraftPacketIds::AddActor, pl->getClientSubId());
    bs1.writeVarInt64(mFormRuntimeId);
    bs1.writeUnsignedVarInt64(mFormRuntimeId);
    bs1.writeString("npc");
    bs1.writeVec3(Vec3{pl->getPosition().x, -66.0f, pl->getPosition().z});
    bs1.writeVec3(Vec3{0, 0, 0});
    bs1.writeVec2(Vec2{0, 0});
    bs1.writeFloat(0.0f);
    bs1.writeFloat(0.0f);
    bs1.writeUnsignedVarInt(0);
    // DataItem
    bs1.writeUnsignedVarInt(5);
    bs1.writeUnsignedVarInt((uint)ActorDataIDs::Name);
    bs1.writeUnsignedVarInt((uint)DataItemType::String);
    bs1.writeString("GMLIB-NpcDialogueForm");
    bs1.writeUnsignedVarInt((uint)ActorDataIDs::HasNpc);
    bs1.writeUnsignedVarInt((uint)DataItemType::Byte);
    bs1.writeBool(true);
    bs1.writeUnsignedVarInt((uint)ActorDataIDs::NpcData);
    bs1.writeUnsignedVarInt((uint)DataItemType::String);
    bs1.writeString(npcData);
    bs1.writeUnsignedVarInt((uint)ActorDataIDs::Actions);
    bs1.writeUnsignedVarInt((uint)DataItemType::String);
    bs1.writeString(actionJson);
    bs1.writeUnsignedVarInt((uint)ActorDataIDs::InteractText);
    bs1.writeUnsignedVarInt((uint)DataItemType::String);
    bs1.writeString("GMLIB-NpcDialogueForm");
    bs1.writeUnsignedVarInt(0);
    bs1.writeUnsignedVarInt(0);
    bs1.writeUnsignedVarInt(0);
    bs1.sendTo(*pl);
    // NpcDialoguePacket
    GMLIB_BinaryStream bs2;
    bs2.writePacketHeader(MinecraftPacketIds::NpcDialogue, pl->getClientSubId());
    bs2.writeUnsignedInt64(mFormRuntimeId); // ActorUniqueId
    bs2.writeVarInt(0);                     // 0: Open  1: Close
    bs2.writeString(mDialogue);             // Dialogue
    bs2.writeString(mSceneName);            // SceneName
    bs2.writeString(mNpcName);              // NpcName
    bs2.writeString(actionJson);            // ActionJSON
    bs2.sendTo(*pl);
    mRuntimeNpcFormList[mFormRuntimeId] = this;
    mCallback                           = callback;
}

LL_AUTO_TYPE_INSTANCE_HOOK(
    Test3,
    ll::memory::HookPriority::Normal,
    ServerNetworkHandler,
    "?handle@ServerNetworkHandler@@UEAAXAEBVNetworkIdentifier@@AEBVNpcRequestPacket@@@Z",
    void,
    class NetworkIdentifier const& source,
    class NpcRequestPacket const&  packet
) {
    auto runtimeId = packet.mId.id;
    if (mRuntimeNpcFormList.count(runtimeId)) {
        auto pl = (Player*)this->getServerPlayer(source, packet.mClientSubId).as_ptr();
        auto fm = mRuntimeNpcFormList[runtimeId];
        if (pl && fm) {
            auto type = (int)packet.mType;
            if (type >= 3 && type <= 5) {
                return;
            }
            fm->mCallback(pl, (int)packet.mActionIndex, packet.mType);
            if (type >= 0 && type <= 2) {
                delete fm;
            }
        }
    }
    return origin(source, packet);
}

} // namespace GMLIB::Server::Form