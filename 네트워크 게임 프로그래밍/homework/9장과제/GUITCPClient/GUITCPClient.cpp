#define _CRT_SECURE_NO_WARNINGS         // 최신 VC++ 컴파일 시 경고 방지
#define _WINSOCK_DEPRECATED_NO_WARNINGS // 최신 VC++ 컴파일 시 경고 방지
#pragma comment(lib, "ws2_32")
#include <winsock2.h>
#include <stdlib.h>
#include <stdio.h>
#include <fstream>
#include "resource.h"

#define SERVERIP   "172.30.1.31"
#define SERVERPORT 8080
#define BUFSIZE 512

// 대화상자 프로시저
BOOL CALLBACK DlgProc(HWND, UINT, WPARAM, LPARAM);
// 편집 컨트롤 출력 함수
void DisplayText(char *fmt, ...);
// 오류 출력 함수
void err_quit(char *msg);
void err_display(char *msg);
// 사용자 정의 데이터 수신 함수
int recvn(SOCKET s, char *buf, int len, int flags);
// 소켓 통신 스레드 함수
DWORD WINAPI ClientMain(LPVOID arg);

SOCKET sock; // 소켓
char filename_buf[BUFSIZE]; // 데이터 송수신 버퍼
HANDLE hReadEvent, hWriteEvent; // 이벤트
HWND hSendButton; // 보내기 버튼
HWND hEdit1, hEdit2; // 편집 컨트롤


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
    LPSTR lpCmdLine, int nCmdShow)
{
    // 이벤트 생성
    hReadEvent = CreateEvent(NULL, FALSE, TRUE, NULL);
    if (hReadEvent == NULL) return 1;
    hWriteEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
    if (hWriteEvent == NULL) return 1;

    // 소켓 통신 스레드 생성
    CreateThread(NULL, 0, ClientMain, NULL, 0, NULL);

    // 대화상자 생성
    DialogBox(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), NULL, DlgProc);

    // 이벤트 제거
    CloseHandle(hReadEvent);
    CloseHandle(hWriteEvent);

    // closesocket()
    closesocket(sock);

    // 윈속 종료
    WSACleanup();
    return 0;
}

// 대화상자 프로시저
BOOL CALLBACK DlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg) {
    case WM_INITDIALOG:
        hEdit1 = GetDlgItem(hDlg, IDC_EDIT1);
        hEdit2 = GetDlgItem(hDlg, IDC_EDIT2);
        hSendButton = GetDlgItem(hDlg, IDOK);
        SendMessage(hEdit1, EM_SETLIMITTEXT, BUFSIZE, 0);
        return TRUE;
    case WM_COMMAND:
        switch (LOWORD(wParam)) {
        case IDOK:
            EnableWindow(hSendButton, FALSE); // 보내기 버튼 비활성화
            WaitForSingleObject(hReadEvent, INFINITE); // 읽기 완료 기다리기
            GetDlgItemText(hDlg, IDC_EDIT1, filename_buf, BUFSIZE + 1);
            SetEvent(hWriteEvent); // 쓰기 완료 알리기
            

            SetFocus(hEdit1);
            SendMessage(hEdit1, EM_SETSEL, 0, -1);
            return TRUE;
        case IDCANCEL:
            EndDialog(hDlg, IDCANCEL);
            return TRUE;
        }
        return FALSE;
    }
    return FALSE;
}

// 편집 컨트롤 출력 함수
void DisplayText(char *fmt, ...)
{
    va_list arg;
    va_start(arg, fmt);

    char cbuf[BUFSIZE + 256];
    vsprintf(cbuf, fmt, arg);

    int nLength = GetWindowTextLength(hEdit2);
    SendMessage(hEdit2, EM_SETSEL, nLength, nLength);
    SendMessage(hEdit2, EM_REPLACESEL, FALSE, (LPARAM)cbuf);

    va_end(arg);
}

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

// 소켓 함수 오류 출력
void err_display(char *msg)
{
    LPVOID lpMsgBuf;
    FormatMessage(
        FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
        NULL, WSAGetLastError(),
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        (LPTSTR)&lpMsgBuf, 0, NULL);
    DisplayText("[%s] %s", msg, (char *)lpMsgBuf);
    LocalFree(lpMsgBuf);
}

