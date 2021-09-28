#define _WINSOCK_DEPRECATED_NO_WARNINGS // �ֽ� VC++ ������ �� ��� ����
#pragma comment(lib, "ws2_32")
#include <winsock2.h>
#include <stdlib.h>
#include <stdio.h>

// ���� �Լ� ���� ��� �� ����
void err_quit(char* msg)
{
    LPVOID lpMsgBuf;
    FormatMessage(
        FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
        NULL, WSAGetLastError(),
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        (LPTSTR)&lpMsgBuf, 0, NULL);
    MessageBox(NULL, (LPCTSTR)lpMsgBuf, msg, MB_ICONERROR);
    LocalFree(lpMsgBuf);
    exit(1);
}

int main(int argc, char* argv[])
{
    char* tmpIP = argv[1];
    int minPort = atoi(argv[2]);
    int maxPort = atoi(argv[3]);
    printf("%s %d %d\n", tmpIP, minPort, maxPort);

    int retval;

    // ���� �ʱ�ȭ
    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
        return 1;

    for (int i = minPort; i <= maxPort; i++)
    {
        // socket()
        SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
        if (sock == INVALID_SOCKET)
            err_quit("socket()");

        // connect()
        SOCKADDR_IN serveraddr;
        ZeroMemory(&serveraddr, sizeof(serveraddr));
        serveraddr.sin_family = AF_INET;
        serveraddr.sin_addr.s_addr = inet_addr(tmpIP);
        serveraddr.sin_port = htons(i);
        retval = connect(sock, (SOCKADDR*)&serveraddr, sizeof(serveraddr));
        if (retval != SOCKET_ERROR)
            printf("%d:LISTENING\n", i);
            
        // closesocket()
        closesocket(sock);
    }
 
    // ���� ����
    WSACleanup();
    return 0;
}
