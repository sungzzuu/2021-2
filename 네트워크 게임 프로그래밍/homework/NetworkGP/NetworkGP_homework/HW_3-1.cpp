#pragma comment(lib, "ws2_32")
#include <WinSock2.h>
#include <stdio.h>

BOOL IsLittleEndian();
BOOL IsBigEndian();

int main()
{
	if (IsLittleEndian())
		printf("시스템에서 사용하는 바이트 정렬방식은 리틀엔디안\n");
	else
		printf("시스템에서 사용하는 바이트 정렬방식은 빅엔디안\n");
}

BOOL IsLittleEndian()
{
	u_short d = 0x1234; // 2바이트 저장
	u_char p = static_cast<u_char>(d); // 1바이트 저장

	if (p == 0x34)
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
