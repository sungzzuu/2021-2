#pragma comment(lib, "ws2_32")
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <stdio.h>

int main()
{
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;

	/*IPv4 ��ȯ*/
	// ������ IPv4 �ּ����
	char* ipv4test = "147.46.114.70";
	printf("")

	// IPv6 ��ȯ
}
