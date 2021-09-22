#pragma comment(lib, "ws2_32")
#include <WinSock2.h>
#include <stdio.h>

BOOL IsLittleEndian();
BOOL IsBigEndian();

BOOL IsLittleEndian()
{
	u_short d = 0x1234;
	u_char* p = (u_char*)&d;

	if (*p == 0x34)
		return TRUE;
	else
		return FALSE;
}

BOOL IsBigEndian()
{
	if (!IsLittleEndian())
		return TRUE;
	else
		return FALSE;
}