// 사용자 정의 데이터 수신 함수
int recvn(SOCKET s, char *buf, int len, int flags)
{
    int received;
    char *ptr = buf;
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

// TCP 클라이언트 시작 부분
DWORD WINAPI ClientMain(LPVOID arg)
{
    int retval;

    // 윈속 초기화
    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
        return 1;

    // socket()
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == INVALID_SOCKET) err_quit("socket()");

    // connect()
    SOCKADDR_IN serveraddr;
    ZeroMemory(&serveraddr, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = inet_addr(SERVERIP);
    serveraddr.sin_port = htons(SERVERPORT);
    retval = connect(sock, (SOCKADDR*)&serveraddr, sizeof(serveraddr));
    if (retval == SOCKET_ERROR) err_quit("connect()");

    while (true)
    {
        WaitForSingleObject(hWriteEvent, INFINITE); // 쓰기 완료 기다리기

        // 문자열 길이가 0이면 보내지 않음
        if (strlen(filename_buf) == 0) {
            EnableWindow(hSendButton, TRUE); // 보내기 버튼 활성화
            SetEvent(hReadEvent); // 읽기 완료 알리기
            continue;
        }

        break;
    }
    
    DisplayText("%s 파일 보내는중...", filename_buf);

    // 파일읽기
    std::ifstream readFile(filename_buf, std::ifstream::binary);

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

    int cnt = fileSize / BUFSIZE;

    int fileNameSize = strlen(filename_buf) + 1;
    // 데이터 보내는 순서: 파일이름 크기 -> 파일이름 -> 나눠보내는 크기 -> 파일

    // 데이터 보내기 -> 고정 길이 - 파일이름 크기
    retval = send(sock, (char*)&fileNameSize, sizeof(int), 0); // 길이가 고정된 값이 아닌 가변인자인 len
    if (retval == SOCKET_ERROR) {
        err_display("send()");
        return 0;
    }

    // 데이터 보내기 -> 가변 길이 - 파일이름
    retval = send(sock, filename_buf, fileNameSize, 0); // 길이가 고정된 값이 아닌 가변인자인 len
    if (retval == SOCKET_ERROR) {
        err_display("send()");
        return 0;
    }

    char* buf = new char[BUFSIZE];
    float percent = 0.f;
    int iBufSize = 512; // BUFSIZE와 같음

    for (int i = 0; i < cnt; ++i)
    {
        // 데이터 보내기 -> 고정 길이 - 파일 크기 -> 버퍼사이즈
        retval = send(sock, (char*)&iBufSize, sizeof(int), 0); // 길이가 고정된 값이 아닌 가변인자인 len
        if (retval == SOCKET_ERROR) {
            err_display("send()");
            return 0;
        }

        readFile.read(buf, BUFSIZE);
        // 데이터 보내기 -> 가변길이 - 파일
        retval = send(sock, buf, BUFSIZE, 0); // 길이가 고정된 값이 아닌 가변인자인 len
        if (retval == SOCKET_ERROR) {
            err_display("send()");
            return 0;
        }
        // 데이터 보내기 -> 고정 길이 - 보낸길이
        percent = ((float)i / (float)cnt) * 100.f;
        int p = (int)percent;
        retval = send(sock, (char*)&p, sizeof(int), 0); // 길이가 고정된 값이 아닌 가변인자인 len
        if (retval == SOCKET_ERROR) {
            err_display("send()");
            return 0;
        }
    }
    // 나머지 보내기
    cnt = fileSize % BUFSIZE;
    readFile.read(buf, cnt);

    // 데이터 보내기 -> 고정 길이 - 파일 크기 -> 버퍼사이즈
    retval = send(sock, (char*)&cnt, sizeof(int), 0); // 길이가 고정된 값이 아닌 가변인자인 len
    if (retval == SOCKET_ERROR) {
        err_display("send()");
        return 0;
    }
    // 데이터 보내기 -> 가변길이 - 파일
    retval = send(sock, buf, cnt, 0); // 길이가 고정된 값이 아닌 가변인자인 len
    if (retval == SOCKET_ERROR) {
        err_display("send()");
        return 0;
    }

    // 데이터 보내기 -> 고정 길이 - 보낸길이
    percent = 100;
    retval = send(sock, (char*)&percent, sizeof(int), 0); // 길이가 고정된 값이 아닌 가변인자인 len
    if (retval == SOCKET_ERROR) {
        err_display("send()");
        return 0;
    }

    DisplayText("\r\n%s 파일 보내기 완료!", filename_buf);

    EnableWindow(hSendButton, TRUE); // 보내기 버튼 활성화
    SetEvent(hReadEvent); // 읽기 완료 알리기
    // closesocket()
    closesocket(sock);

    // 윈속 종료
    WSACleanup();

    return 0;
}
