#include <Novice.h>

const char kWindowTitle[] = "GC1D_03_イトウヒビキ_タイトル";

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, 1280, 720);

	// キー入力結果を受け取る箱
	char keys[256] = {0};
	char preKeys[256] = {0};

	const int World_Width = 3840;

	int background[4] = {0, 1280, 2560, 3860};

	int posX = 40;
	int posY = 560;
	int posrad = 1;
	int posW = 50;
	int posH = 50;
	int speed = 20;
	int ScrollX = posX + posrad;
	int playerScroll = posX - ScrollX;

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

		if (keys[DIK_D]) {
			posX += speed;
		}
		if (keys[DIK_A]) {
			posX -= speed;
		}

		if (posX < 0 + posrad) {
			posX = 0 + posrad;
		}
		if (posX > World_Width - posrad) {
			posX = World_Width - posrad;
		}

		ScrollX = posX + posrad;

		if (ScrollX < 0) {
			ScrollX = 0;
		}
		if (ScrollX > World_Width - 1280) {
			ScrollX = World_Width - 1280;
		}
		playerScroll = posX - ScrollX;

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

	// ライブラリの終了
	Novice::Finalize();
	return 0;
}
