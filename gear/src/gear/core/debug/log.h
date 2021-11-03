#pragma once

#include "log_functions.h"

#if defined(GEAR_DEBUG)

#define GEAR_DEBUG_LOG(...)                  gear::log(__VA_ARGS__)
#define GEAR_DEBUG_LOG_SET_OUTPUT(flags)     gear::log_Set_Output(flags)
#define GEAR_DEBUG_LOG_OPEN_FILE(filename)   gear::log_Open_File(filename)
#define GEAR_DEBUG_LOG_CLOSE_FILE()          gear::log_Close_File()
#else
#define GEAR_DEBUG_LOG(...)
#define GEAR_DEBUG_LOG_SET_OUTPUT(flags)
#define GEAR_DEBUG_LOG_OPEN_FILE(filename)
#define GEAR_DEBUG_LOG_CLOSE_FILE()
#endif