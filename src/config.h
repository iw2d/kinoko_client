#pragma once

#define CONFIG_WINDOW_NAME      "Kinoko"
#define CONFIG_DLL_NAME         "Kinoko.dll"
#define CONFIG_LOG_FILE         "log.txt"

#define CONFIG_REGISTRY_KEY     "SOFTWARE\\Kinoko"
#define CONFIG_VERSION_STRING   "Ver. 100.1"
// #define CONFIG_IMAGE_LOADING    // comment out to disable image loading

#define CONFIG_SERVER_ADDRESS   "127.0.0.1"
#define CONFIG_NEXON_SEARCH     "8.31.98."
#define CONFIG_NEXON_ADDRESS    "8.31.98.51"

extern char* g_sServerAddress;
extern int g_nServerPort;