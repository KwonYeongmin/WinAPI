// SocketProgramming.cpp : Defines the entry point for the application.
//
#include <winsock2.h>
#include <ws2tcpip.h>
#include <Windows.h>
#include <TCHAR.h>
#include <string>
#include "framework.h"
#include "SocketProgramming.h"

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
    WSADATA wsadata;
    SOCKET s;
    SOCKADDR_IN addr = { 0 };

    // 사용할 윈도우 소켓 버전을 지정하고 윈속을 초기화한다.
    // MAKEWORD(2, 2)는 윈속 버전 2.2를 지정한다.
    if (WSAStartup(MAKEWORD(2, 2), &wsadata) != 0)
    {
        int error = WSAGetLastError();
        std::wstring errorMsg = L"WSAStartup 실패. 에러 코드: " + std::to_wstring(error);
        MessageBox(NULL, errorMsg.c_str(), _T("오류"), MB_OK);
        return 1;
    }

    // TCP 소켓을 생성한다.
    // AF_INET은 IPv4를, SOCK_STREAM은 TCP 프로토콜을 의미한다.
    s = socket(AF_INET, SOCK_STREAM, 0);
    if (s == INVALID_SOCKET)
    {
        int error = WSAGetLastError();
        std::wstring errorMsg = L"소켓 생성 실패. 에러 코드: " + std::to_wstring(error);
        MessageBox(NULL, errorMsg.c_str(), _T("오류"), MB_OK);
        WSACleanup();
        return 1;
    }

    // 서버의 IP 주소와 포트 번호를 설정한다.
    // IP 주소 문자열인 "127.0.0.1"을 32비트 이진 형식으로 변환하여 addr.sin_addr에 저장한다.
    // 포트 번호는 htons()로 네트워크 바이트 순서로 변환하여 설정한다.
    addr.sin_family = AF_INET;
    addr.sin_port = htons(8080);  // 포트를 8080으로 설정
    inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr);  // IP 주소를 이진 형식으로 변환

    // 서버에 연결을 시도한다.
    // 서버의 IP 주소와 포트에 연결을 요청하며, 성공 시 소켓을 통해 서버와 통신할 수 있게 된다.
    if (connect(s, (LPSOCKADDR)&addr, sizeof(addr)) == SOCKET_ERROR)
    {
        int error = WSAGetLastError();
        std::wstring errorMsg = L"서버 연결 실패. 에러 코드: " + std::to_wstring(error);
        MessageBox(NULL, errorMsg.c_str(), _T("오류"), MB_OK);
        closesocket(s);
        WSACleanup();
        return 1;
    }

    // 소켓 연결을 종료한다.
    closesocket(s);

    // 윈속 사용을 종료하고, 모든 리소스를 해제한다.
    WSACleanup();
    return 0;
}

