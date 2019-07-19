#include "lua.hpp"

#define newindex(name,func) \
	lua_pushstring(L, name); \
	lua_pushcfunction(L, func); \
	lua_rawset(L, -3); \

#define index(tn,name) lua_pushstring(L,name); lua_rawget(L,tn);

#define CheckWheelOnArg1 \
	if (!LogiUpdate()) luaL_error(L, "Not available"); \
	int index = luaL_checkinteger(L, 1); \
	index--; \
	if (!LogiIsDeviceConnected(index, LOGI_DEVICE_TYPE_WHEEL)) luaL_error(L, "%s", "No device connected at the given index or it's not a wheel."); \

extern "C" { EXPORT int luaopen_luawheel(lua_State* L); }

static const char* NameToString(int enm) {
	switch (enm) {
	case LOGI_MODEL_G27: return "Logitech G27"; break;
	case LOGI_MODEL_DRIVING_FORCE_GT: return "Logitech Driving Force GT"; break;
	case LOGI_MODEL_G25: return "Logitech G25"; break;
	case LOGI_MODEL_MOMO_RACING: return "Logitech MOMO Racing"; break;
	case LOGI_MODEL_MOMO_FORCE: return "Logitech MOMO Racing Force Feedback Wheel"; break;
	case LOGI_MODEL_DRIVING_FORCE_PRO: return "Logitech Driving Force Pro"; break;
	case LOGI_MODEL_DRIVING_FORCE: return "Logitech Driving Force"; break;
	case LOGI_MODEL_NASCAR_RACING_WHEEL: return "Logitech NASCAR Racing Wheel"; break;
	case LOGI_MODEL_FORMULA_FORCE: return "Logitech Formula Force Feedback Wheel"; break;
	case LOGI_MODEL_FORMULA_FORCE_GP: return "Logitech Formula Force GP"; break;
	case LOGI_MODEL_FORCE_3D_PRO: return "Logitech WingMan Force 3D"; break;
	case LOGI_MODEL_EXTREME_3D_PRO: return "Logitech Extreme 3D Pro"; break;
	case LOGI_MODEL_FREEDOM_24: return "Logitech Freedom 2.4 Cordless"; break;
	case LOGI_MODEL_ATTACK_3: return "Logitech Attack 3"; break;
	case LOGI_MODEL_FORCE_3D: return "Logitech WingMan Force 3D"; break;
	case LOGI_MODEL_STRIKE_FORCE_3D: return "Logitech WingMan Strike Force 3D"; break;
	case LOGI_MODEL_G940_JOYSTICK: return "Logitech Flight System G940 (Joystick)"; break;
	case LOGI_MODEL_G940_THROTTLE: return "Logitech Flight System G940 (Throttle)"; break;
	case LOGI_MODEL_G940_PEDALS: return "Logitech Flight System G940 (Rudder Pedals)"; break;
	case LOGI_MODEL_RUMBLEPAD: return "Logitech WingMan RumblePad"; break;
	case LOGI_MODEL_RUMBLEPAD_2: return "Logitech Rumblepad 2"; break;
	case LOGI_MODEL_CORDLESS_RUMBLEPAD_2: return "Logitech Cordless Rumblepad 2"; break;
	case LOGI_MODEL_CORDLESS_GAMEPAD: return "Logitech WingMan Cordless RumblePad"; break;
	case LOGI_MODEL_DUAL_ACTION_GAMEPAD: return "Logitech Dual Action Gamepad"; break;
	case LOGI_MODEL_PRECISION_GAMEPAD_2: return "Logitech Precision Gamepad 2"; break;
	case LOGI_MODEL_CHILLSTREAM: return "Logitech ChillStream Controller"; break;
	case LOGI_MODEL_G29: return "Logitech G29 Driving Force"; break;
	case LOGI_MODEL_G920: return "Logitech G920 Driving Force"; break;
	default: return "Unknown model"; break;
	}
}

