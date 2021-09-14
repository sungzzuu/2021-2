#pragma comment(lib, "ws2_32")
#include <winsock2.h>
#include <iostream>
#include <string>


int main(int argc, char *argv[])
{
    // ���� �ʱ�ȭ
    WSADATA wsa;
    if (WSAStartup(MAKEWORD(1,1)/*���ӹ��� �����Լ�*/, &wsa) != 0)
        return 1;
    MessageBox(NULL, "���� �ʱ�ȭ ����", "�˸�", MB_OK);

    // �������� 2-1
    // wsa ������ �ʵ� �� ó�� �� �� (wVersion, wHighVersion, szDescription, szSystemStatus)�� ȭ�鿡 ����϶�.
    
    // wVersion�� WORD����(unsigned short)���� 2����Ʈ�̴�. 
    // LOBYTE�� ���� ���� 1����Ʈ
    // HIBYTE�� ���� ���� 1����Ʈ
    //    257 -> 256 + 1 -> 1.1
    //    514 -> 512 + 2 -> 2.2
    std::cout << "���� ����: " << static_cast<int>LOBYTE(wsa.wVersion) << "."
        << static_cast<int>HIBYTE(wsa.wVersion) << "\n";

    std::cout << "���� ��������: " << static_cast<int>LOBYTE(wsa.wHighVersion) << "."
        << static_cast<int>HIBYTE(wsa.wHighVersion) << "\n";

    std::cout << "���� ����: " << wsa.szDescription << "\n";
    std::cout << "���� ����: " << wsa.szSystemStatus << "\n";

    // ���� 1.1 ������ ����ϵ��� ������ ������ ��� �� ������ ��� �ٲ�°�?
    // MAKEWORD(1,1)�� �ٲٸ� �ȴ�. 
    // ������ 2.2 -> 1.1�� �ٲ��. 1.1���� ���� 1�� ���� ����Ʈ�� ������ �����̰�, 
    // ���� 1�� ���� ����Ʈ�� ���̳� ����
    // �������� WinSock 2.0, Running���� ����

    // ���� ����
    WSACleanup();
    return 0;
}
