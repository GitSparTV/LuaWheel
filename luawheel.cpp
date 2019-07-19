#include "luawheel.h"
#include <iostream>
#define LUAWHEEL_VERSION "1.0.0"

static int LuaWheel_GetVersion(lua_State* L)
{
	lua_pushstring(L, LUAWHEEL_VERSION);
	return 1;
}

static int LuaWheel_GetDevices(lua_State* L)
{
	if (!LogiUpdate()) luaL_error(L, "Not available");
	lua_newtable(L);
	for (int index = 0; index < LOGI_MAX_CONTROLLERS + 1; index++)
	{
		if (LogiIsConnected(index)) {
			for (int i = 0; i < LOGI_NUMBER_MODELS; i++)
			{
				if (LogiIsDeviceConnected(index, LOGI_DEVICE_TYPE_WHEEL))
				{
					if (LogiIsModelConnected(index, i)) { lua_pushstring(L, NameToString(i)); lua_rawseti(L, -2, index + 1); break; }
				}
				else
				{
					if (LogiIsModelConnected(index, i)) { lua_pushfstring(L, "%s (Not a wheel)", NameToString(i)); lua_rawseti(L, -2, index + 1); break; }
				}
			}
		}
		else {
			lua_pushstring(L, "Empty Slot");
			lua_rawseti(L, -2, index + 1);
		}
	}
	return 1;
}

static int LuaWheel_IsDeviceConnected(lua_State* L)
{
	if (!LogiUpdate()) luaL_error(L, "Not available");
	int index = luaL_checkinteger(L, 1);
	index--;
	lua_pushboolean(L, LogiIsConnected(index));
	return 1;
}

static int LuaWheel_GetDeviceType(lua_State* L)
{
	if (!LogiUpdate()) luaL_error(L, "Not available");
	int index = luaL_checkinteger(L, 1);
	index--;
	if (!LogiIsConnected(index)) { lua_pushboolean(L, false); return 1; }
	if (!lua_isnoneornil(L, 2)) {
		luaL_argcheck(L, lua_isnumber(L, 2), 2, "number or nil or none expected");
		int type = lua_tonumber(L, 2);
		lua_pushboolean(L, LogiIsDeviceConnected(index, type - 2));
		return 1;
	}
	for (int i = -1; i < LOGI_NUMBER_DEVICE_TYPES; i++) {
		if (LogiIsDeviceConnected(index, i)) { lua_pushinteger(L, i + 2); return 1; }
	}
	lua_pushboolean(L, false);

	return 1;
}

static int LuaWheel_GetDeviceModel(lua_State* L)
{
	if (!LogiUpdate()) luaL_error(L, "Not available");
	int index = luaL_checkinteger(L, 1);
	index--;
	if (!LogiIsConnected(index)) { lua_pushboolean(L, false); return 1; }
	if (!lua_isnoneornil(L, 2)) {
		luaL_argcheck(L, lua_isnumber(L, 2), 2, "number or nil or none expected");
		int type = lua_tonumber(L, 2);
		lua_pushboolean(L, LogiIsModelConnected(index, type - 1));
		return 1;
	}
	for (int i = 0; i < LOGI_NUMBER_MODELS; i++) {
		if (LogiIsModelConnected(index, i)) { lua_pushinteger(L, i + 1); return 1; }
	}
	lua_pushboolean(L, false);

	return 1;
}

static int LuaWheel_DeviceModelToName(lua_State* L)
{
	int deviceModel = luaL_checkinteger(L, 1);
	lua_pushstring(L, NameToString(deviceModel - 1));
	return 1;
}

static int LuaWheel_DeviceManufacturerToName(lua_State* L)
{
	int deviceManufacturer = luaL_checkinteger(L, 1);
	lua_pushstring(L, ManfToString(deviceManufacturer - 2));
	return 1;
}

static int LuaWheel_DeviceTypeToName(lua_State* L)
{
	int deviceType = luaL_checkinteger(L, 1);
	lua_pushstring(L, TypeToString(deviceType - 2));
	return 1;
}

