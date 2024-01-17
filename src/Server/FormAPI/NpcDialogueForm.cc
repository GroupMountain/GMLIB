#include "Global.h"
#include <GMLIB/Server/BinaryStreamAPI.h>
#include <GMLIB/Server/FormAPI/NpcDialogueForm.h>

std::string npcData =
    R"({"picker_offsets":{"scale":[1.70,1.70,1.70],"translate":[0,20,0]},"portrait_offsets":{"scale":[1.750,1.750,1.750],"translate":[-7,50,0]},"skin_list":[{"variant":0},{"variant":1},{"variant":2},{"variant":3},{"variant":4},{"variant":5},{"variant":6},{"variant":7},{"variant":8},{"variant":9},{"variant":10},{"variant":11},{"variant":12},{"variant":13},{"variant":14},{"variant":15},{"variant":16},{"variant":17},{"variant":18},{"variant":19},{"variant":25},{"variant":26},{"variant":27},{"variant":28},{"variant":29},{"variant":30},{"variant":31},{"variant":32},{"variant":33},{"variant":34},{"variant":20},{"variant":21},{"variant":22},{"variant":23},{"variant":24},{"variant":35},{"variant":36},{"variant":37},{"variant":38},{"variant":39},{"variant":40},{"variant":41},{"variant":42},{"variant":43},{"variant":44},{"variant":50},{"variant":51},{"variant":52},{"variant":53},{"variant":54},{"variant":45},{"variant":46},{"variant":47},{"variant":48},{"variant":49},{"variant":55},{"variant":56},{"variant":57},{"variant":58},{"variant":59}]})";

std::string npcAction = R"([
   {
      "button_name" : "button",
      "data" : [
         {
            "cmd_line" : "say test1",
            "cmd_ver" : 36
         }
      ],
      "mode" : 0,
      "text" : "say test1\n",
      "type" : 1
   },
   {
      "button_name" : "",
      "data" : [
         {
            "cmd_line" : "say test2",
            "cmd_ver" : 36
         }
      ],
      "mode" : 2,
      "text" : "say test2\n",
      "type" : 1
   },
   {
      "button_name" : "",
      "data" : [
         {
            "cmd_line" : "say test3",
            "cmd_ver" : 36
         }
      ],
      "mode" : 1,
      "text" : "say test3\n",
      "type" : 1
   },
   {
      "button_name" : "q",
      "data" : [
         {
            "cmd_line" : "/hhh",
            "cmd_ver" : 36
         },
         {
            "cmd_line" : "sb",
            "cmd_ver" : 36
         }
      ],
      "mode" : 0,
      "text" : "/hhh\nsb\n",
      "type" : 1
   }
])";

int64_t nextLong() {
    auto res = ll::service::getLevel()->getRandom().nextLong();
    while (ll::service::getLevel()->fetchEntity(ActorUniqueID(res)) != nullptr) {
        res = ll::service::getLevel()->getRandom().nextLong();
    }
    return res;
}

void sendFakeNpc(Player* pl) {
    auto auid = nextLong();
    // dataItem
    std::vector<std::unique_ptr<DataItem>> npcDataItem;
    npcDataItem.emplace_back(DataItem::create<schar>(ActorDataIDs::HasNpc, true));
    npcDataItem.emplace_back(DataItem::create(ActorDataIDs::NpcData, npcData));
    npcDataItem.emplace_back(DataItem::create(ActorDataIDs::Actions, npcAction));
    npcDataItem.emplace_back(DataItem::create(ActorDataIDs::InteractText, std::string("傻逼")));
    npcDataItem.emplace_back(DataItem::create(ActorDataIDs::Name, std::string("测试")));
    GMLIB_BinaryStream bs1;
    // ActorUniqueID
    bs1.writeVarInt64(auid);
    // ActorRuntimeID
    bs1.writeUnsignedVarInt64(auid + 1145141919810);
    // ActorDefinitionIdentifider
    bs1.writeString("npc");
    // Position  Vec3
    bs1.writeVec3(Vec3{pl->getPosition().x, -66.0f, pl->getPosition().z});
    // Velocity  Vec3
    bs1.writeVec3(Vec3{0, 0, 0});
    // Rot  Vec2
    bs1.writeFloat(0.0f);
    bs1.writeFloat(0.0f);
    // YHeadRotation
    bs1.writeFloat(0.0f);
    // YBodyRotation
    bs1.writeFloat(0.0f);
    // Atrribute
    bs1.writeUnsignedVarInt(0);
    // DataItem
    bs1.writeDataItem(npcDataItem);
    // PropertySyncData
    bs1.writeUnsignedVarInt(0);
    bs1.writeUnsignedVarInt(0);
    // ActorLinks
    bs1.writeUnsignedVarInt(0);
    auto pkt1 = MinecraftPackets::createPacket(MinecraftPacketIds::AddActor);
    pkt1->read(bs1);
    pkt1->sendTo(*pl);
    auto pkt2 = NpcDialoguePacket(ActorUniqueID(auid));
    pkt2.sendTo(*pl);
}


