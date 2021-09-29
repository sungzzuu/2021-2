#define _WINSOCK_DEPRECATED_NO_WARNINGS // 최신 VC++ 컴파일 시 경고 방지

#pragma comment(lib, "ws2_32")
#include <WinSock2.h>
#include <stdio.h>

#define TESTNAME "www.example.com"

// 소켓 함수 오류 출력
void err_display(char* msg)
{
    LPVOID lpMsgBuf;
    FormatMessage(
        FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
        NULL, WSAGetLastError(),
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        (LPTSTR)&lpMsgBuf, 0, NULL);
    printf("[%s] %s", msg, (char*)lpMsgBuf);
    LocalFree(lpMsgBuf);
}

// 도메인 이름 -> IPv4 주소
BOOL GetIPAddr(char* name, IN_ADDR* addr)
{
	HOSTENT* ptr = gethostbyname(name);
	if (ptr == NULL)
	{
		err_display("gethostbyname()");
        return FALSE;
	}

    if (ptr->h_addrtype != AF_INET)
        return FALSE;
    memcpy(addr, ptr->h_addr, ptr->h_length);
    return TRUE;
}

// IPv4 주소 -> 도메인 이름
BOOL GetDomainName(IN_ADDR addr, char* name, int namelen)
{
    HOSTENT* ptr = gethostbyaddr((char*)&addr, sizeof(addr), AF_INET);
    if (ptr == NULL)
    {
        err_display("gethostbyaddr()");
        return FALSE;
    }
    if (ptr->h_addrtype != AF_INET)
        return FALSE;
 
    strncpy(name, ptr->h_name, namelen);
    return TRUE;

       
}

int main(int argc, char* argv[])
{
    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
        return 1;

    printf("도메인이름 변환전: %s\n", TESTNAME);

    // 도메인 이름 -> IP 주소
    IN_ADDR addr;
    if (GetIPAddr(TESTNAME, &addr))
        printf("IP주소 변환후: %s\n", inet_ntoa(addr)); // 숫자->문자열

    // IP주소 -> 도메인
    char name[256];
    if (GetDomainName(addr, name, sizeof(name)))
        printf("도메인이름 다시 변환: %s\n", name);
}