static int LuaWheel_ForceTypesToName(lua_State* L)
{
	int forceTypes = luaL_checkinteger(L, 1);
	lua_pushstring(L, ForceTypeToString(forceTypes - 1));
	return 1;
}



static int LuaWheel_GetDeviceManufacturer(lua_State* L)
{
	if (!LogiUpdate()) luaL_error(L, "Not available");
	int index = luaL_checkinteger(L, 1);
	index--;
	if (!LogiIsConnected(index)) { lua_pushboolean(L, false); return 1; }
	if (!lua_isnoneornil(L, 2)) {
		luaL_argcheck(L, lua_isnumber(L, 2), 2, "number or nil or none expected");
		int type = lua_tonumber(L, 2);
		lua_pushboolean(L, LogiIsManufacturerConnected(index, type - 2));
		return 1;
	}
	for (int i = -1; i < 3; i++) {
		if (LogiIsManufacturerConnected(index, i)) { lua_pushinteger(L, i + 2); return 1; }
	}
	lua_pushboolean(L, false);

	return 1;
}

static int LuaWheel_GetWheelProperties(lua_State* L)
{
	CheckWheelOnArg1
		lua_newtable(L);
	LogiControllerPropertiesData propertiesData_;
	LogiGetCurrentControllerProperties(index, propertiesData_);

	lua_pushstring(L, "forceEnable");
	lua_pushboolean(L, propertiesData_.forceEnable);
	lua_rawset(L, -3);
	lua_pushstring(L, "overallGain");
	lua_pushinteger(L, propertiesData_.overallGain);
	lua_rawset(L, -3);
	lua_pushstring(L, "springGain");
	lua_pushinteger(L, propertiesData_.springGain);
	lua_rawset(L, -3);
	lua_pushstring(L, "damperGain");
	lua_pushinteger(L, propertiesData_.damperGain);
	lua_rawset(L, -3);
	lua_pushstring(L, "defaultSpringEnabled");
	lua_pushboolean(L, propertiesData_.defaultSpringEnabled);
	lua_rawset(L, -3);
	lua_pushstring(L, "defaultSpringGain");
	lua_pushinteger(L, propertiesData_.defaultSpringGain);
	lua_rawset(L, -3);
	lua_pushstring(L, "combinePedals");
	lua_pushboolean(L, propertiesData_.combinePedals);
	lua_rawset(L, -3);
	lua_pushstring(L, "wheelRange");
	lua_pushinteger(L, propertiesData_.wheelRange);
	lua_rawset(L, -3);
	lua_pushstring(L, "gameSettingsEnabled");
	lua_pushboolean(L, propertiesData_.gameSettingsEnabled);
	lua_rawset(L, -3);
	lua_pushstring(L, "allowGameSettings");
	lua_pushboolean(L, propertiesData_.allowGameSettings);
	lua_rawset(L, -3);

	return 1;
}

static int LuaWheel_SetPreferredProperties(lua_State* L)
{
	if (!LogiUpdate()) luaL_error(L, "Not available");
	luaL_checktype(L, 1, LUA_TTABLE);
	if (lua_gettop(L) > 1) lua_pop(L, lua_gettop(L) - 2);	

	index(1, "forceEnable");
	luaL_checktype(L, 2, LUA_TBOOLEAN);
	bool forceEnable = lua_toboolean(L, 2);

	index(1, "overallGain");
	int overallGain = luaL_checkinteger(L, 3);

	index(1, "springGain");
	int springGain = luaL_checkinteger(L, 4);

	index(1, "damperGain");
	int damperGain = luaL_checkinteger(L, 5);

	index(1, "defaultSpringEnabled");
	luaL_checktype(L, 6, LUA_TBOOLEAN);
	bool defaultSpringEnabled = lua_toboolean(L, 6);

	index(1, "defaultSpringGain");
	int defaultSpringGain = luaL_checkinteger(L, 7);

	index(1, "combinePedals");
	luaL_checktype(L, 8, LUA_TBOOLEAN);
	bool combinePedals = lua_toboolean(L, 8);

	index(1, "wheelRange");
	int wheelRange = luaL_checkinteger(L, 9);

	index(1, "gameSettingsEnabled");
	luaL_checktype(L, 10, LUA_TBOOLEAN);
	bool gameSettingsEnabled = lua_toboolean(L, 10);

	index(1, "allowGameSettings");
	luaL_checktype(L, 11, LUA_TBOOLEAN);
	bool allowGameSettings = lua_toboolean(L, 11);

	LogiControllerPropertiesData propertiesData = {
		forceEnable,
		overallGain,
		springGain,
		damperGain,
		defaultSpringEnabled,
		defaultSpringGain,
		combinePedals,
		wheelRange,
		gameSettingsEnabled,
		allowGameSettings,
	};

	lua_pushboolean(L, LogiSetPreferredControllerProperties(propertiesData));
	return 1;
}

