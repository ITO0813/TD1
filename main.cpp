#include <Novice.h>
#include <Vector2.h>

const char kWindowTitle[] = "GC1D_03_イトウヒビキ_タイトル";

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, 1280, 720);

	// キー入力結果を受け取る箱
	char keys[256] = {0};
	char preKeys[256] = {0};

	int mapY = 24; // マップ縦幅
	int mapX = 40; // マップ横幅
	// 当たり判定の変数
	int leftBottomX = 0;
	int leftBottomY = 0;
	int leftTopX = 0;
	int leftTopY = 0;
	int rightBottomX = 0;
	int rightBottomY = 0;

	int blockImges1 = Novice::LoadTexture("./block4.png"); // ブロックの画像 　　マップ番号が1の時
	int blockImges2 = Novice::LoadTexture("./Obstacle.png"); // ブロックの画像　 マップ番号が2の時
	int blockSize = 32;

	int rightTopX = 0;
	int rightTopY = 0;
	int isJump = 0;
	int isFly = 0;

	struct Ball {
		Vector2 position;
		Vector2 speed;
		Vector2 jump;

		float radius;
		unsigned int color;
	};

	Ball ball = {
	    {70.0f, 400.0f},
	    {0.0f,  0.0f  },
	    {0.0f,  1.8f  },

	    10.0f,
	    WHITE
    };

	int map[24][40] = {
	    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 2, 2, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 2, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	};

	int time = 999;

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

		time -= 1;

		if (time == 0) {
			break;
		}
		
		// 当たり判定の変数
		leftBottomX = ((int)ball.position.x - (int)ball.radius) / blockSize;
		leftBottomY = ((int)ball.position.y + (int)ball.radius - 1) / blockSize;

		rightBottomX = ((int)ball.position.x + (int)ball.radius - 1) / blockSize;
		rightBottomY = ((int)ball.position.y + (int)ball.radius - 1) / blockSize;

		rightTopX = ((int)ball.position.x + (int)ball.radius - 1) / blockSize;
		rightTopY = ((int)ball.position.y - (int)ball.radius) / blockSize;
		if (map[rightTopY][rightTopX] == 0 && map[rightBottomY][rightBottomX] == 0) {
			ball.speed.x = 2;
			ball.position.x = ball.position.x + ball.speed.x; // 本体を進める
		}

		if (isJump == 0) {
			if (keys[DIK_SPACE] && preKeys[DIK_SPACE] == 0) {
				ball.speed.y = -5.0f; // ジャンプ
				isJump = 1;
			}
		} else if (isFly == 0 && isJump == 1) {
			if (preKeys[DIK_SPACE] && keys[DIK_SPACE]) {
				ball.jump.y = 0.1f;
				isFly = 1;
			} else {
				ball.jump.y = 0.8f;
			}
		}

		ball.speed.y += ball.jump.y; // 重力を加算

		ball.position.y += ball.speed.y; // プレイヤーに重力を加算
		rightBottomX = ((int)ball.position.x + (int)ball.radius - 1) / blockSize;
		rightBottomY =
		    ((int)ball.position.y + (int)ball.radius - 1 + (int)ball.speed.y) / blockSize;

		leftBottomX = ((int)ball.position.x - (int)ball.radius) / blockSize;
		leftBottomY = ((int)ball.position.y + (int)ball.radius - 1 + (int)ball.speed.y) / blockSize;

		rightTopX = ((int)ball.position.x + (int)ball.radius - 1) / blockSize;
		rightTopY = ((int)ball.position.y - (int)ball.radius + (int)ball.speed.y) / blockSize;
		leftTopX = ((int)ball.position.x - (int)ball.radius) / blockSize;
		leftTopY = ((int)ball.position.y - (int)ball.radius + (int)ball.speed.y) / blockSize;

		if (map[rightTopY][rightTopX] != 0 && map[leftTopY][leftTopX] != 0) {

			ball.speed.y = 0.0f;
			isJump = 0;
			isFly = 0;
		}
		if (map[rightBottomY][rightBottomX] != 0 && map[leftBottomY][leftBottomX] != 0) {
			ball.position.y = (rightBottomY * blockSize) - ball.radius;
			ball.speed.y = 0.0f;
			isJump = 0;
			isFly = 0;
		}

		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///
		// Novice::DrawSprite(0, 0, grapHandle, 1, 1, 0.0f, WHITE);
		Novice::DrawEllipse(
		    (int)ball.position.x, (int)ball.position.y, (int)ball.radius, (int)ball.radius, 0.0f,
		    RED, kFillModeSolid);
		// Novice::DrawSprite((int)ball.position.x, (int)ball.position.y, playerImage, 1, 1, 0.0f,
		// kFillModeSolid);//プレイヤー
		for (int i = 0; i < mapY; i++) {
			for (int p = 0; p < mapX; p++) {
				if (map[i][p] == 1) { // マップ番号が1の時
					Novice::DrawSprite(
					    (p * blockSize), i * blockSize, blockImges1, 1.0f, 1.0f, 0.0f, WHITE);
				}
				if (map[i][p] == 2) { // マップ番号が2の時
					Novice::DrawSprite(
					    (p * blockSize), i * blockSize, blockImges2, 1.0f, 1.0f, 0.0f, WHITE);
				}
			}
		}
		
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
