#define _WINSOCK_DEPRECATED_NO_WARNINGS // 최신 VC++ 컴파일 시 경고 방지
#pragma comment(lib, "ws2_32")
#include <winsock2.h>
#include <stdlib.h>
#include <stdio.h>

// 소켓 함수 오류 출력 후 종료
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

    // 윈속 초기화
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
 
    // 윈속 종료
    WSACleanup();
    return 0;
}
