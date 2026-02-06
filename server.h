#pragma once
#include <winsock2.h>

class server {
public:
	//初期化
	void Initialize();
	//更新
	void Update();
	//デバッグ
	void Debug();
	//終了
	void Finalize();

private:
	SOCKET listenSocket_ = INVALID_SOCKET;
	SOCKET clientSocket_ = INVALID_SOCKET;
};


////ImGui
//{
//	static bool isConnected = true;
//	static char sendBuffer[256] = "Hello";
//	static std::string serverReply = "Server received: Hello";

//	if (ImGui::CollapsingHeader("Client Chat", ImGuiTreeNodeFlags_DefaultOpen)) {
//		// 接続状態表示
//		if (isConnected) {
//			ImGui::TextColored(ImVec4(0.2f, 1.0f, 0.2f, 1.0f), "Connected");
//		}
//		else {
//			ImGui::TextColored(ImVec4(1.0f, 0.2f, 0.2f, 1.0f), "Disconnected");
//		}

//		ImGui::Spacing();

//		// メッセージ入力
//		ImGui::PushItemWidth(200.0f);
//		ImGui::InputText("##MessageInput", sendBuffer, sizeof(sendBuffer));
//		ImGui::PopItemWidth();

//		ImGui::SameLine();
//		ImGui::Text("Message");

//		ImGui::Spacing();

//		// Send ボタン
//		if (ImGui::Button("Send")) {
//			if (isConnected) {
//				// 本来はここで send()
//				serverReply = std::string("Server received: ") + sendBuffer;
//			}
//		}

//		ImGui::SameLine();

//		// Disconnect ボタン
//		if (ImGui::Button("Disconnect")) {
//			isConnected = false;
//		}

//		ImGui::Spacing();
//		ImGui::Separator();
//		ImGui::Spacing();

//		// サーバー返信表示
//		ImGui::Text("Reply from Server:");
//		ImGui::TextWrapped("%s", serverReply.c_str());
//	}
//}
