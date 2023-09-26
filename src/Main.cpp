#include <LoggerAPI.h>
#include "version.h"
#include "GlobalHeader.h"

void PluginInit()
{
    ExportAPI();
    
    logger.info("GMLib Loaded!");
    logger.info("GMLib - Made by GroupMountain");
    logger.info("Version: {}", PLUGIN_FILE_VERSION_STRING);
}