#include <windows.h>
#include <commctrl.h>
#include <stdio.h>
#include <UrlMon.h>
#include <wininet.h>

#pragma comment(lib,"urlmon.lib")
#pragma comment(lib,"wininet.lib")

int Pos;//프로그레스바 상태를 나타내는 변수

//다운로드 상태 알아오기 위해 클래스 생성
class StatusCallBack : public IBindStatusCallback
{
public:
	virtual HRESULT STDMETHODCALLTYPE OnStartBinding(
		/* [in] */ DWORD dwReserved,
		/* [in] */ IBinding* pib) {
		return S_OK;
	}

	virtual HRESULT STDMETHODCALLTYPE GetPriority(
		/* [out] */ LONG* pnPriority) {
		return S_OK;
	}

	virtual HRESULT STDMETHODCALLTYPE OnLowResource(
		/* [in] */ DWORD reserved) {
		return S_OK;
	}

	virtual HRESULT STDMETHODCALLTYPE OnProgress(
		/* [in] */ ULONG ulProgress,
		/* [in] */ ULONG ulProgressMax,
		/* [in] */ ULONG ulStatusCode,
		/* [in] */ LPCWSTR szStatusText)
	{
		if (ulProgressMax > 0)
			Pos = (double)ulProgress / (double)ulProgressMax * 100.0;//얼마나 다운로드 했는지   

		return S_OK;
	}

	virtual HRESULT STDMETHODCALLTYPE OnStopBinding(
		/* [in] */ HRESULT hresult,
		/* [unique][in] */ LPCWSTR szError) {
		return S_OK;
	}


	virtual /* [local] */ HRESULT STDMETHODCALLTYPE GetBindInfo(
		/* [out] */ DWORD* grfBINDF,
		/* [unique][out][in] */ BINDINFO* pbindinfo) {
		return S_OK;
	}

	virtual /* [local] */ HRESULT STDMETHODCALLTYPE OnDataAvailable(
		/* [in] */ DWORD grfBSCF,
		/* [in] */ DWORD dwSize,
		/* [in] */ FORMATETC* pformatetc,
		/* [in] */ STGMEDIUM* pstgmed) {
		return S_OK;
	}

	virtual HRESULT STDMETHODCALLTYPE OnObjectAvailable(
		/* [in] */ REFIID riid,
		/* [iid_is][in] */ IUnknown* punk) {
		return S_OK;
	}

	STDMETHOD_(ULONG, AddRef)() { return 0; }
	STDMETHOD_(ULONG, Release)() { return 0; }

	STDMETHOD(QueryInterface)(
		/* [in] */ REFIID riid,
		/* [iid_is][out] */ void __RPC_FAR* __RPC_FAR* ppvObject) {
		return E_NOTIMPL;
	}

public:
	StatusCallBack()
	{

	}

};


LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HINSTANCE g_hInst;
LPCTSTR lpszClass = TEXT("Test");


int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR ipszCmdParam, int nCmdShow)
{
	HWND hWnd;
	MSG Message;
	WNDCLASS WndClass;
	g_hInst = hInstance;


	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	WndClass.hInstance = hInstance;
	WndClass.lpfnWndProc = WndProc;
	WndClass.lpszClassName = lpszClass;
	WndClass.lpszMenuName = NULL;
	WndClass.style = CS_HREDRAW | CS_VREDRAW;
	RegisterClass(&WndClass);


	hWnd = CreateWindow(lpszClass, lpszClass,
		WS_OVERLAPPEDWINDOW, CW_USEDEFAULT,
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
		NULL, (HMENU)NULL, hInstance, NULL);
	ShowWindow(hWnd, nCmdShow);


	while (GetMessage(&Message, NULL, 0, 0))
	{
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}


	return (int)Message.wParam;


}


DWORD threadId;
HANDLE hThread;
HRESULT fileDownLoad;
HWND hProgress;

DWORD WINAPI ThreaDownloadProc(LPVOID lpParameter)
{
	HWND hwnd = (HWND)lpParameter;

	StatusCallBack callback;

	DeleteUrlCacheEntry("https://www.youtube.com/watch?v=pfYxoxL9LdU&ab_channel=%EC%A7%84%EC%98%81%ED%98%B8");//캐쉬 지우기(항상 다시 받기)

	HRESULT hr = URLDownloadToFile(
		NULL,
		"https://www.youtube.com/watch?v=pfYxoxL9LdU&ab_channel=%EC%A7%84%EC%98%81%ED%98%B8",
		"test.mp4",
		0,
		&callback
	);

	KillTimer(hwnd, 0);

	hThread = NULL;

	return hr;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;

	switch (iMessage)
	{
	case WM_CREATE:

		//프로그레스바 생성
		hProgress = CreateWindow(PROGRESS_CLASS, NULL, WS_CHILD | WS_VISIBLE | WS_BORDER,
			10, 10, 400, 30, hWnd, NULL, g_hInst, NULL);

		SendMessage(hProgress, PBM_SETRANGE, 0, MAKELPARAM(0, 100));//프로그레스 초기화
		SendMessage(hProgress, PBM_SETPOS, Pos, 0);//프로그레스 초기값

		return 0;

		//왼쪽 버튼 클릭하면 다운로드 시작
	case WM_LBUTTONDOWN:
		SetTimer(hWnd, 0, 100, NULL);

		if (!hThread)
		{
			//스레드 생성
			hThread = CreateThread(NULL, 0, ThreaDownloadProc,
				(LPVOID)hWnd, 0, &threadId);
		}
		return 0;

	case WM_TIMER:
		SendMessage(hProgress, PBM_SETPOS, Pos, 0);//프로그레스바 값 갱신
		return 0;

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return (DefWindowProc(hWnd, iMessage, wParam, lParam));
}