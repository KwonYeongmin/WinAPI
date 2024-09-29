// SocketProgramming.cpp : Defines the entry point for the application.
//
#include <winsock2.h>
#include <ws2tcpip.h>
#include <Windows.h>
#include <TCHAR.h>
#include <string>
#include "framework.h"
#include "SocketProgramming.h"

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{

    WSADATA wsadata;
    SOCKET s;
    SOCKADDR_IN addr = { 0 };

    // 사용할 윈도우 소켓 버전을 지정하고 소켓 정보를 얻어온다.
    // MAKEWORD(2, 2)는 WORD 타입의 값을 만들어 윈속의 버전 2.2를 지정하는 역할을 한다.
    if (WSAStartup(MAKEWORD(2, 2), &wsadata) != 0)
    {
        int error = WSAGetLastError();
        std::wstring errorMsg = L"WSAStartup 실패. 에러 코드: " + std::to_wstring(error);
        MessageBox(NULL, errorMsg.c_str(), _T("오류"), MB_OK);
        return 1;
    }

    // TCP 소켓을 생성한다. AF_INET은 IPv4, SOCK_STREAM은 TCP 프로토콜을 의미한다.
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

    // 설정된 주소와 포트로 소켓을 바인딩한다.
    if (bind(s, (LPSOCKADDR)&addr, sizeof(addr)) == SOCKET_ERROR)
    {
        int error = WSAGetLastError();
        std::wstring errorMsg = L"바인드 실패. 에러 코드: " + std::to_wstring(error);
        MessageBox(NULL, errorMsg.c_str(), _T("오류"), MB_OK);
        closesocket(s);
        WSACleanup();
        return 1;
    }

    // 소켓을 클라이언트의 연결을 기다리는 상태로 만든다. 대기열 크기는 5로 설정.
    if (listen(s, 5) == SOCKET_ERROR)
    {
        int error = WSAGetLastError();
        std::wstring errorMsg = L"리스닝 실패. 에러 코드: " + std::to_wstring(error);
        MessageBox(NULL, errorMsg.c_str(), _T("오류"), MB_OK);
        closesocket(s);
        WSACleanup();
        return 1;
    }

    MessageBox(NULL, _T("서버가 성공적으로 시작되었습니다."), _T("정보"), MB_OK);

    // 클라이언트 연결 대기 루프
    do
    {
        // 접속을 시도하는 클라이언트의 정보를 담을 구조체
        SOCKADDR_IN c_addr;
        int size = sizeof(c_addr);

        // 클라이언트의 연결을 기다리고, 연결이 발생하면 새 소켓을 반환한다.
        SOCKET client = accept(s, (LPSOCKADDR)&c_addr, &size);

        if (client == INVALID_SOCKET)
        {
            int error = WSAGetLastError();
            std::wstring errorMsg = L"클라이언트 수락 실패. 에러 코드: " + std::to_wstring(error);
            MessageBox(NULL, errorMsg.c_str(), _T("오류"), MB_OK);
        }
        else
        {
            // 클라이언트의 접속을 확인한 후 서버를 종료할지 여부를 묻는다.
            MessageBox(NULL, _T("클라이언트 접속을 확인했습니다. 서버를 종료하시겠습니까?"), _T("Server 메시지"), MB_YESNO);
            closesocket(client);  // 클라이언트 소켓 닫기
        }
    } while (MessageBox(NULL, _T("서버를 계속 실행하시겠습니까?"), _T("Server 메시지"), MB_YESNO) == IDNO);

    closesocket(s);  // 서버 소켓 닫기
    WSACleanup();    // 윈속 종료
    return 1;
}


