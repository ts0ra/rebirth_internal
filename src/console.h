#pragma once
#include <Windows.h>
#include <cstdio>

namespace debug
{
	FILE* fDummy; // Don't use this variable, its just a dummy!

	void initConsole()
	{
		fDummy = { nullptr };
		AllocConsole();
		freopen_s(&fDummy, "CONOUT$", "w", stdout);
	}

	void shutdownConsole()
	{
		if (fDummy != 0)
			fclose(fDummy);
		FreeConsole();
	}
}