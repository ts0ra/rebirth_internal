#include "console.h"

namespace console
{
	FILE* fDummy = nullptr;

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