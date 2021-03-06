#define _CRT_SECURE_NO_WARNINGS         // 최신 VC++ 컴파일 시 경고 방지
#define _WINSOCK_DEPRECATED_NO_WARNINGS // 최신 VC++ 컴파일 시 경고 방지
#pragma comment(lib, "ws2_32")
#include <winsock2.h>
#include <stdlib.h>
#include <stdio.h>
#include <fstream>
#include "resource.h"

#define SERVERPORT 8080
#define BUFSIZE 512

// 함수
BOOL CALLBACK DlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
void DisplayText(char* fmt, ...);
void err_quit(char* msg);
void err_display(char* msg);
int recvn(SOCKET s, char* buf, int len, int flags);
DWORD WINAPI ClientMain(LPVOID arg);

// 추가 정의 함수
void AddFileAtListBox(HDROP hDrop); // 파일 리스트박스에 드래그앤 드롭 처리
void GetFileNameFromListBox();
const char* ConvertWCtoC(wchar_t* str);
char* GetOnlyFileName(char* str);

// GUI 변수
HWND hIPEdit;           // IDC_EDIT1
HWND hConnectBtn;       // IDC_BUTTON1
HWND hFileListbox;      // IDC_LIST1
HWND hSelectedFileEdit; // IDC_EDIT2
HWND hSendBtn;          // IDOK
HWND hPrintEdit;        // IDC_EDIT3

// 전역 변수
SOCKET sock; // 소켓
char g_IPAddress[BUFSIZE];
char g_FileDir[BUFSIZE];
char g_FileName[BUFSIZE];

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
    // 대화상자 생성
    DialogBox(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), NULL, DlgProc);

    return 0;
}

BOOL CALLBACK DlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_INITDIALOG:
        SetWindowPos(hDlg, NULL, 800, 200, 400, 400, SWP_NOSIZE);

        // hwnd 설정
        hIPEdit = GetDlgItem(hDlg, IDC_EDIT1);
        hConnectBtn = GetDlgItem(hDlg, IDC_BUTTON1);
        hFileListbox = GetDlgItem(hDlg, IDC_LIST1);
        hSelectedFileEdit = GetDlgItem(hDlg, IDC_EDIT2);
        hSendBtn = GetDlgItem(hDlg, IDOK);
        hPrintEdit = GetDlgItem(hDlg, IDC_EDIT3);

        return TRUE;
       
    case WM_DROPFILES:
        // 여기서 드래그 앤 드랍 처리
        AddFileAtListBox((HDROP)wParam);
        break;
    case WM_COMMAND:
        switch (LOWORD(wParam))
        {
        case IDC_BUTTON1:
            GetDlgItemText(hDlg, IDC_EDIT1, g_IPAddress, BUFSIZE);
            EnableWindow(hIPEdit, FALSE); // 보내기 버튼 비활성화
            EnableWindow(hConnectBtn, FALSE); // 보내기 버튼 비활성화

            break;
        case IDC_LIST1:
            SetWindowText(GetDlgItem(hDlg, IDC_EDIT2), nullptr); // 에디트박스 비우기
            GetFileNameFromListBox();
            break;
        case IDOK:
            EnableWindow(hSendBtn, FALSE); // 보내기 버튼 비활성화
            SetFocus(0);
            strcpy(g_FileName, GetOnlyFileName(g_FileDir));

            // 소켓 통신 스레드 생성
            CreateThread(NULL, 0, ClientMain, NULL, 0, NULL);
            break;
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
    SendMessage(hPrintEdit, EM_SETSEL, nLength, nLength);
    SendMessage(hPrintEdit, EM_REPLACESEL, FALSE, (LPARAM)cbuf);

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
    serveraddr.sin_addr.s_addr = inet_addr(g_IPAddress);
    serveraddr.sin_port = htons(SERVERPORT);
    retval = connect(sock, (SOCKADDR*)&serveraddr, sizeof(serveraddr));
    if (retval == SOCKET_ERROR) err_quit("connect()");

    while (true)
    {
        //WaitForSingleObject(hWriteEvent, INFINITE); // 쓰기 완료 기다리기

        // 문자열 길이가 0이면 보내지 않음
        if (strlen(g_FileName) == 0) {
            EnableWindow(hSendBtn, TRUE); // 보내기 버튼 활성화
            //SetEvent(hReadEvent); // 읽기 완료 알리기
            continue;
        }

        break;
    }

    DisplayText("%s 파일 보내는중...", g_FileName);

    // 파일읽기
    std::ifstream readFile(g_FileDir, std::ifstream::binary);

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

    int fileNameSize = strlen(g_FileName) + 1;
    // 데이터 보내는 순서: 파일이름 크기 -> 파일이름 -> 나눠보내는 크기 -> 파일

    // 데이터 보내기 -> 고정 길이 - 파일이름 크기
    retval = send(sock, (char*)&fileNameSize, sizeof(int), 0); // 길이가 고정된 값이 아닌 가변인자인 len
    if (retval == SOCKET_ERROR) {
        err_display("send()");
        return 0;
    }

    // 데이터 보내기 -> 가변 길이 - 파일이름
    retval = send(sock, g_FileName, fileNameSize, 0); // 길이가 고정된 값이 아닌 가변인자인 len
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

    DisplayText("\r\n%s 파일 보내기 완료!", g_FileName);

    EnableWindow(hSendBtn, TRUE); // 보내기 버튼 활성화
    // SetEvent(hReadEvent); // 읽기 완료 알리기
  
    // closesocket()
    closesocket(sock);

    // 윈속 종료
    WSACleanup();

    return 0;
}

void AddFileAtListBox(HDROP hDrop)
{
    POINT pt;
    WCHAR buf[BUFSIZE];

    if (DragQueryPoint(hDrop, &pt))
    {
        int i = 0;

        UINT uCount = DragQueryFileA(hDrop, -1, NULL, 0);

        for (int i = 0; i < uCount; i++)
        {
            DragQueryFileW(hDrop, i, (LPWSTR)buf, BUFSIZE);
            SendMessageW(hFileListbox, LB_ADDSTRING, (WPARAM)TRUE, (LPARAM)buf);

        }
        SendMessageW(hFileListbox, LB_SETHORIZONTALEXTENT, BUFSIZE, 0);
    }
    DragFinish(hDrop);


}

void GetFileNameFromListBox()
{
    int index = SendMessageW(hFileListbox, LB_GETCURSEL, NULL, NULL);
    wchar_t temp[BUFSIZE];

    if (index != -1)
    {

        SendMessageW(hFileListbox, LB_GETTEXT, index, (LPARAM)&temp);
        strcpy(g_FileDir, ConvertWCtoC(temp));
        SendMessageW(hSelectedFileEdit, EM_REPLACESEL, FALSE, (LPARAM)temp);
    }
    
}

const char* ConvertWCtoC(wchar_t* str)
{
    char* pStr;
    int strSize = WideCharToMultiByte(CP_ACP, 0, str, -1, NULL, 0, NULL, NULL);
    pStr = new char[strSize];
    WideCharToMultiByte(CP_ACP, 0, str, -1, pStr, strSize, 0, 0);

    return pStr;
}

char* GetOnlyFileName(char* str)
{
    char* ptr = NULL;
    char fileName[BUFSIZE];
    int cnt = 0;

    ptr = strrchr(str, '\\');     //문자열(path)의 뒤에서부터 '\'의 위치를 검색하여 반환

    if (ptr == NULL)
        strcpy(fileName, str);
    else
        strcpy(fileName, ptr + 1); // 포인터에 +1을 더하여 파일이름만 추출


    return fileName;
}
