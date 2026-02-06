#include "server.h"
#include <ImGuiManager.h>
#include <cassert>

void server::Initialize() {
    WSADATA wsaData;
    int iResult;

    // Winsock 初期化
    iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0) {
        return;
    }

    // listen用ソケット作成
    listenSocket_ = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (listenSocket_ == INVALID_SOCKET) {
        WSACleanup();
        return;
    }

    // バインド
    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(12345);

    iResult = bind(listenSocket_, (sockaddr*)&addr, sizeof(addr));
    if (iResult == SOCKET_ERROR) {
        closesocket(listenSocket_);
        WSACleanup();
        return;
    }

    // listen
    iResult = listen(listenSocket_, SOMAXCONN);
    if (iResult == SOCKET_ERROR) {
        closesocket(listenSocket_);
        WSACleanup();
        return;
    }

    // 非同期モード
    u_long nonBlocking = 1;
    ioctlsocket(listenSocket_, FIONBIO, &nonBlocking);
}

void server::Update() {
    // クライアント未接続の時
    if (clientSocket_ == INVALID_SOCKET) {
        clientSocket_ = accept(listenSocket_, nullptr, nullptr);

        if (clientSocket_ != INVALID_SOCKET) {
            // クライアント用ソケットも非同期にする
            u_long nonBlocking = 1;
            ioctlsocket(clientSocket_, FIONBIO, &nonBlocking);
        }
        return;
    }

    // recv
    char buffer[256]{};
    int recvSize = recv(clientSocket_, buffer, sizeof(buffer), 0);

    if (recvSize > 0) {
        // 受信した内容をそのまま返す
        send(clientSocket_, buffer, recvSize, 0);
    }
    else if (recvSize == 0) {
        // クライアント切断
        closesocket(clientSocket_);
        clientSocket_ = INVALID_SOCKET;
    }
    else {
        // 非同期用所為
        if (WSAGetLastError() != WSAEWOULDBLOCK) {
            closesocket(clientSocket_);
            clientSocket_ = INVALID_SOCKET;
        }
    }
}

void server::Debug() {
#ifdef _DEBUG
    ImGui::Begin("Server");

    ImGui::Text("Listen: %s",
        listenSocket_ != INVALID_SOCKET ? "Active" : "Stopped");

    ImGui::Text("Client: %s",
        clientSocket_ != INVALID_SOCKET ? "Connected" : "None");

    ImGui::End();
#endif
}

void server::Finalize() {
    if (clientSocket_ != INVALID_SOCKET) {
        closesocket(clientSocket_);
        clientSocket_ = INVALID_SOCKET;
    }

    if (listenSocket_ != INVALID_SOCKET) {
        closesocket(listenSocket_);
        listenSocket_ = INVALID_SOCKET;
    }

    WSACleanup();
}
