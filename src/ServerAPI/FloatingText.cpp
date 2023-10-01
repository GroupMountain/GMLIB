/**
 * @file FloatingText.cpp
 * @author Zhongzi8972
 * @brief 悬浮字相关API
 * 
 * @version 0.1
 * @date 2023-09-30
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#include "../GlobalHeader.h"
#include <GMLib/GMLib_ServerAPI.h>
#include <mc/RemoveActorPacket.hpp>
#include <mc/NetworkItemStackDescriptor.hpp>
#include <mc/ServerPlayer.hpp>
#include <mc/ItemStackDescriptor.hpp>

struct FloatingTextData
{
    int mRuntimeId;
    Vec3 mPos;
    std::string mText;
    xuid_t mXuid;
    int mDimensionId;
};

class ChangeDimensionRequest {
public:
    int mState;
    AutomaticID<Dimension, int> mFromDimensionId;
    AutomaticID<Dimension, int> mToDimensionId;
    Vec3 mPosition;
    bool mUsePortal;
    bool mRespawn;
    std::unique_ptr<CompoundTag> mAgentTag;
};

template <typename T>
inline std::unique_ptr<DataItem2<T>> makeDataItem2(ActorDataIDs id, T flag)
{
    return std::make_unique<DataItem2<T>>((unsigned short)id, flag);
}

std::unordered_map<int, FloatingTextData> dataMap;
std::mutex mtx;

inline int getRandom() {
    std::random_device rd;  // 随机数种子
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> distribution(100000, std::numeric_limits<int>::max());
    return distribution(gen); // 生成随机数
}

//在指定地点放置一次悬浮字
void addFloatingText(FloatingTextData data, Player* pl,bool isBroadCast) {
    std::vector<std::unique_ptr<DataItem>> idataItem;
    idataItem.emplace_back(makeDataItem2<std::string>(ActorDataIDs::NAMETAG, data.mText));
    idataItem.emplace_back(makeDataItem2<signed char>(ActorDataIDs::ALWAYS_SHOW_NAMETAG, true));
    auto is = ItemStack::create("minecraft:air");
    BinaryStream ft;
    ft.writeVarInt64(data.mRuntimeId);
    ft.writeUnsignedVarInt64(data.mRuntimeId);
    ItemStackDescriptor desc(*is->getItem(), is->getAux(), 1, nullptr);
    NetworkItemStackDescriptor netDesc(desc);
    ft.writeType(netDesc);
    ft.writeType(data.mPos);
    ft.writeType(Vec3::ZERO);
    ft.writeType(idataItem);
    ft.writeBool(false);
    NetworkPacket<(int)MinecraftPacketIds::AddItemActor> pkti(ft.getAndReleaseData());
    if (!isBroadCast) {
        if(pl)
            pktSender->sendToClient(pl->getUserEntityIdentifierComponent(), pkti);
    }
    else {
        Global<Level>->forEachPlayer([&](Player& pl) {
            if(pl.getDimensionId()==data.mDimensionId) {
                pktSender->sendToClient(pl.getUserEntityIdentifierComponent(), pkti);
            }
            return true;
        });
    }
}

inline int GMLib_Server::sendAddFloatingTextPacket(std::string text, Vec3 spos, int dimid) {
    if (isServerStarted == false) {
        return -1;
    }
    auto rid = getRandom();
    FloatingTextData data = { 
        rid, 
        spos, 
        text, 
        "", 
        dimid 
    };
    addFloatingText(data, nullptr,true);
    mtx.lock();
    dataMap.insert({ rid,data });
    mtx.unlock();
    return rid;
}

inline int GMLib_Server::sendAddFloatingTextPacketToPlayer(std::string text, Vec3 spos, int dimid, Player* pl) {
    if (isServerStarted == false) {
        return -1;
    }
    auto rid = getRandom();
    FloatingTextData data = { 
        rid,
        spos,
        text,
        pl->getXuid(),
        dimid
    };
    addFloatingText(data, pl,false);
    mtx.lock();
    dataMap.insert({ rid,data});
    mtx.unlock();
    return rid;
}

void GMLib_Server::sendDeleteFloatingTextPacket(int id) {
    if (isServerStarted == false) {
        return;
    }
    auto pkt = MinecraftPackets::createPacket(MinecraftPacketIds::RemoveActor);
    BinaryStream bs;
    bs.writeVarInt64(id);
    pkt->read(bs);
    pktSender->send(*pkt);
    mtx.lock();
    dataMap.erase(id);
    mtx.unlock();
}

void GMLib_Server::sendUpdateFloatingTextPacket(int id, std::string newtext) { //这个包不用管维度，一视同仁发给所有人就行
    if (!dataMap.count(id) || !isServerStarted) {
        return;
    }                  //不存在id
    std::vector<std::unique_ptr<DataItem>> DataItems;
    DataItems.emplace_back(makeDataItem2<std::string>(ActorDataIDs::NAMETAG, newtext));
    DataItems.emplace_back(makeDataItem2<signed char>(ActorDataIDs::ALWAYS_SHOW_NAMETAG, true));
    BinaryStream bs;
    bs.writeUnsignedVarInt64(id);
    bs.writeType(DataItems);
    bs.writeUnsignedVarInt(0);
    bs.writeUnsignedVarInt(0);
    bs.writeUnsignedVarInt64(0);
    auto packet = MinecraftPackets::createPacket(MinecraftPacketIds::SetActorData);
    packet->read(bs);
    mtx.lock();
    auto data = dataMap[id];
    data.mText = newtext;
    if (data.mXuid == "") {                 //全服
        Global<Level>->forEachPlayer([&](Player& pl) {
            if (pl.getDimensionId() == data.mDimensionId) {
                pktSender->sendToClient(pl.getUserEntityIdentifierComponent(), *packet);
            }
            return true;
        });
    }
    else {
        auto pl = Global<Level>->getPlayer(data.mXuid);
        if (pl) {
            pktSender->sendToClient(pl->getUserEntityIdentifierComponent(), *packet);
        }   
    }
    mtx.unlock();
}

void waitDimChange(Player* player, std::unique_ptr<ChangeDimensionRequest> *request,int a) {
    if (player == nullptr) {
        return;
    }
    if (*request == nullptr || !Global<Level>->_playerChangeDimensionWaitingForRespawn(*player, **request)) {
        mtx.lock();
        for (auto it = dataMap.begin(); it != dataMap.end(); it++) {
            if ((it->second.mXuid == "" || it->second.mXuid == player->getXuid()) && it->second.mDimensionId == a) {
                addFloatingText(it->second, player,false);
            }
        }
        mtx.unlock();
        return;
    }
    Schedule::nextTick([request, player, a]() {
        waitDimChange(player, request, a);
    });
}

LL_AUTO_TYPED_INSTANCE_HOOK(                //进服发包
    TextSend,
    ServerPlayer,
    HookPriority::Normal,
    "?setLocalPlayerAsInitialized@ServerPlayer@@QEAAXXZ",
    bool
)
{
    if (!CanLoad) {
        return origin();
    }
    auto uid = this->getXuid();
    mtx.lock();
    for (auto it =dataMap.begin(); it != dataMap.end(); it++) {
        if ((it->second.mXuid == "" || it->second.mXuid == uid) && it->second.mDimensionId == this->getDimensionId()) {
            addFloatingText(it->second, this,false);
        }       
    }
    mtx.unlock();
    return origin();
}

LL_AUTO_TYPED_INSTANCE_HOOK(                //切换维度给包
    DimTextSend,
    Level,
    HookPriority::Normal,
    "?requestPlayerChangeDimension@Level@@UEAAXAEAVPlayer@@V?$unique_ptr@VChangeDimensionRequest@@U?"
    "$default_delete@VChangeDimensionRequest@@@std@@@std@@@Z",
    bool, 
    Player* player, std::unique_ptr<ChangeDimensionRequest> request
)
{
    if (!CanLoad) {
        return origin(player, std::move(request));
    }
    auto a = request->mToDimensionId;
    auto re = &request;
    Schedule::nextTick([re,player, a]() {
        waitDimChange(player, re, a);
    });
    return origin(player, std::move(request));
}