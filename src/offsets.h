#pragma once

#include <cstdint>

namespace offsets
{
	namespace base
	{
		constexpr std::uintptr_t modelViewProjectionMatrix = 0x57DFD0; // float[16] / glmatrixf structure
		constexpr std::uintptr_t gameMode = 0x58ABF8; // int, game mode (check enums)
		constexpr std::uintptr_t localPlayer = 0x58AC00; // pointer
		constexpr std::uintptr_t playerList = 0x58AC04; // pointer
		constexpr std::uintptr_t totalPlayer = 0x58AC0C; // int, total player in a match
		constexpr std::uintptr_t entityList = 0x59086C; // pointer
		constexpr std::uintptr_t totalPickup = 0x57F2B4; // int, total pickup in a map
		constexpr std::uintptr_t totalEntity = 0x5829B0; // int, on a map (exclude player entities)
		constexpr std::uintptr_t radarMap = 0x5D2C4; // radar map patch bytes
		constexpr std::uintptr_t radarMiniMap = 0x5C4D6; // radar mini map patch bytes
	}

	namespace member
	{
		constexpr std::uintptr_t playerPtrSize = 0x4; // size 0x4 for each player
		constexpr std::uintptr_t entitySize = 0x18; // size 0x18 for each item
	}

	namespace function
	{
		constexpr std::uintptr_t mousemove = 0x4BF780;
	}
}

//ac_client.exe+5D2C4 - 0F8D CC000000         - jnl ac_client.exe+5D396 radar hack map
//ac_client.exe+5D3E6 - F3 0F7E 46 04         - movq xmm0,[esi+04] change to this

//ac_client.exe+5C4D6 - 0F8D D5000000         - jnl ac_client.exe+5C5B1 radar hack minimap
//ac_client.exe+5C606 - F3 0F7E 46 04         - movq xmm0,[esi+04]