static int LuaWheel_GetWheelData(lua_State* L)
{
	CheckWheelOnArg1
		lua_newtable(L);
	DIJOYSTATE2* dj;

	dj = LogiGetState(index);

	lua_pushinteger(L, dj->lX);
	lua_setfield(L, -2, "lX");
	lua_pushinteger(L, dj->lY);
	lua_setfield(L, -2, "lY");
	lua_pushinteger(L, dj->lZ);
	lua_setfield(L, -2, "lZ");
	lua_pushinteger(L, dj->lRx);
	lua_setfield(L, -2, "lRx");
	lua_pushinteger(L, dj->lRy);
	lua_setfield(L, -2, "lRy");
	lua_pushinteger(L, dj->lRz);
	lua_setfield(L, -2, "lRz");
	lua_pushinteger(L, dj->lVX);
	lua_setfield(L, -2, "lVX");
	lua_pushinteger(L, dj->lVY);
	lua_setfield(L, -2, "lVY");
	lua_pushinteger(L, dj->lVZ);
	lua_setfield(L, -2, "lVZ");
	lua_pushinteger(L, dj->lVRx);
	lua_setfield(L, -2, "lVRx");
	lua_pushinteger(L, dj->lVRy);
	lua_setfield(L, -2, "lVRy");
	lua_pushinteger(L, dj->lVRz);
	lua_setfield(L, -2, "lVRz");
	lua_pushinteger(L, dj->lAX);
	lua_setfield(L, -2, "lAX");
	lua_pushinteger(L, dj->lAY);
	lua_setfield(L, -2, "lAY");
	lua_pushinteger(L, dj->lAZ);
	lua_setfield(L, -2, "lAZ");
	lua_pushinteger(L, dj->lARx);
	lua_setfield(L, -2, "lARx");
	lua_pushinteger(L, dj->lARy);
	lua_setfield(L, -2, "lARy");
	lua_pushinteger(L, dj->lARz);
	lua_setfield(L, -2, "lARz");
	lua_pushinteger(L, dj->lFX);
	lua_setfield(L, -2, "lFX");
	lua_pushinteger(L, dj->lFY);
	lua_setfield(L, -2, "lFY");
	lua_pushinteger(L, dj->lFZ);
	lua_setfield(L, -2, "lFZ");
	lua_pushinteger(L, dj->lFRx);
	lua_setfield(L, -2, "lFRx");
	lua_pushinteger(L, dj->lFRy);
	lua_setfield(L, -2, "lFRy");
	lua_pushinteger(L, dj->lFRz);
	lua_setfield(L, -2, "lFRz");

	lua_createtable(L, 128, 0);
	for (int i = 0; i < 128; i++) {
		lua_pushinteger(L, dj->rgbButtons[i]);
		lua_rawseti(L, -2, i + 1);
	}
	lua_setfield(L, -2, "rgbButtons");

	lua_createtable(L, 2, 0);
	for (int i = 0; i < 2; i++) {
		lua_pushinteger(L, dj->rglFSlider[i]);
		lua_rawseti(L, -2, i + 1);
	}
	lua_setfield(L, -2, "rglFSlider");

	lua_createtable(L, 2, 0);
	for (int i = 0; i < 2; i++) {
		lua_pushinteger(L, dj->rglVSlider[i]);
		lua_rawseti(L, -2, i + 1);
	}
	lua_setfield(L, -2, "rglVSlider");

	lua_createtable(L, 2, 0);
	for (int i = 0; i < 2; i++) {
		lua_pushinteger(L, dj->rglASlider[i]);
		lua_rawseti(L, -2, i + 1);
	}
	lua_setfield(L, -2, "rglASlider");

	lua_createtable(L, 2, 0);
	for (int i = 0; i < 2; i++) {
		lua_pushinteger(L, dj->rglSlider[i]);
		lua_rawseti(L, -2, i + 1);
	}
	lua_setfield(L, -2, "rglSlider");

	lua_createtable(L, 4, 0);
	for (int i = 0; i < 4; i++) {
		lua_pushinteger(L, dj->rgdwPOV[i]);
		lua_rawseti(L, -2, i + 1);
	}
	lua_setfield(L, -2, "rgdwPOV");

	return 1;
}

