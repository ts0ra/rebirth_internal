#ifndef DEBUG_REBIRTH
#define DEBUG_REBIRTH
#include <Windows.h>
#include <cstdio>

namespace debug
{
	inline FILE* fDummy = nullptr; // FILE pointer for redirection

	void initConsole()
	{
		AllocConsole();
		freopen_s(&fDummy, "CONOUT$", "w", stdout);
	}

	void shutdownConsole()
	{
		if (fDummy != nullptr)
		{
			fclose(fDummy);
			fDummy = nullptr;  // Avoid dangling pointer
		}
		FreeConsole();
	}
}

#endif // DEBUG_REBIRTH