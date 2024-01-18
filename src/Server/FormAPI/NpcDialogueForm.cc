#include "Global.h"
#include <GMLIB/Server/ActorAPI.h>
#include <GMLIB/Server/BinaryStreamAPI.h>
#include <GMLIB/Server/FormAPI/NpcDialogueForm.h>
#include <GMLIB/Server/NetworkPacketAPI.h>


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
      "button_name" : "sff",
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
      "button_name" : "sd",
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
      "button_name" : "qa",
      "data" : [
         {
            "cmd_line" : "/hhh",
            "cmd_ver" : 36
         },
         {
            "cmd_line" : "oj",
            "cmd_ver" : 36
         }
      ],
      "mode" : 0,
      "text" : "/hhh\nsb\n",
      "type" : 1
   }
])";

int genRandomNumber() {
    std::random_device                 rd;
    std::mt19937                       gen(rd());
    std::uniform_int_distribution<int> dis(0, 99999999);
    int                                randomNumber = dis(gen);
    return randomNumber;
}

int getNextNpcId() {
    auto result = genRandomNumber();
    while (ll::service::getLevel()->fetchEntity(ActorUniqueID(result))) {
        result = genRandomNumber();
    }
    return result;
}

void sendFakeNpc(Player* pl) {
    auto auid = getNextNpcId();
    logger.warn("{}", auid);
    // DataItem
    // std::vector<std::unique_ptr<DataItem>> npcDataItem;
    // npcDataItem.emplace_back(DataItem::create<schar>(ActorDataIDs::HasNpc, true));
    // npcDataItem.emplace_back(DataItem::create(ActorDataIDs::NpcData, npcData));
    // npcDataItem.emplace_back(DataItem::create(ActorDataIDs::Actions, npcAction));
    // npcDataItem.emplace_back(DataItem::create(ActorDataIDs::InteractText, std::string("傻逼oj")));
    // AddActorPacket
    GMLIB_BinaryStream bs1;
    bs1.writeVarInt64(auid);
    bs1.writeUnsignedVarInt64(auid);
    bs1.writeString("npc");
    bs1.writeVec3(Vec3{pl->getPosition().x, -66.0f, pl->getPosition().z});
    bs1.writeVec3(Vec3{0, 0, 0});
    bs1.writeVec2(Vec2{0, 0});
    bs1.writeFloat(0.0f);
    bs1.writeFloat(0.0f);
    bs1.writeUnsignedVarInt(0);
    // bs1.writeDataItem(npcDataItem);
    bs1.writeUnsignedVarInt(5);
    bs1.writeUnsignedVarInt((uint)0x4);
    bs1.writeUnsignedVarInt((uint)0x4);
    bs1.writeString("测试");
    bs1.writeUnsignedVarInt((uint)0x27);
    bs1.writeUnsignedVarInt((uint)0x0);
    bs1.writeBool(true);
    bs1.writeUnsignedVarInt((uint)0x28);
    bs1.writeUnsignedVarInt((uint)0x4);
    bs1.writeString(npcData);
    bs1.writeUnsignedVarInt((uint)0x29);
    bs1.writeUnsignedVarInt((uint)0x4);
    bs1.writeString(npcAction);
    bs1.writeUnsignedVarInt((uint)0x64);
    bs1.writeUnsignedVarInt((uint)0x4);
    bs1.writeString("傻逼oj");

    bs1.writeUnsignedVarInt(0);
    bs1.writeUnsignedVarInt(0);
    bs1.writeUnsignedVarInt(0);
    GMLIB_NetworkPacket<(int)MinecraftPacketIds::AddActor> pkt1(bs1.getAndReleaseData());
    pkt1.sendTo(*pl);
    logger.warn("send 1");
    // NpcDialoguePacket
    GMLIB_BinaryStream bs2;
    bs2.writeUnsignedInt64(auid);     // ActorUniqueId
    bs2.writeVarInt(0);               // 0: Open  1: Close
    bs2.writeString("1145141919810"); // Dialogue
    bs2.writeString("jbjbjbjbjb");    // SceneName
    bs2.writeString("sbsbsbsbsb");    // NpcName
    bs2.writeString(npcAction);       // ActionJSON
    GMLIB_NetworkPacket<(int)MinecraftPacketIds::NpcDialoguePacket> pkt2(bs2.getAndReleaseData());
    pkt2.sendTo(*pl);
    logger.warn("send 2");
}

#include <GMLIB/Server/PlayerAPI.h>
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


__int64 __fastcall NpcDialoguePacket::write(NpcDialoguePacket *this, struct BinaryStream *a2)
{
  char *v4; // rcx
  __int64 v5; // rax
  char *v6; // rcx
  __int64 v7; // rax
  char *v8; // rcx
  __int64 v9; // rax
  char *v10; // rcx
  __int64 v11; // rax
  char *v13; // [rsp+20h] [rbp-18h] BYREF
  __int64 v14; // [rsp+28h] [rbp-10h]

  BinaryStream::writeUnsignedInt64(a2, *((_QWORD *)this + 6), 0i64, 0i64);
  BinaryStream::writeVarInt(a2, *((_DWORD *)this + 14), 0i64, 0i64);
  v4 = (char *)this + 64;
  if ( *((_QWORD *)this + 11) >= 0x10ui64 )
    v4 = (char *)*((_QWORD *)this + 8);
  v5 = *((_QWORD *)this + 10);
  v13 = v4;
  v14 = v5;
  ((void (__fastcall *)(struct BinaryStream *, char **, _QWORD, _QWORD))BinaryStream::writeString)(a2, &v13, 0i64,
0i64); v6 = (char *)this + 96; if ( *((_QWORD *)this + 15) >= 0x10ui64 ) v6 = (char *)*((_QWORD *)this + 12); v7 =
*((_QWORD *)this + 14); v13 = v6; v14 = v7;
  ((void (__fastcall *)(struct BinaryStream *, char **, _QWORD, _QWORD))BinaryStream::writeString)(a2, &v13, 0i64,
0i64); v8 = (char *)this + 128; if ( *((_QWORD *)this + 19) >= 0x10ui64 ) v8 = (char *)*((_QWORD *)this + 16); v9 =
*((_QWORD *)this + 18); v13 = v8; v14 = v9;
  ((void (__fastcall *)(struct BinaryStream *, char **, _QWORD, _QWORD))BinaryStream::writeString)(a2, &v13, 0i64,
0i64); v10 = (char *)this + 160; if ( *((_QWORD *)this + 23) >= 0x10ui64 ) v10 = (char *)*((_QWORD *)this + 20); v11 =
*((_QWORD *)this + 22); v13 = v10; v14 = v11; return ((__int64 (__fastcall *)(struct BinaryStream *, char **, _QWORD,
_QWORD))BinaryStream::writeString)( a2, &v13, 0i64, 0i64);
}
*/