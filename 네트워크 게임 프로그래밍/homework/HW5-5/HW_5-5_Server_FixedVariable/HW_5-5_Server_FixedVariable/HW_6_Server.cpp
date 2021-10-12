#define _WINSOCK_DEPRECATED_NO_WARNINGS // 최신 VC++ 컴파일 시 경고 방지
#pragma comment(lib, "ws2_32")
#include <winsock2.h>
#include <stdlib.h>
#include <stdio.h>
#include <fstream>

#define SERVERPORT 9000

int g_percent[2];
int g_port[2];
int g_cnt = 0;

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

int recvn(SOCKET s, char* buf, int len, int flags)
{
    int received;
    char* ptr = buf;
    int left = len;

    while (left > 0) {
        received = recv(s, ptr, left, flags);
        if (received == SOCKET_ERROR)
            return SOCKET_ERROR;
        else if (received == 0)
            break;
        left -= received;
        ptr += received;
    }

    return (len - left);
}

void ProcessClient(LPVOID arg)
{
    SOCKET client_sock = (SOCKET)arg;
    int retval;
    SOCKADDR_IN clientaddr;

    int addrlen;
    char* buf = nullptr;            // 파일 저장할 버퍼
    int len;                        // 파일 크기
    char* fileNameBuf = nullptr;    // 파일 이름
    int fileNameLen;                // 파일 이름 크기
    int sendOn = false;             // 파일이름, 크기는 한번만
    int percent = 0;                // 전송률
    std::ofstream writeFile;        // 쓸 파일

    // 클라이언트 정보 얻기
    addrlen = sizeof(clientaddr);
    getpeername(client_sock, (SOCKADDR*)&clientaddr, &addrlen);

    g_port[g_cnt++] = ntohs(clientaddr.sin_port);

    // 클라이언트와 데이터 통신
    while (1) {
        if (!sendOn)
        {
            // 데이터 받기(고정 길이) - 파일 이름 크기
            retval = recvn(client_sock, (char*)&fileNameLen, sizeof(int), 0);
            if (retval == SOCKET_ERROR) {
                err_display("recv()");
                break;
            }
            else if (retval == 0)
                break;

            fileNameBuf = new char[fileNameLen];
            // 데이터 받기(고정 길이) - 파일 이름
            retval = recvn(client_sock, fileNameBuf, fileNameLen, 0);
            if (retval == SOCKET_ERROR) {
                err_display("recv()");
                break;
            }
            else if (retval == 0)
                break;
            writeFile.open(fileNameBuf, std::ifstream::binary);
            sendOn = true;
        }
        // 데이터 받기(고정 길이) - 파일 크기
        retval = recvn(client_sock, (char*)&len, sizeof(int), 0);
        if (retval == SOCKET_ERROR) {
            err_display("recv()");
            break;
        }
        else if (retval == 0)
            break;

        // 고정 길이만큼의 크기로 버퍼 생성
        buf = new char[len];

        // 데이터 받기(가변 길이) - 파일
        retval = recvn(client_sock, buf, len, 0);
        if (retval == SOCKET_ERROR) {
            err_display("recv()");
            break;
        }
        else if (retval == 0)
            break;

        // 받은 데이터 저장
        writeFile.write(buf, len);

        // 데이터 받기(고정 길이) - 전송률
        retval = recvn(client_sock, (char*)&percent, sizeof(int), 0);
        if (retval == SOCKET_ERROR) {
            err_display("recv()");
            break;
        }
        else if (retval == 0)
            break;

        //g_percent[threadNum] = percent;
        if (percent != g_percent[0] || percent != g_percent[1])
        {
            if (percent >= 100)
                percent = 100;
            if (ntohs(clientaddr.sin_port) == g_port[0])
                g_percent[0] = percent;
            else
                g_percent[1] = percent;
            printf("클라이언트(%d) 전송률: %d%% \r\r\r\r\r", g_port[0], g_percent[0]);
            if (g_port[1] != 0)
            {
                printf("\t\t\t\t\t\t클라이언트(%d) 전송률: %d%%\r\r\r\r\r\r\r\r\r\r\r\r\r", g_port[1], g_percent[1]);

            }
        }
        delete[] buf;
    }
    delete[] fileNameBuf;
    closesocket(client_sock);

}
// 클라이언트와 데이터 통신
DWORD WINAPI MyThread1(LPVOID arg)
{
    ProcessClient(arg);

    return 0;
}

int main(int argc, char* argv[])
{
    int retval;

    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
        return 1;

    SOCKET listen_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (listen_sock == INVALID_SOCKET) err_quit("socket()");

    SOCKADDR_IN serveraddr;
    ZeroMemory(&serveraddr, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serveraddr.sin_port = htons(SERVERPORT);
    retval = bind(listen_sock, (SOCKADDR*)&serveraddr, sizeof(serveraddr));
    if (retval == SOCKET_ERROR) err_quit("bind()");

    retval = listen(listen_sock, SOMAXCONN);
    if (retval == SOCKET_ERROR) err_quit("listen()");

    SOCKET client_sock;
    SOCKADDR_IN clientaddr;
    int addrlen;
    HANDLE hThread = nullptr;
    int cnt = 0;

    while(1)
    {
        cnt++;
        addrlen = sizeof(clientaddr);
        client_sock = accept(listen_sock, (SOCKADDR*)&clientaddr, &addrlen);
        if (client_sock == INVALID_SOCKET) {
            err_display("accept()");
            break;
        }

        hThread = CreateThread(NULL, 0, MyThread1,
            (LPVOID)client_sock, 0, NULL);

        if (cnt == 2)
        {
            if (hThread != NULL && WaitForSingleObject(hThread, INFINITE) != WAIT_FAILED)
                break;
        }

        if (hThread == NULL)
        {
            closesocket(client_sock);
        }
        else {
            CloseHandle(hThread);
        }

        
    }

    closesocket(listen_sock);

    WSACleanup();

}