LL_AUTO_INSTANCE_HOOK(
    Test1,
    ll::memory::HookPriority::Normal,
    "?executeCommand@MinecraftCommands@@QEBA?AUMCRESULT@@AEAVCommandContext@@_N@Z",
    void,
    void* a1,
    void* a2,
    bool  a3
) {
    ll::service::getLevel()->forEachPlayer([](Player& pl) -> bool {
        logger.warn("{}", pl.getRealName());
        sendFakeNpc(&pl);
        return true;
    });
    origin(a1, a2, a3);
}

//

/*
putString(Entity.DATA_NAMETAG, dialog.getTitle())
                    .putByte(Entity.DATA_HAS_NPC_COMPONENT, 1)
                    .putString(Entity.DATA_NPC_SKIN_DATA, dialog.getSkinData())
                    .putString(Entity.DATA_NPC_ACTIONS, actionJson)
                    .putString(Entity.DATA_INTERACTIVE_TAG, dialog.getContent());

[
   {
      "button_name" : "button",
      "data" : [
         {
            "cmd_line" : "say test1",
            "cmd_ver" : 36
         }
      ],
      "mode" : 0,
      "text" : "say test1\n",
      "type" : 1
   },
   {
      "button_name" : "",
      "data" : [
         {
            "cmd_line" : "say test2",
            "cmd_ver" : 36
         }
      ],
      "mode" : 2,
      "text" : "say test2\n",
      "type" : 1
   },
   {
      "button_name" : "",
      "data" : [
         {
            "cmd_line" : "say test3",
            "cmd_ver" : 36
         }
      ],
      "mode" : 1,
      "text" : "say test3\n",
      "type" : 1
   },
   {
      "button_name" : "q",
      "data" : [
         {
            "cmd_line" : "/hhh",
            "cmd_ver" : 36
         },
         {
            "cmd_line" : "sb",
            "cmd_ver" : 36
         }
      ],
      "mode" : 0,
      "text" : "/hhh\nsb\n",
      "type" : 1
   }
]

LL_AUTO_TYPE_INSTANCE_HOOK(
    Test1,
    ll::memory::HookPriority::Normal,
    NpcDialoguePacket,
    "?write@NpcDialoguePacket@@UEBAXAEAVBinaryStream@@@Z",
    void,
    BinaryStream& bs
) {
    // logger.warn("{} | {} | {} | {} | {}", this->mActionJSON, this->mDialogue, this->mNpcName, this->mSceneName,
    // (int)this->mNpcDialogueActionType);
    origin(bs);
}

LL_AUTO_TYPE_INSTANCE_HOOK(
    Test2,
    ll::memory::HookPriority::Normal,
    NpcRequestPacket,
    "?write@NpcRequestPacket@@UEBAXAEAVBinaryStream@@@Z",
    void,
    BinaryStream& bs
) {
    // logger.warn("{} | {} | {} | {}", this->mActions, (int)this->mType, this->mSceneName, (int)this->mActionIndex);
    origin(bs);
}

#include <GMLIB/Server/LevelAPI.h>
LL_AUTO_TYPE_INSTANCE_HOOK(
    Test3,
    ll::memory::HookPriority::Normal,
    ServerNetworkHandler,
    "?handle@ServerNetworkHandler@@UEAAXAEBVNetworkIdentifier@@AEBVNpcRequestPacket@@@Z",
    void,
    class NetworkIdentifier const& source,
    class NpcRequestPacket const&  packet
) {
    // logger.warn("{} | {} | {} | {}", packet.mActions, (int)packet.mType, packet.mSceneName,
    // (int)packet.mActionIndex);
    origin(source, packet);
    auto ens = GMLIB_Level::getLevel()->getAllEntities();
    for (auto en : ens) {
        if (en->getTypeName() == "minecraft:npc") {
            logger.error("data {}", en->getEntityData().getString(0x28));
            logger.error("data {}", en->getEntityData().getString(0x29));
        }
    }
}
*/