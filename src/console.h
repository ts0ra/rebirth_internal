#pragma once

#include <Windows.h>
#include <cstdio>

namespace console
{
	extern FILE* fDummy;

	void initConsole();
	void shutdownConsole();
}