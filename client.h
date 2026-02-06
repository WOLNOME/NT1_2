#pragma once
#include <winsock2.h>

class client {
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

	//接続
	bool Connect();


	SOCKET socket_ = INVALID_SOCKET;

	char sendBuffer_[256] = "";
	char recvBuffer_[256]{};

	bool isConnected_ = false;
};

