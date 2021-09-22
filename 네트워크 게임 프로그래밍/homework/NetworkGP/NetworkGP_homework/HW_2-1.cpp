#pragma comment(lib, "ws2_32")
#include <winsock2.h>
#include <iostream>
#include <string>


int main(int argc, char* argv[])
{
    // 윈속 초기화
    WSADATA wsa;
    if (WSAStartup(MAKEWORD(1, 1)/*윈속버전 지정함수*/, &wsa) != 0)
        return 1;
    MessageBox(NULL, "윈속 초기화 성공", "알림", MB_OK);

    // 연습문제 2-1
    // wsa 변수의 필드 중 처음 네 개 (wVersion, wHighVersion, szDescription, szSystemStatus)를 화면에 출력하라.

    // wVersion은 WORD형식(unsigned short)으로 2바이트이다. 
    // LOBYTE는 가장 하위 1바이트
    // HIBYTE는 가장 상위 1바이트
    //    257 -> 256 + 1 -> 1.1
    //    514 -> 512 + 2 -> 2.2
    std::cout << "윈속 버전: " << static_cast<int>LOBYTE(wsa.wVersion) << "."
        << static_cast<int>HIBYTE(wsa.wVersion) << "\n";

    std::cout << "윈속 상위버전: " << static_cast<int>LOBYTE(wsa.wHighVersion) << "."
        << static_cast<int>HIBYTE(wsa.wHighVersion) << "\n";

    std::cout << "윈속 설명: " << wsa.szDescription << "\n";
    std::cout << "윈속 상태: " << wsa.szSystemStatus << "\n";

    // 윈속 1.1 버전을 사용하도록 예제를 수정할 경우 이 값들이 어떻게 바뀌는가?
    // MAKEWORD(1,1)로 바꾸면 된다. 
    // 버전이 2.2 -> 1.1로 바뀐다. 1.1에서 앞의 1은 하위 바이트로 메이저 버전이고, 
    // 뒤의 1은 상위 바이트로 마이너 버전
    // 나머지는 WinSock 2.0, Running으로 동일

    // 윈속 종료
    WSACleanup();
    return 0;
}
