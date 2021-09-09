#pragma comment(lib, "ws2_32")
#include <winsock2.h>
#include <iostream>
#include <string>

// ���� �Լ� ���� ��� �� ����
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
    // ���� �ʱ�ȭ
    WSADATA wsa;
    if (WSAStartup(MAKEWORD(1,1)/*���ӹ��� �����Լ�*/, &wsa/*���� ������*/) != 0)
        return 1;
    MessageBox(NULL, "���� �ʱ�ȭ ����", "�˸�", MB_OK);

    // �������� 2-1
    // wsa ������ �ʵ� �� ó�� �� �� (wVersion, wHighVersion, szDescription, szSystemStatus)�� ȭ�鿡 ����϶�.
    std::cout << wsa.wVersion << "\n" << wsa.wHighVersion << "\n"
        << wsa.szDescription << "\n" << wsa.szSystemStatus << "\n";
   
    
    // ���� 1.1 ������ ����ϵ��� ������ ������ ��� �� ������ ��� �ٲ�°�?
    // MAKEWORD(1,1)�� �ٲٸ� �ȴ�. 
    // 514, 514 -> 257, 514 �������� WinSock 2.0, Running���� ����

    SOCKET tcp_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (tcp_sock == INVALID_SOCKET) err_quit("socket()");
    MessageBox(NULL, "TCP ���� ���� ����", "�˸�", MB_OK);

    // closesocket()
    closesocket(tcp_sock);

    // ���� ����
    WSACleanup();
    return 0;
}