// Get Wheel Position
static int LuaWheel_GetWheelPosition(lua_State* L)
{
	CheckWheelOnArg1
		DIJOYSTATE2* dj;

	dj = LogiGetState(index);

	lua_pushinteger(L, dj->lX);

	return 1;
}

// Get Wheel Accelerator
static int LuaWheel_GetWheelAccelerator(lua_State* L)
{
	CheckWheelOnArg1
		DIJOYSTATE2* dj;

	dj = LogiGetState(index);

	lua_pushinteger(L, dj->lY);

	return 1;
}

// Get Wheel Brake
static int LuaWheel_GetWheelBrake(lua_State* L)
{
	CheckWheelOnArg1
		DIJOYSTATE2* dj;

	dj = LogiGetState(index);

	lua_pushinteger(L, dj->lRz);

	return 1;
}

static int LuaWheel_GetWheelControls(lua_State* L)
{
	CheckWheelOnArg1
		DIJOYSTATE2* dj;

	dj = LogiGetState(index);

	lua_pushinteger(L, dj->lX);
	lua_pushinteger(L, dj->lY);
	lua_pushinteger(L, dj->lRz);

	return 3;
}

// Get Wheel Buttons
static int LuaWheel_GetWheelButtons(lua_State* L)
{
	CheckWheelOnArg1
		DIJOYSTATE2* dj;

	dj = LogiGetState(index);

	lua_createtable(L, 128, 0);
	for (int i = 0; i < 128; i++) {
		int val = dj->rgbButtons[i];
		if (val == 0) continue;
		lua_pushboolean(L, val == 128 ? 1 : 0);
		lua_rawseti(L, -2, i + 1);
	}

	return 1;
}

static int LuaWheel_SetWheelLED(lua_State* L)
{
	CheckWheelOnArg1
		float RPM = luaL_checknumber(L, 2);
	float led1 = luaL_checknumber(L, 3);
	float led2 = luaL_checknumber(L, 4);
	lua_pushboolean(L, LogiPlayLeds(index, RPM, led1, led2));
	return 1;
}

static int LuaWheel_IsButtonTriggered(lua_State* L)
{
	CheckWheelOnArg1
		int buttonNbr = luaL_checkinteger(L, 2);

	lua_pushboolean(L, LogiButtonTriggered(index, buttonNbr - 1));
	return 1;
}

static int LuaWheel_IsButtonReleased(lua_State* L)
{
	CheckWheelOnArg1
		int buttonNbr = luaL_checkinteger(L, 2);

	lua_pushboolean(L, LogiButtonReleased(index, buttonNbr - 1));
	return 1;
}

static int LuaWheel_IsButtonPressed(lua_State* L)
{
	CheckWheelOnArg1
		int buttonNbr = luaL_checkinteger(L, 2);

	lua_pushboolean(L, LogiButtonIsPressed(index, buttonNbr - 1));
	return 1;
}

