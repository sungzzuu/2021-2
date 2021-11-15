#define _CRT_SECURE_NO_WARNINGS         // 최신 VC++ 컴파일 시 경고 방지
#define _WINSOCK_DEPRECATED_NO_WARNINGS // 최신 VC++ 컴파일 시 경고 방지
#pragma comment(lib, "ws2_32")
#include <winsock2.h>
#include <stdlib.h>
#include <stdio.h>
#include <commctrl.h>
#include <fstream>


#define SERVERPORT 2701
#define BUFSIZE    512

// 윈도우 프로시저
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
// 편집 컨트롤 출력 함수
void DisplayText(char* fmt, ...);
// 오류 출력 함수
void err_quit(char* msg);
void err_display(char* msg);
// 소켓 통신 스레드 함수
DWORD WINAPI ServerMain(LPVOID arg);

HINSTANCE hInst; // 인스턴스 핸들
HWND hEdit; // 편집 컨트롤
CRITICAL_SECTION cs; // 임계 영역
HWND hProgress[2]; // 프로그레스 바

int g_percent[2];
int g_port[2];
int g_cnt = 0;


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
    LPSTR lpCmdLine, int nCmdShow)
{
    hInst = hInstance;
    InitializeCriticalSection(&cs);

    // 윈도우 클래스 등록
    WNDCLASS wndclass;
    wndclass.style = CS_HREDRAW | CS_VREDRAW;
    wndclass.lpfnWndProc = WndProc;
    wndclass.cbClsExtra = 0;
    wndclass.cbWndExtra = 0;
    wndclass.hInstance = hInstance;
    wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
    wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
    wndclass.lpszMenuName = NULL;
    wndclass.lpszClassName = "MyWndClass";
    if (!RegisterClass(&wndclass)) return 1;

    // 윈도우 생성
    HWND hWnd = CreateWindow("MyWndClass", "TCP 서버", WS_OVERLAPPEDWINDOW,
        500, 500, 500, 500, NULL, NULL, hInstance, NULL);
    if (hWnd == NULL) return 1;
    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    // 소켓 통신 스레드 생성
    CreateThread(NULL, 0, ServerMain, NULL, 0, NULL);

    // 메시지 루프
    MSG msg;
    while (GetMessage(&msg, 0, 0, 0) > 0) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    DeleteCriticalSection(&cs);
    return msg.wParam;
}

// 윈도우 프로시저
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg) {
    case WM_CREATE:
        

        hEdit = CreateWindow("edit", NULL,
            WS_CHILD | WS_VISIBLE | WS_HSCROLL |
            WS_VSCROLL | ES_AUTOHSCROLL |
            ES_AUTOVSCROLL | ES_MULTILINE | ES_READONLY,
            0, 0, 0, 0, hWnd, (HMENU)100, hInst, NULL);

        // 클라1 프로그레스바
        DisplayText("\r\n\r\n Client1\r\n\r\n\r\n");

        hProgress[0] = CreateWindow(PROGRESS_CLASS, NULL, WS_CHILD | WS_VISIBLE | WS_BORDER,
            60, 25, 400, 30, hWnd, NULL, hInst, NULL);

        SendMessage(hProgress[0], PBM_SETRANGE, 0, MAKELPARAM(0, 100));//프로그레스 초기화

        // 클라2 프로그레스바
        DisplayText(" Client2\r\n\r\n");

        hProgress[1] = CreateWindow(PROGRESS_CLASS, NULL, WS_CHILD | WS_VISIBLE | WS_BORDER,
            60, 75, 400, 30, hWnd, NULL, hInst, NULL);

        SendMessage(hProgress[1], PBM_SETRANGE, 0, MAKELPARAM(0, 100));//프로그레스 초기화

        return 0;
    case WM_SIZE:
        MoveWindow(hEdit, 0, 0, LOWORD(lParam), HIWORD(lParam), TRUE);
        return 0;
    case WM_SETFOCUS:
        //SetFocus(hEdit);
        return 0;
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }
    return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

// 편집 컨트롤 출력 함수
void DisplayText(char* fmt, ...)
{
    va_list arg;
    va_start(arg, fmt);

    char cbuf[BUFSIZE + 256];
    vsprintf(cbuf, fmt, arg);

    EnterCriticalSection(&cs);
    int nLength = GetWindowTextLength(hEdit);
    SendMessage(hEdit, EM_SETSEL, nLength, nLength);
    SendMessage(hEdit, EM_REPLACESEL, FALSE, (LPARAM)cbuf);
    LeaveCriticalSection(&cs);

    va_end(arg);
}
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

            SendMessage(hProgress[0], PBM_SETPOS, g_percent[0], 0);//프로그레스 초기값
            SendMessage(hProgress[1], PBM_SETPOS, g_percent[1], 0);//프로그레스 초기값

        }
        delete[] buf;
    }
    delete[] fileNameBuf;
    closesocket(client_sock);

    return 0;
}

// TCP 서버 시작 부분
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
    SOCKET client_sock;
    SOCKADDR_IN clientaddr;
    int addrlen;
    HANDLE hThread;
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

        hThread = CreateThread(NULL, 0, ClientProcess,
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

    // closesocket()
    closesocket(listen_sock);

    // 윈속 종료
    WSACleanup();
    return 0;
}
