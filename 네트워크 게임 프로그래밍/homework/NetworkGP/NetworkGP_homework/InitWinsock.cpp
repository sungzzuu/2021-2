#pragma comment(lib, "ws2_32")
#include <WinSock2.h>

void err_quit(char* msg)
{
	LPVOID lpMsgBuf; // FormatMessage�Լ��� ������ ���� ���ڿ��� ���� ����

	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, NULL); // Flag, ���� �ڵ�, �����޼����� ǥ���� ���, �����޼����� ���� ������ �����ּ�, 

	MessageBox(NULL, (LPCTSTR)lpMsgBuf, msg, MB_ICONERROR);
	LocalFree(lpMsgBuf);
	exit(1);

}
int main(int argc, char* argv[])
{
	// ���� �ʱ�ȭ
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;
	 
	MessageBox(NULL, "���� �ʱ�ȭ ����", "�˸�", MB_OK);

	// socket()
	SOCKET tcp_socket = socket(AF_INET, SOCK_STREAM, 0); // �ּ�ü��� IPv4, ��������������
	if (tcp_socket == INVALID_SOCKET)
		err_quit("socket()");
	MessageBox(NULL, "TCP ���� ���� ����", "�˸�", MB_OK);

	// closesocket()
	closesocket(tcp_socket);

	// ���� ����
	WSACleanup();
	return 0;

}