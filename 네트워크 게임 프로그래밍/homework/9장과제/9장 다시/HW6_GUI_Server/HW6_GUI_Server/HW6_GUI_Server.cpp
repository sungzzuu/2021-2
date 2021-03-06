#define _CRT_SECURE_NO_WARNINGS         // 최신 VC++ 컴파일 시 경고 방지
#define _WINSOCK_DEPRECATED_NO_WARNINGS // 최신 VC++ 컴파일 시 경고 방지
#pragma comment(lib, "ws2_32")
#include <winsock2.h>
#include <stdlib.h>
#include <stdio.h>
#include <fstream>
#include "resource.h"
#include <CommCtrl.h>


#define SERVERPORT 8080
#define BUFSIZE 512

// 함수
BOOL CALLBACK DlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
void DisplayText(char* fmt, ...);
void err_quit(char* msg);
void err_display(char* msg);
DWORD WINAPI ServerMain(LPVOID arg);
DWORD WINAPI ClientProcess(LPVOID arg);

// GUI 변수
HWND hProgress[2];
HWND hPrintEdit;
HWND hDialog;

// 전역 변수
CRITICAL_SECTION cs; // 임계 영역
HANDLE hThread[2];
SOCKET g_client_sock[3];
int g_percent[2];
DWORD g_dwThreadId[2];

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
    InitializeCriticalSection(&cs);

    // 대화상자 생성
    CreateThread(NULL, 0, ServerMain, NULL, 0, NULL);
    DialogBox(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), NULL, DlgProc);

    DeleteCriticalSection(&cs);

    return 0;
}

BOOL CALLBACK DlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    hDialog = hDlg;
    switch (uMsg)
    {
    case WM_INITDIALOG:
        SetWindowPos(hDlg, NULL, 800, 200, 400, 400, SWP_NOSIZE);

        hProgress[0] = GetDlgItem(hDlg, IDC_PROGRESS1);
        hProgress[1] = GetDlgItem(hDlg, IDC_PROGRESS2);
        hPrintEdit = GetDlgItem(hDlg,  IDC_EDIT1);

        return TRUE;

    case WM_COMMAND:
        switch (LOWORD(wParam))
        {
        case IDCANCEL:
            EndDialog(hDlg, IDCANCEL);
            return TRUE;
        default:
            break;
        }
        return FALSE;
    default:
        break;
    }
    return FALSE;
}

void DisplayText(char* fmt, ...)
{
    va_list arg;
    va_start(arg, fmt);

    char cbuf[BUFSIZE + 256];
    vsprintf(cbuf, fmt, arg);
    int nLength = GetWindowTextLength(hPrintEdit);

    EnterCriticalSection(&cs);

    SendMessage(hPrintEdit, EM_SETSEL, nLength, nLength);
    SendMessage(hPrintEdit, EM_REPLACESEL, FALSE, (LPARAM)cbuf);

    LeaveCriticalSection(&cs);

    va_end(arg);
}

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
    DisplayText("[%s] %s", msg, (char*)lpMsgBuf);
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

// 클라이언트와 데이터 통신
DWORD WINAPI ClientProcess(LPVOID arg)
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

            DisplayText("\r\n파일이름:%s 전송시작", fileNameBuf);

        }


        // 데이터 받기(고정 길이) - 파일 크기
        retval = recvn(client_sock, (char*)&len, sizeof(int), 0);
        if (retval == SOCKET_ERROR) {
            err_display("recv()");
            break;
        }
        else if (retval == 0)
        {
            DisplayText("\r\n파일이름:%s 전송완료", fileNameBuf);
            break;
        }

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


        if (percent != g_percent[0] || percent != g_percent[1])
        {
            if (percent >= 100)
                percent = 100;
            if (GetCurrentThreadId() == g_dwThreadId[0])
                g_percent[0] = percent;
            else
                g_percent[1] = percent;

            EnterCriticalSection(&cs);

            SendMessage(hProgress[0], PBM_SETPOS, g_percent[0], 0);
            SendMessage(hProgress[1], PBM_SETPOS, g_percent[1], 0);

            LeaveCriticalSection(&cs);


        }
        delete[] buf;


    }
    delete[] fileNameBuf;
    closesocket(client_sock);

    return 0;
}

DWORD WINAPI ServerMain(LPVOID arg)
{
    int retval;

    // 윈속 초기화
    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
        return 1;

    // socket()
    SOCKET listen_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (listen_sock == INVALID_SOCKET) err_quit("socket()");

    // bind()
    SOCKADDR_IN serveraddr;
    ZeroMemory(&serveraddr, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serveraddr.sin_port = htons(SERVERPORT);
    retval = bind(listen_sock, (SOCKADDR*)&serveraddr, sizeof(serveraddr));
    if (retval == SOCKET_ERROR) err_quit("bind()");

    // listen()
    retval = listen(listen_sock, SOMAXCONN);
    if (retval == SOCKET_ERROR) err_quit("listen()");

    // 데이터 통신에 사용할 변수
    SOCKADDR_IN clientaddr;
    HANDLE hThread;

    int addrlen;
    int cnt = 0;

    while (1)
    {
        addrlen = sizeof(clientaddr);
        g_client_sock[cnt] = accept(listen_sock, (SOCKADDR*)&clientaddr, &addrlen);
        if (g_client_sock[cnt] == INVALID_SOCKET) {
            err_display("accept()");
            break;
        }

        hThread = CreateThread(NULL, 0, ClientProcess,
            (LPVOID)g_client_sock[cnt], 0, NULL);
        g_dwThreadId[cnt] = GetThreadId(hThread);

        //CloseHandle(hThread[cnt]);
        
        cnt++;
    }

    // closesocket()
    //closesocket(client_sock[0]);
    //closesocket(client_sock[1]);
    closesocket(listen_sock);

    // 윈속 종료
    WSACleanup();
    return 0;
}
