#include "client.h"
#include <ImGuiManager.h>
#include <ws2tcpip.h>

void client::Initialize() {
    WSADATA wsaData;
    int iResult;

    // Winsock 初期化
    iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0) {
        return;
    }

    // socket 作成
    socket_ = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (socket_ == INVALID_SOCKET) {
        WSACleanup();
        return;
    }

    // サーバー情報
    sockaddr_in serverAddr{};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(12345);
    inet_pton(AF_INET, "127.0.0.1", &serverAddr.sin_addr);

    // connect
    iResult = connect(socket_, (sockaddr*)&serverAddr, sizeof(serverAddr));
    if (iResult == SOCKET_ERROR) {
        closesocket(socket_);
        socket_ = INVALID_SOCKET;
        WSACleanup();
        return;
    }

    // 非同期モード
    u_long nonBlocking = 1;
    ioctlsocket(socket_, FIONBIO, &nonBlocking);

    isConnected_ = true;
}

void client::Update() {
    if (!isConnected_) {
        return;
    }

    int recvSize = recv(socket_, recvBuffer_, sizeof(recvBuffer_) - 1, 0);

    if (recvSize > 0) {
        recvBuffer_[recvSize] = '\0';
    }
    else if (recvSize == 0) {
        // サーバー切断
        closesocket(socket_);
        socket_ = INVALID_SOCKET;
        isConnected_ = false;
    }
    else {
        if (WSAGetLastError() != WSAEWOULDBLOCK) {
            closesocket(socket_);
            socket_ = INVALID_SOCKET;
            isConnected_ = false;
        }
    }
}

void client::Debug() {
#ifdef _DEBUG
    ImGui::Begin("Client Chat");

    ImGui::Text("Status: %s", isConnected_ ? "Connected" : "Disconnected");

    ImGui::InputText("Message", sendBuffer_, sizeof(sendBuffer_));

    // Send（接続中のみ）
    ImGui::BeginDisabled(!isConnected_);
    if (ImGui::Button("Send")) {
        send(socket_, sendBuffer_, (int)strlen(sendBuffer_), 0);
    }
    ImGui::EndDisabled();

    ImGui::SameLine();

    // Connect / Disconnect 切り替え
    if (isConnected_) {
        if (ImGui::Button("Disconnect")) {
            closesocket(socket_);
            socket_ = INVALID_SOCKET;
            isConnected_ = false;
            recvBuffer_[0] = '\0';
        }
    }
    else {
        if (ImGui::Button("Connect")) {
            Connect();
        }
    }

    ImGui::Separator();
    ImGui::Text("Reply from Server:");
    ImGui::TextWrapped("%s", recvBuffer_);

    ImGui::End();
#endif
}

void client::Finalize() {
    if (socket_ != INVALID_SOCKET) {
        closesocket(socket_);
        socket_ = INVALID_SOCKET;
    }

    WSACleanup();
}

bool client::Connect() {
    if (isConnected_) {
        return true;
    }

    socket_ = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (socket_ == INVALID_SOCKET) {
        return false;
    }

    sockaddr_in serverAddr{};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(12345);
    InetPton(AF_INET, L"127.0.0.1", &serverAddr.sin_addr);

    if (connect(socket_, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        closesocket(socket_);
        socket_ = INVALID_SOCKET;
        return false;
    }

    u_long nonBlocking = 1;
    ioctlsocket(socket_, FIONBIO, &nonBlocking);

    isConnected_ = true;
    return true;
}