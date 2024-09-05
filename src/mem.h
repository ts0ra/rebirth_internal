#pragma once

#include <Windows.h>
#include <vector>

namespace mem
{
	void patch(BYTE* dst, BYTE* src, unsigned int size);
	void nop(BYTE* src, unsigned int size);
	std::uintptr_t read(std::uintptr_t ptr, std::vector<unsigned int> offsets);
}