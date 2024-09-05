#include "mem.h"

namespace mem
{
	// patch((BYTE*)(offsets::base::radarMap), (BYTE*)"\x50\x8D\x4C\x24\x1C\x51\x8B\xCE\xFF\xD2", 10);
	// patch((BYTE*)(offsets::base::radarMap), (BYTE*)"50 8D 4C 24 1C 51 8B CE FF D2", 10);
	void patch(BYTE* dst, BYTE* src, unsigned int size)
	{
		DWORD oldProtect;
		VirtualProtect(dst, size, PAGE_EXECUTE_READWRITE, &oldProtect);
		memcpy(dst, src, size);
		VirtualProtect(dst, size, oldProtect, &oldProtect);
	}

	void nop(BYTE* dst, unsigned int size)
	{
		DWORD oldProtect;
		VirtualProtect(dst, size, PAGE_EXECUTE_READWRITE, &oldProtect);
		memset(dst, 0x90, size);
		VirtualProtect(dst, size, oldProtect, &oldProtect);
	}

	// Use case 1: read(offsets::base::localPlayer, { 0x4, 0x3C, 0x0 }),
	// Use case 2: read(offsets::base::localPlayer, { 0x0 }), just to dereference the pointer
	std::uintptr_t read(std::uintptr_t ptr, std::vector<unsigned int> offsets) 
	{
		std::uintptr_t currentAddress = ptr;
		for (unsigned int offset : offsets)
		{
			currentAddress = *reinterpret_cast<std::uintptr_t*>(currentAddress);
			currentAddress += offset;
		}
		return currentAddress;
	}
}