static int LuaWheel_SetWheelNonLinear(lua_State* L)
{
	CheckWheelOnArg1
		int val = luaL_checkinteger(L, 2);
	lua_pushboolean(L, LogiGenerateNonLinearValues(index, val));
	return 1;
}

static int LuaWheel_ToWheelNonLinear(lua_State* L)
{
	CheckWheelOnArg1
		int val = luaL_checkinteger(L, 2);

	lua_pushinteger(L, LogiGetNonLinearValue(index, val));
	return 1;
}

static int LuaWheel_GetWheelForceFeedback(lua_State* L)
{
	CheckWheelOnArg1

		lua_pushboolean(L, LogiHasForceFeedback(index));

	return 1;
}

static int LuaWheel_GetWheelForceType(lua_State* L)
{
	CheckWheelOnArg1
		if (!lua_isnoneornil(L, 2)) {
			luaL_argcheck(L, lua_isnumber(L, 2), 2, "number or nil or none expected");
			int type = lua_tonumber(L, 2);
			lua_pushboolean(L, LogiIsPlaying(index, type - 1));
			return 1;
		}
	lua_newtable(L);
	int n = 0;
	for (int i = 0; i < 12; i++) {
		if (i == LOGI_NUMBER_FORCE_EFFECTS) continue;
		if (LogiIsPlaying(index, i)) { lua_pushinteger(L, i + 1); lua_rawseti(L, -2, n + 1);  return 1; }
	}
	lua_pushboolean(L, false);

	return 1;
}

static int LuaWheel_SetWheelForceType(lua_State* L)
{
	CheckWheelOnArg1
		int forceType = luaL_checkinteger(L, 2);
	luaL_checktype(L, 3, LUA_TBOOLEAN);
	bool IO = lua_toboolean(L, 3);

	switch (forceType - 1) {
	case LOGI_FORCE_SPRING:
		if (IO) {
			int offsetPercentage = luaL_checkinteger(L, 4);
			int saturationPercentage = luaL_checkinteger(L, 5);
			int coefficientPercentage = luaL_checkinteger(L, 6);
			lua_pushboolean(L, LogiPlaySpringForce(index, offsetPercentage, saturationPercentage, coefficientPercentage));
		}
		else {
			lua_pushboolean(L, LogiStopSpringForce(index));
		}
		break;
	case LOGI_FORCE_CONSTANT:
		if (IO) {
			int magnitudePercentage = luaL_checkinteger(L, 4);
			lua_pushboolean(L, LogiPlayConstantForce(index, magnitudePercentage));
		}
		else {
			lua_pushboolean(L, LogiStopConstantForce(index));
		}
		break;
	case LOGI_FORCE_DAMPER:
		if (IO) {
			int coefficientPercentage = luaL_checkinteger(L, 4);
			lua_pushboolean(L, LogiPlayDamperForce(index, coefficientPercentage));
		}
		else {
			lua_pushboolean(L, LogiStopDamperForce(index));
		}
		break;
	case LOGI_FORCE_SIDE_COLLISION:
		if (IO) {
			int magnitudePercentage = luaL_checkinteger(L, 4);
			lua_pushboolean(L, LogiPlaySideCollisionForce(index, magnitudePercentage));
		}
		else {
			lua_pushboolean(L, LogiPlaySideCollisionForce(index, 0));
		}
		break;
	case LOGI_FORCE_FRONTAL_COLLISION:
		if (IO) {
			int magnitudePercentage = luaL_checkinteger(L, 4);
			lua_pushboolean(L, LogiPlayFrontalCollisionForce(index, magnitudePercentage));
		}
		else {
			lua_pushboolean(L, LogiPlayFrontalCollisionForce(index, 0));
		}
		break;
	case LOGI_FORCE_DIRT_ROAD:
		if (IO) {
			int magnitudePercentage = luaL_checkinteger(L, 4);
			lua_pushboolean(L, LogiPlayDirtRoadEffect(index, magnitudePercentage));
		}
		else {
			lua_pushboolean(L, LogiStopDirtRoadEffect(index));
		}
		break;
	case LOGI_FORCE_BUMPY_ROAD:
		if (IO) {
			int magnitudePercentage = luaL_checkinteger(L, 4);
			lua_pushboolean(L, LogiPlayBumpyRoadEffect(index, magnitudePercentage));
		}
		else {
			lua_pushboolean(L, LogiStopBumpyRoadEffect(index));
		}
		break;
	case LOGI_FORCE_SLIPPERY_ROAD:
		if (IO) {
			int magnitudePercentage = luaL_checkinteger(L, 4);
			lua_pushboolean(L, LogiPlaySlipperyRoadEffect(index, magnitudePercentage));
		}
		else {
			lua_pushboolean(L, LogiStopSlipperyRoadEffect(index));
		}
		break;
	case LOGI_FORCE_SURFACE_EFFECT:
		if (IO) {
			int type = luaL_checkinteger(L, 4);
			int magnitudePercentage = luaL_checkinteger(L, 5);
			int period = luaL_checkinteger(L, 6);
			if (period > 150) period = 150;
			lua_pushboolean(L, LogiPlaySurfaceEffect(index, type - 2, magnitudePercentage, period));
		}
		else {
			lua_pushboolean(L, LogiStopSurfaceEffect(index));
		}
		break;
	case LOGI_FORCE_SOFTSTOP:
		if (IO) {
			int usableRangePercentage = luaL_checkinteger(L, 4);
			lua_pushboolean(L, LogiPlaySoftstopForce(index, usableRangePercentage));
		}
		else {
			lua_pushboolean(L, LogiStopSoftstopForce(index));
		}
		break;
	case LOGI_FORCE_CAR_AIRBORNE:
		if (IO) {
			lua_pushboolean(L, LogiPlayCarAirborne(index));
		}
		else {
			lua_pushboolean(L, LogiStopCarAirborne(index));
		}
		break;
	default: lua_pushboolean(L, false); break;
	}
	return 1;
}

