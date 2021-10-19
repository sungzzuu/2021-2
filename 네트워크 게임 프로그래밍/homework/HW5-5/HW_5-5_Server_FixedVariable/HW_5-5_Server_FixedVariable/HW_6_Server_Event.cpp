#define _WINSOCK_DEPRECATED_NO_WARNINGS // �ֽ� VC++ ������ �� ��� ����
#pragma comment(lib, "ws2_32")
#include <winsock2.h>
#include <stdlib.h>
#include <stdio.h>
#include <fstream>

#define SERVERPORT 9000

HANDLE hThread1Event;
HANDLE hThread2Event;

int     g_port[2];
bool    bEventEnd[2];
// ���� ������ �������� ��밡��

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
    char* buf = nullptr;            // ���� ������ ����
    int len;                        // ���� ũ��
    char* fileNameBuf = nullptr;    // ���� �̸�
    int fileNameLen;                // ���� �̸� ũ��
    int sendOn = false;             // �����̸�, ũ��� �ѹ���
    int percent = 0;                // ���۷�
    std::ofstream writeFile;        // �� ����

    // Ŭ���̾�Ʈ ���� ���
    addrlen = sizeof(clientaddr);
    getpeername(client_sock, (SOCKADDR*)&clientaddr, &addrlen);
    int client_port = ntohs(clientaddr.sin_port);

    // Ŭ���̾�Ʈ�� ������ ���
    while (1) {
        
        if (client_port == g_port[0] && !bEventEnd[1])
        {
            retval = WaitForSingleObject(hThread2Event, INFINITE);     
            if (retval != WAIT_OBJECT_0)
                break;

        }
        else if (client_port == g_port[1] && !bEventEnd[0])
        {
            retval = WaitForSingleObject(hThread1Event, INFINITE);
            if (retval != WAIT_OBJECT_0)
                break;
        }

        
        if (!sendOn)
        {
            // ������ �ޱ�(���� ����) - ���� �̸� ũ��
            retval = recvn(client_sock, (char*)&fileNameLen, sizeof(int), 0);
            if (retval == SOCKET_ERROR) {
                err_display("recv()");
                break;
            }
            else if (retval == 0)
                break;

            fileNameBuf = new char[fileNameLen];
            // ������ �ޱ�(���� ����) - ���� �̸�
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
        // ������ �ޱ�(���� ����) - ���� ũ��
        retval = recvn(client_sock, (char*)&len, sizeof(int), 0);
        if (retval == SOCKET_ERROR) {
            err_display("recv()");

            break;
        }
        else if (retval == 0)
        {
            if (client_port == g_port[0])
            {
                SetEvent(hThread1Event);
                CloseHandle(hThread1Event);
                bEventEnd[0] = true;
            }
            else
            {
                SetEvent(hThread2Event);
                CloseHandle(hThread2Event);
                bEventEnd[1] = true;
            }
            break;
        }

        // ���� ���̸�ŭ�� ũ��� ���� ����
        buf = new char[len];

        // ������ �ޱ�(���� ����) - ����
        retval = recvn(client_sock, buf, len, 0);
        if (retval == SOCKET_ERROR) {
            err_display("recv()");
            break;
        }
        else if (retval == 0)
            break;

        // ���� ������ ����
        writeFile.write(buf, len);

        // ������ �ޱ�(���� ����) - ���۷�
        retval = recvn(client_sock, (char*)&percent, sizeof(int), 0);
        if (retval == SOCKET_ERROR) {
            err_display("recv()");
            break;
        }
        else if (retval == 0)
            break;

        if (percent >= 100)
            percent = 100;

        if (client_port == g_port[0])
            printf("Ŭ���̾�Ʈ(%d) ���۷�: %d%%\r\r\r\r\r\r\r", client_port, percent);
        else
            printf("\t\t\t\t\t\t\t\tŬ���̾�Ʈ(%d) ���۷�: %d%%\r\r\r\r\r\r\r\r\r\r\r\r\r\r", client_port, percent);

        delete[] buf;

        if (client_port == g_port[0])
            SetEvent(hThread1Event);
        else
            SetEvent(hThread2Event);

    }
    delete[] fileNameBuf;
    closesocket(client_sock);

}

// Ŭ���̾�Ʈ�� ������ ���
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

    hThread1Event = CreateEvent(NULL, FALSE, FALSE, NULL);
    if (hThread1Event == NULL)
        return 1;
    hThread2Event = CreateEvent(NULL, FALSE, TRUE, NULL);
    if (hThread2Event == NULL)
        return 1;

    HANDLE hThread[2];

    for (int i = 0;; i++)
    {
        addrlen = sizeof(clientaddr);
        client_sock = accept(listen_sock, (SOCKADDR*)&clientaddr, &addrlen);
        if (client_sock == INVALID_SOCKET) {
            err_display("accept()");
            break;
        }

        hThread[i] = CreateThread(NULL, 0, MyThread1,
            (LPVOID)client_sock, 0, NULL);

        g_port[i] = ntohs(clientaddr.sin_port);

        if (hThread[i] == NULL)
            closesocket(client_sock);

        // �� ���� ������ ���� ���
        if (i == 1)
            WaitForMultipleObjects(2, hThread, TRUE, INFINITE);
    }

    closesocket(listen_sock);

    WSACleanup();

}
