#pragma once
#include <cstdint>
// Created with ReClass.NET 1.2 by KN4CK3R

struct Vector3
{
	float x, y, z;
};

class entList
{
public:
	int16_t x; //0x0000
	int16_t y; //0x0002
	int16_t z; //0x0004
	int16_t attr1; //0x0006
	int8_t type; //0x0008
	int8_t attr2; //0x0009
	int8_t attr3; //0x000A
	int8_t attr4; //0x000B
	int16_t attr5; //0x000C
	int8_t attr6; //0x000E
	int8_t attr7; //0x000F
	char pad_0010[2]; //0x0010
}; //Size: 0x0012

class playerEnt
{
public:
	char pad_0000[4]; //0x0000
	Vector3 playerHeadPos; //0x0004
	char pad_0010[24]; //0x0010
	Vector3 playerFootPos; //0x0028
	Vector3 viewAngles; //0x0034
	char pad_0040[172]; //0x0040
	int32_t health; //0x00EC
	int32_t armor; //0x00F0
	char pad_00F4[273]; //0x00F4
	char username[16]; //0x0205
	char pad_0215[247]; //0x0215
	int32_t teamSide; //0x030C
	char pad_0310[8]; //0x0310
	bool isDead; //0x0318
	char pad_0319[75]; //0x0319
	class currentWpnObj* ptrToCurrentWeaponObj; //0x0364
	char pad_0368[3440]; //0x0368
}; //Size: 0x10D8

class currentWpnObj
{
public:
	char pad_0000[4]; //0x0000
	int32_t gunId; //0x0004
	char pad_0008[4]; //0x0008
	class weaponStruct* ptrToCurrentWeaponStruct; //0x000C
	int32_t* ptrToReserveAmmo; //0x0010
	int32_t* ptrToLoadedAmmo; //0x0014
	int32_t* ptrToWeaponDelay; //0x0018
	int32_t shotCount; //0x001C
	char pad_0020[320]; //0x0020
}; //Size: 0x0160

class knifeObj
{
public:
	char pad_0000[48]; //0x0000
}; //Size: 0x0030

class weaponStruct
{
public:
	char modelName[23]; //0x0000
	char weaponName[42]; //0x0017
	char pad_0041[1]; //0x0041
	int16_t shootFX; //0x0042
	int16_t reloadFX; //0x0044
	int16_t reloadTime; //0x0046
	int16_t shootDelay; //0x0048
	int16_t damage; //0x004A
	int16_t piercing; //0x004C
	int16_t projectileSpeed; //0x004E
	int16_t part; //0x0050
	int16_t spread; //0x0052
	int16_t recoil; //0x0054
	int16_t magsize; //0x0056
	int16_t mdl_kick_rot; //0x0058
	int16_t mdl_kick_back; //0x005A
	int16_t recoilIncrease; //0x005C
	int16_t recoilBase; //0x005E
	int16_t maxRecoil; //0x0060
	int16_t recoilBackfade; //0x0062
	int16_t pushFactor; //0x0064
	bool isAuto; //0x0066
}; //Size: 0x0067

namespace data
{
	HWND hWindow{ nullptr };
}

namespace offsets
{
	constexpr std::uintptr_t gameMode = 0x18ABF8; // int, game mode (check enums)

	constexpr std::uintptr_t totalPlayer = 0x18AC0C; // int, total player in a match

	constexpr std::uintptr_t viewMatrix = 0x17DFD0; // float[16], view matrix

	constexpr std::uintptr_t playersEntityList = 0x18AC04; // uintptr_t, start from 1
	constexpr std::uintptr_t playerEntitySize = 0x4; // size 0x4 for each player

	constexpr std::uintptr_t itemsEntityList = 0x19086C; // uintptr_t, start from 0 (dont dereference this, use with offset directly)
	constexpr std::uintptr_t itemEntitySize = 0x18; // size 0x18 for each item
	constexpr std::uintptr_t itemPosition = 0x0; // Vector3ItemEntity, short data type, size 6 bytes
	constexpr std::uintptr_t itemType = 0x8; // int, enum item type
	constexpr std::uintptr_t itemAttr2 = 0x9; // int, for CTF_FLAG, attr2 = red/blue (0/1)

	constexpr std::uintptr_t totalItemPickups = 0x17F2B4; // uintptr_t, crosshair entity
	constexpr std::uintptr_t totalEntity = 0x1829B0; // int, on a map (exclude player entities)
	constexpr std::uintptr_t localPlayerEntity = 0x18AC00; // uintptr_t, local player entity

	constexpr std::uintptr_t name = 0x205; // char[16], player name
	constexpr std::uintptr_t head = 0x4; // vec3, player head
	constexpr std::uintptr_t foot = 0x28; // vec3, player feet
	constexpr std::uintptr_t viewAngles = 0x34; // vec3

	constexpr std::uintptr_t health = 0xEC; // int, 0 - 100
	constexpr std::uintptr_t armor = 0xF0; // int, 0 - 100
	constexpr std::uintptr_t teamSide = 0x30C; // 0: CLA, 1: RVSF, 2: CLA-SPEC, 3: RVSF-SPEC, 4: SPECTATOR
	constexpr std::uintptr_t isDead = 0x318; // 0: alive, 1: dead

	constexpr std::uintptr_t currentWeaponObject = 0x364;
	constexpr std::uintptr_t ammoReserve = 0x10; // uintptr_t, ammo reserve
	constexpr std::uintptr_t ammoLoaded = 0x14; // uintptr_t, ammo loaded
	constexpr std::uintptr_t noRecoil = 0xC8BA0; // uintptr_t, ammo loaded

	constexpr std::uintptr_t radarMap = 0x5D2C4;
	constexpr std::uintptr_t radarMiniMap = 0x5C4D6;

	//ac_client.exe+5D2C4 - 0F8D CC000000         - jnl ac_client.exe+5D396 radar hack map
	//ac_client.exe+5D3E6 - F3 0F7E 46 04         - movq xmm0,[esi+04] change to this

	//ac_client.exe+5C4D6 - 0F8D D5000000         - jnl ac_client.exe+5C5B1 radar hack minimap
	//ac_client.exe+5C606 - F3 0F7E 46 04         - movq xmm0,[esi+04]

}