static const char* ModelToString(int enm) {
	switch (enm) {
	case LOGI_MODEL_G27: return "G27"; break;
	case LOGI_MODEL_DRIVING_FORCE_GT: return "DRIVING_FORCE_GT"; break;
	case LOGI_MODEL_G25: return "G25"; break;
	case LOGI_MODEL_MOMO_RACING: return "MOMO_RACING"; break;
	case LOGI_MODEL_MOMO_FORCE: return "MOMO_FORCE"; break;
	case LOGI_MODEL_DRIVING_FORCE_PRO: return "DRIVING_FORCE_PRO"; break;
	case LOGI_MODEL_DRIVING_FORCE: return "DRIVING_FORCE"; break;
	case LOGI_MODEL_NASCAR_RACING_WHEEL: return "NASCAR_RACING_WHEEL"; break;
	case LOGI_MODEL_FORMULA_FORCE: return "FORMULA_FORCE"; break;
	case LOGI_MODEL_FORMULA_FORCE_GP: return "FORMULA_FORCE_GP"; break;
	case LOGI_MODEL_FORCE_3D_PRO: return "FORCE_3D_PRO"; break;
	case LOGI_MODEL_EXTREME_3D_PRO: return "EXTREME_3D_PRO"; break;
	case LOGI_MODEL_FREEDOM_24: return "FREEDOM_24"; break;
	case LOGI_MODEL_ATTACK_3: return "ATTACK_3"; break;
	case LOGI_MODEL_FORCE_3D: return "FORCE_3D"; break;
	case LOGI_MODEL_STRIKE_FORCE_3D: return "STRIKE_FORCE_3D"; break;
	case LOGI_MODEL_G940_JOYSTICK: return "G940_JOYSTICK"; break;
	case LOGI_MODEL_G940_THROTTLE: return "G940_THROTTLE"; break;
	case LOGI_MODEL_G940_PEDALS: return "G940_PEDALS"; break;
	case LOGI_MODEL_RUMBLEPAD: return "RUMBLEPAD"; break;
	case LOGI_MODEL_RUMBLEPAD_2: return "RUMBLEPAD_2"; break;
	case LOGI_MODEL_CORDLESS_RUMBLEPAD_2: return "CORDLESS_RUMBLEPAD_2"; break;
	case LOGI_MODEL_CORDLESS_GAMEPAD: return "CORDLESS_GAMEPAD"; break;
	case LOGI_MODEL_DUAL_ACTION_GAMEPAD: return "DUAL_ACTION_GAMEPAD"; break;
	case LOGI_MODEL_PRECISION_GAMEPAD_2: return "PRECISION_GAMEPAD_2"; break;
	case LOGI_MODEL_CHILLSTREAM: return "CHILLSTREAM"; break;
	case LOGI_MODEL_G29: return "G29"; break;
	case LOGI_MODEL_G920: return "G920"; break;
	default: return "UNKNOWN"; break;
	}
}

static const char* TypeToString(int enm) {
	switch (enm) {
	case LOGI_DEVICE_TYPE_NONE: return "NONE"; break;
	case LOGI_DEVICE_TYPE_WHEEL: return "WHEEL"; break;
	case LOGI_DEVICE_TYPE_JOYSTICK: return "JOYSTICK"; break;
	case LOGI_DEVICE_TYPE_GAMEPAD: return "GAMEPAD"; break;
	case LOGI_DEVICE_TYPE_OTHER: return "OTHER"; break;
	default: return "UNKNOWN"; break;
	}
}

static const char* ManfToString(int enm) {
	switch (enm) {
	case LOGI_MANUFACTURER_NONE: return "NONE"; break;
	case LOGI_MANUFACTURER_LOGITECH: return "LOGITECH"; break;
	case LOGI_MANUFACTURER_MICROSOFT: return "MICROSOFT"; break;
	case LOGI_MANUFACTURER_OTHER: return "OTHER"; break;
	default: return "UNKNOWN"; break;
	}
}

static const char* ForceTypeToString(int enm) {
	switch (enm) {
	case LOGI_FORCE_NONE: return "NONE"; break;
	case LOGI_FORCE_SPRING: return "SPRING"; break;
	case LOGI_FORCE_CONSTANT: return "CONSTANT"; break;
	case LOGI_FORCE_DAMPER: return "DAMPER"; break;
	case LOGI_FORCE_SIDE_COLLISION: return "SIDE_COLLISION"; break;
	case LOGI_FORCE_FRONTAL_COLLISION: return "FRONTAL_COLLISION"; break;
	case LOGI_FORCE_DIRT_ROAD: return "DIRT_ROAD"; break;
	case LOGI_FORCE_BUMPY_ROAD: return "BUMPY_ROAD"; break;
	case LOGI_FORCE_SLIPPERY_ROAD: return "SLIPPERY_ROAD"; break;
	case LOGI_FORCE_SURFACE_EFFECT: return "SURFACE_EFFECT"; break;
	case LOGI_FORCE_SOFTSTOP: return "SOFTSTOP"; break;
	case LOGI_FORCE_CAR_AIRBORNE: return "CAR_AIRBORNE"; break;
	default: return "UNKNOWN"; break;
	}
}

static const char* PeriodicTypeToString(int enm) {
	switch (enm) {
	case LOGI_PERIODICTYPE_NONE: return "NONE"; break;
	case LOGI_PERIODICTYPE_SINE: return "SINE"; break;
	case LOGI_PERIODICTYPE_SQUARE: return "SQUARE"; break;
	case LOGI_PERIODICTYPE_TRIANGLE: return "TRIANGLE"; break;
	default: return "UNKNOWN"; break;
	}
}