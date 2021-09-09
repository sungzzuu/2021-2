#pragma comment(lib, "ws2_32")
#include <winsock2.h>
#include <iostream>
#include <string>

// 소켓 함수 오류 출력 후 종료
void err_quit(char *msg)
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

int main(int argc, char *argv[])
{
    // 윈속 초기화
    WSADATA wsa;
    if (WSAStartup(MAKEWORD(1,1)/*윈속버전 지정함수*/, &wsa/*거의 사용안함*/) != 0)
        return 1;
    MessageBox(NULL, "윈속 초기화 성공", "알림", MB_OK);

    // 연습문제 2-1
    // wsa 변수의 필드 중 처음 네 개 (wVersion, wHighVersion, szDescription, szSystemStatus)를 화면에 출력하라.
    std::cout << wsa.wVersion << "\n" << wsa.wHighVersion << "\n"
        << wsa.szDescription << "\n" << wsa.szSystemStatus << "\n";
   
    
    // 윈속 1.1 버전을 사용하도록 예제를 수정할 경우 이 값들이 어떻게 바뀌는가?
    // MAKEWORD(1,1)로 바꾸면 된다. 
    // 514, 514 -> 257, 514 나머지는 WinSock 2.0, Running으로 동일

    SOCKET tcp_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (tcp_sock == INVALID_SOCKET) err_quit("socket()");
    MessageBox(NULL, "TCP 소켓 생성 성공", "알림", MB_OK);

    // closesocket()
    closesocket(tcp_sock);

    // 윈속 종료
    WSACleanup();
    return 0;
}