static int LuaWheel_GetWheelShiterMode(lua_State* L)
{
	CheckWheelOnArg1
		lua_pushinteger(L, LogiGetShifterMode(index));
	return 1;
}

static int LuaWheel_GetWheelOperationRange(lua_State* L)
{
	CheckWheelOnArg1
		int result = -1;
	bool s = LogiGetOperatingRange(index, result);
	if (!s) { lua_pushboolean(L, false); }
	else { lua_pushinteger(L, result); }
	return 1;
}

static int LuaWheel_SetWheelOperationRange(lua_State* L)
{
	CheckWheelOnArg1
		int range = luaL_checkinteger(L, 2);
	lua_pushboolean(L, LogiSetOperatingRange(index, range));
	return 1;
}

static int LuaWheel_Release(lua_State* L)
{
	LogiSteeringShutdown();
	return 0;
}

static int LuaWheel_IsAvailable(lua_State* L)
{
	lua_pushboolean(L, LogiUpdate());
	return 1;
}


int EXPORT luaopen_luawheel(lua_State* L) {
	LogiSteeringInitialize(false);
	LogiUpdate();
	luaL_newmetatable(L, "LuaWheel");
	newindex("__gc", LuaWheel_Release)
	lua_pushstring(L, "__index");
	luaL_getmetatable(L, "LuaWheel");
	lua_rawset(L, -3);

		lua_newtable(L);

	newindex("GetVersion", LuaWheel_GetVersion)
		newindex("IsAvailable", LuaWheel_IsAvailable)
		newindex("GetDevices", LuaWheel_GetDevices)
		newindex("IsDeviceConnected", LuaWheel_IsDeviceConnected)
		newindex("GetDeviceType", LuaWheel_GetDeviceType)
		newindex("GetDeviceModel", LuaWheel_GetDeviceModel)
		newindex("DeviceModelToName", LuaWheel_DeviceModelToName)
		newindex("DeviceManufacturerToName", LuaWheel_DeviceManufacturerToName)
		newindex("DeviceTypeToName", LuaWheel_DeviceTypeToName)
		newindex("ForceTypesToName", LuaWheel_ForceTypesToName)

		newindex("GetDeviceManufacturer", LuaWheel_GetDeviceManufacturer)

		newindex("SetPreferredProperties", LuaWheel_SetPreferredProperties)
		newindex("GetWheelProperties", LuaWheel_GetWheelProperties)
		newindex("GetWheelData", LuaWheel_GetWheelData)

		newindex("GetWheelPosition", LuaWheel_GetWheelPosition)
		newindex("GetWheelAccelerator", LuaWheel_GetWheelAccelerator)
		newindex("GetWheelBrake", LuaWheel_GetWheelBrake)
		newindex("GetWheelControls", LuaWheel_GetWheelControls)

		newindex("GetWheelButtons", LuaWheel_GetWheelButtons)
		newindex("IsWheelButtonTriggered", LuaWheel_IsButtonTriggered)
		newindex("IsWheelButtonReleased", LuaWheel_IsButtonReleased)
		newindex("IsWheelButtonPressed", LuaWheel_IsButtonPressed)

		newindex("SetWheelLED", LuaWheel_SetWheelLED)

		newindex("SetWheelNonLinear", LuaWheel_SetWheelNonLinear)
		newindex("ToWheelNonLinear", LuaWheel_ToWheelNonLinear)

		newindex("GetWheelForceFeedback", LuaWheel_GetWheelForceFeedback)
		newindex("GetWheelForceType", LuaWheel_GetWheelForceType)
		newindex("SetWheelForceType", LuaWheel_SetWheelForceType)

		newindex("GetWheelShiterMode", LuaWheel_GetWheelShiterMode)

		newindex("GetWheelOperationRange", LuaWheel_GetWheelOperationRange)
		newindex("SetWheelOperationRange", LuaWheel_SetWheelOperationRange)

		lua_pushstring(L, "ForceTypes");
	lua_newtable(L);
	for (int i = 0; i < 12; i++) {
		if (i == LOGI_NUMBER_FORCE_EFFECTS) continue;
		lua_pushstring(L, ForceTypeToString(i));
		lua_pushinteger(L, i + 1);
		lua_rawset(L, -3);
	}
	lua_rawset(L, -3);

	lua_pushstring(L, "PeriodicTypes");
	lua_newtable(L);
	for (int i = -1; i < 3; i++) {
		lua_pushstring(L, PeriodicTypeToString(i));
		lua_pushinteger(L, i + 2);
		lua_rawset(L, -3);
	}
	lua_rawset(L, -3);

	lua_pushstring(L, "DeviceTypes");
	lua_newtable(L);
	for (int i = -1; i < LOGI_NUMBER_DEVICE_TYPES; i++) {
		lua_pushstring(L, TypeToString(i));
		lua_pushinteger(L, i + 2);
		lua_rawset(L, -3);
	}
	lua_rawset(L, -3);

	lua_pushstring(L, "SupportedDevices");
	lua_newtable(L);
	for (int i = 0; i < LOGI_NUMBER_MODELS; i++) {
		lua_pushstring(L, ModelToString(i));
		lua_pushinteger(L, i + 1);
		lua_rawset(L, -3);
	}
	lua_rawset(L, -3);

	lua_pushstring(L, "ManufactureTypes");
	lua_newtable(L);
	for (int i = -1; i < 3; i++) {
		lua_pushstring(L, ManfToString(i));
		lua_pushinteger(L, i + 2);
		lua_rawset(L, -3);
	}
	lua_rawset(L, -3);


	lua_newuserdata(L, sizeof(char));
	luaL_getmetatable(L, "LuaWheel");
	lua_setmetatable(L, -2);

	lua_setfield(L, LUA_REGISTRYINDEX, "LuaWheelDLLFinalizer");

	return 1;
}
// print(pcall(require,"luawheel"))
// printt(require("luawheel"))