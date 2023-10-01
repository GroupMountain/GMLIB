#include <LoggerAPI.h>
#include "version.h"
#include "GlobalHeader.h"

LoopbackPacketSender* pktSender = nullptr;
bool isServerStarted = false;

void PluginInit()
{
    ExportAPI();
    logger.info("GMLib Loaded!");
    logger.info("GMLib - Made by GroupMountain");
    logger.info("Version: {}", PLUGIN_FILE_VERSION_STRING);
}

LL_AUTO_INSTANCE_HOOK(
    ServerStartEvent, 
    HookPriority::Normal, 
    "?sendServerThreadStarted@ServerInstanceEventCoordinator@@QEAAXAEAVServerInstance@@@Z", 
    void,
    void* a1)
{
    origin(a1);
    //After Event
    isServerStarted = true;
    pktSender = (LoopbackPacketSender*)(Global<Level>->getPacketSender());
}