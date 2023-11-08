#include <Novice.h>

const char kWindowTitle[] = "No.5248_タイトル";

enum scene {
	titlescene,
	mainscene,
	clearscene,
	gameoverscene
};

int Titlescene(char keys[], char preKeys[]) {
	Novice::DrawBox(0, 0, 50, 50, 0.0f, RED, kFillModeSolid);

	if (!keys[DIK_SPACE] && preKeys[DIK_SPACE]) {
		return mainscene;
	} else {
		return titlescene;
	}
}

int Mainscene(char keys[], char preKeys[]) {
	Novice::DrawBox(0, 0, 50, 50, 0.0f, WHITE, kFillModeSolid);

	if (!keys[DIK_SPACE] && preKeys[DIK_SPACE]) {
		return clearscene;
	} else {
		return mainscene;
	}
}

int Clearscene(char keys[], char preKeys[]) {
	Novice::DrawBox(0, 0, 50, 50, 0.0f, BLACK, kFillModeSolid);

	if (!keys[DIK_SPACE] && preKeys[DIK_SPACE]) {
		return gameoverscene;
	} else {
		return clearscene;
	}
}

int Gameoverscene(char keys[], char preKeys[]) {
	Novice::DrawBox(0, 0, 50, 50, 0.0f, BLUE, kFillModeSolid);

	if (!keys[DIK_SPACE] && preKeys[DIK_SPACE]) {
		return titlescene;
	} else {
		return gameoverscene;
	}
}

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, 1280, 720);

	// キー入力結果を受け取る箱
	char keys[256] = {0};
	char preKeys[256] = {0};

	int scene;
	scene = titlescene;

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

		int nextScene = scene;

		if (scene==titlescene) {
			nextScene = Titlescene(keys, preKeys);
		}
		if (scene != nextScene) {
			scene = nextScene;
		}

		else if (scene == mainscene) {
			nextScene = Mainscene(keys, preKeys);
		}
		if (scene != nextScene) {
			scene = nextScene;
		}

		else if (scene == clearscene) {
			nextScene = Clearscene(keys, preKeys);
		}
		if (scene != nextScene) {
			scene = nextScene;
		}

		else if (scene == gameoverscene) {
			nextScene = Gameoverscene(keys, preKeys);
		}
		if (scene != nextScene) {
			scene = nextScene;
		}
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
