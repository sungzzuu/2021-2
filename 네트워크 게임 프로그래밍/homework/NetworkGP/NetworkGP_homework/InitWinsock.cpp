#pragma comment(lib, "ws2_32")
#include <WinSock2.h>

void err_quit(char* msg)
{
	LPVOID lpMsgBuf; // FormatMessage함수가 생성한 에러 문자열을 담을 버퍼

	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, NULL); // Flag, 오류 코드, 오류메세지를 표시할 언어, 오류메세지를 담을 버퍼의 시작주소, 

	MessageBox(NULL, (LPCTSTR)lpMsgBuf, msg, MB_ICONERROR);
	LocalFree(lpMsgBuf);
	exit(1);

}
int main(int argc, char* argv[])
{
	// 윈속 초기화
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;
	 
	MessageBox(NULL, "윈속 초기화 성공", "알림", MB_OK);

	// socket()
	SOCKET tcp_socket = socket(AF_INET, SOCK_STREAM, 0); // 주소체계는 IPv4, 연결형프로토콜
	if (tcp_socket == INVALID_SOCKET)
		err_quit("socket()");
	MessageBox(NULL, "TCP 소켓 생성 성공", "알림", MB_OK);

	// closesocket()
	closesocket(tcp_socket);

	// 윈속 종료
	WSACleanup();
	return 0;

}