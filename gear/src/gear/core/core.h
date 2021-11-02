#pragma once

#define _GEAR_START namespace gear {
#define _GEAR_END }

#define TYPE_PUN(var, type) (*(type*)(&var))
#define TYPE_PUN_POINTER(var, type) ((type*)(&var))