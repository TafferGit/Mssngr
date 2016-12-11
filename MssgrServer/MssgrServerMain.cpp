#include "LibEvent.h"

int __cdecl main()
{
	LibEvent * libEventPtr = new LibEvent;
	libEventPtr->initialize_libevent();
	system("pause");
	delete libEventPtr;
	return 0;
}
