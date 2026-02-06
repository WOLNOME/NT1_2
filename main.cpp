#include <Novice.h>
#include <memory>
#include "server.h"
#include "client.h"

const char kWindowTitle[] = "LE3B_04_ウシオユウキ_Winsock";

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, 1280, 720);

	// キー入力結果を受け取る箱
	char keys[256] = { 0 };
	char preKeys[256] = { 0 };

	//生成
	std::unique_ptr<server> server_ = nullptr;
	server_ = std::make_unique<server>();
	std::unique_ptr<client> client_ = nullptr;
	client_ = std::make_unique<client>();
	
	//初期化
	server_->Initialize();
	client_->Initialize();

	// ウィンドウの×ボタンが押されるまでループ
	while (Novice::ProcessMessage() == 0) {
		// フレームの開始
		Novice::BeginFrame();

		// キー入力を受け取る
		memcpy(preKeys, keys, 256);
		Novice::GetHitKeyStateAll(keys);

		///
		/// ↓更新処理ここから
		///

		//更新
		server_->Update();
		client_->Update();

		//デバッグ
		server_->Debug();
		client_->Debug();

		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///

		///
		/// ↑描画処理ここまで
		///

		// フレームの終了
		Novice::EndFrame();

		// ESCキーが押されたらループを抜ける
		if (preKeys[DIK_ESCAPE] == 0 && keys[DIK_ESCAPE] != 0) {
			break;
		}
	}

	//終了
	client_->Finalize();
	server_->Finalize();

	// ライブラリの終了
	Novice::Finalize();
	return 0;
}
