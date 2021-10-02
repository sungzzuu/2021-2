#define _CRT_SECURE_NO_WARNINGS         // 최신 VC++ 컴파일 시 경고 방지
#define _WINSOCK_DEPRECATED_NO_WARNINGS // 최신 VC++ 컴파일 시 경고 방지
#pragma comment(lib, "ws2_32")
#include <winsock2.h>
#include <stdlib.h>
#include <stdio.h>
#include <fstream>

#define SERVERIP   "127.0.0.1"
#define SERVERPORT 9000

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

int main(int argc, char* argv[])
{
    //////////////////////////////////////////////////////////////////////
    /* 파일 읽고 버퍼에 저장하기 */
    char* fileName = argv[1];
    std::ifstream readFile(fileName, std::ifstream::binary);
    
    // 파일 읽었는지 확인
    if (!readFile.is_open())
    {
        printf("파일 읽기 실패!");
        return 0;
    }

    // 파일의 크기 알아내기
    readFile.seekg(0, readFile.end); // 다음으로 읽을 위치
    long fileSize = readFile.tellg(); // 현재위치
    readFile.seekg(0); // 위치 원래대로

    char* buf = new char[fileSize];
    readFile.read(buf, fileSize);
    
    //////////////////////////////////////////////////////////////////////


    int retval;

    // 윈속 초기화
    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
        return 1;

    // socket()
    SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == INVALID_SOCKET)
        err_quit("socket()");

    // connet()
    SOCKADDR_IN serveraddr;
    ZeroMemory(&serveraddr, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = inet_addr(SERVERIP);
    serveraddr.sin_port = htons(SERVERPORT);
    retval = connect(sock, (SOCKADDR*)&serveraddr, sizeof(serveraddr));
    if (retval == SOCKET_ERROR)
        err_quit("connect()");

    int fileNameSize = strlen(fileName)+1;
    printf("문자열길이: %d", fileNameSize);
    // 데이터 보내기 -> 고정 길이 - 파일이름 크기
    retval = send(sock, (char*)&fileNameSize, sizeof(int), 0); // 길이가 고정된 값이 아닌 가변인자인 len
    if (retval == SOCKET_ERROR) {
        err_display("send()");
        return 0;
    }

    // 데이터 보내기 -> 가변 길이 - 파일이름
    retval = send(sock, fileName, fileNameSize, 0); // 길이가 고정된 값이 아닌 가변인자인 len
    if (retval == SOCKET_ERROR) {
        err_display("send()");
        return 0;
    }

    // 데이터 보내기 -> 고정 길이 - 파일 크기
    retval = send(sock, (char*)&fileSize, sizeof(long), 0); // 길이가 고정된 값이 아닌 가변인자인 len
    if (retval == SOCKET_ERROR) {
        err_display("send()");
        return 0;
    }

    // 데이터 보내기 -> 가변길이 - 파일
    retval = send(sock, buf, fileSize, 0); // 길이가 고정된 값이 아닌 가변인자인 len
    if (retval == SOCKET_ERROR) {
        err_display("send()");
        return 0;
    }

    printf("[TCP 클라이언트] %d바이트를 보냈습니다.\n", retval);

    // closesocket()
    closesocket(sock);
    -
    // 윈속 종료
    WSACleanup();

    delete[] buf;
}