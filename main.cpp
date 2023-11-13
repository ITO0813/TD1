#include <Novice.h>

const char kWindowTitle[] = "No.5248_タイトル";

//シーンの定義
enum scene {
	titlescene,
	trialscene,
	mainscene,
	clearscene,
	gameoverscene
};

//タイトルシーン
int Titlescene(char keys[], char preKeys[]) {
	Novice::DrawBox(0, 0, 50, 50, 0.0f, RED, kFillModeSolid);

	if (!keys[DIK_SPACE] && preKeys[DIK_SPACE]) {
		return trialscene;
	} else {
		return titlescene;
	}
}

// チュートリアルシーン
int Trialscene(char keys[], char preKeys[]) {
	Novice::DrawBox(0, 0, 50, 50, 0.0f, GREEN, kFillModeSolid);

	if (!keys[DIK_SPACE] && preKeys[DIK_SPACE]) {
		return mainscene;
	} else {
		return trialscene;
	}
}

// メインシーン
int Mainscene(char keys[], char preKeys[],int mainbackgroundHandle) {
	Novice::DrawSprite(0, 0, mainbackgroundHandle, 1, 1, 0.0f, WHITE);

	if (!keys[DIK_SPACE] && preKeys[DIK_SPACE]) {
		return clearscene;
	} else {
		return mainscene;
	}
}

// クリアシーン
int Clearscene(char keys[], char preKeys[]) {
	Novice::DrawBox(0, 0, 50, 50, 0.0f, BLACK, kFillModeSolid);

	if (!keys[DIK_SPACE] && preKeys[DIK_SPACE]) {
		return gameoverscene;
	} else {
		return clearscene;
	}
}

// ゲームオーバーシーン
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

	//最初のシーンの定義
	int scene;
	scene = titlescene;

	int mainbackgroundHandle = Novice::LoadTexture("./Title1.png");

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

		//タイトルシーンの時
		if (scene==titlescene) {
			nextScene = Titlescene(keys, preKeys);
		}
		if (scene != nextScene) {
			scene = nextScene;
		}

		//チュートリアルシーンの時
		else if (scene == trialscene) {
			nextScene = Trialscene(keys, preKeys);
		}
		if (scene != nextScene) {
			scene = nextScene;
		}

		// メインシーンの時
		else if (scene == mainscene) {
			nextScene = Mainscene(keys, preKeys, mainbackgroundHandle);
		}
		if (scene != nextScene) {
			scene = nextScene;
		}

		// クリアシーンの時
		else if (scene == clearscene) {
			nextScene = Clearscene(keys, preKeys);
		}
		if (scene != nextScene) {
			scene = nextScene;
		}

		// ゲームオーバーシーンの時	
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

		Novice::ScreenPrintf(100, 100, "%d", scene);

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
