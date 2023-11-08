#include <Novice.h>

const char kWindowTitle[] = "GC1D_03_イトウヒビキ_タイトル";

struct Vector2 {
	float x;
	float y;

};

struct Quad {
	Vector2 pos;//中心座標
	float halfWidth;
	float halfHeight;
	Vector2 leftTop;
	Vector2 rightTop;
	Vector2 leftBottom;
	Vector2 rightBottom;
	Vector2 speed;
	//Vector2 acceleration;//加速度
};

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化

	const int WIN_WIDTH = 1280;
	const int WIN_HEIGHT = 720;

	Novice::Initialize(kWindowTitle, WIN_WIDTH, WIN_HEIGHT);

	Quad player = {
		{640,360},
		32,32,
		{player.pos.x - player.halfWidth,player.pos.y - player.halfHeight},
		{player.pos.x + player.halfWidth,player.pos.y - player.halfHeight},
		{player.pos.x - player.halfWidth,player.pos.y + player.halfHeight},
		{player.pos.x + player.halfWidth,player.pos.y + player.halfHeight},
		{0,0},
		//{0,0},

	};

	int mousePosX = 0;
	int mousePosY = 0;

	int accelerateStartLine = WIN_WIDTH / 2;//略称ASL

	int furthermoreASL1 = accelerateStartLine + 213;
	int furthermoreASL2 = furthermoreASL1 + 213;
	int furthermoreASL3 = accelerateStartLine - 213;
	int furthermoreASL4 = furthermoreASL3 - 213;

	bool isPlayerInTheLeft = false;
	bool isPlayerInTheRight = false;


	unsigned int playerColor = WHITE;

	int playerAnimationTimer = 0;
	int playerAnimationIndex = 0;

	int rightTexture[4] = {
		Novice::LoadTexture("./Resources/player/PLAYER1.png"),
		Novice::LoadTexture("./Resources/player/PLAYER2.png"),
		Novice::LoadTexture("./Resources/player/PLAYER3.png"),
		Novice::LoadTexture("./Resources/player/PLAYER4.png"),
	};


	// キー入力結果を受け取る箱
	char keys[256] = { 0 };
	char preKeys[256] = { 0 };

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

		playerAnimationTimer++;

		if (playerAnimationTimer % 15 == 0) {
			playerAnimationIndex++;
			if (playerAnimationIndex > 3) {
				playerAnimationIndex = 0;
			}
		}



		if (mousePosX >= WIN_WIDTH / 2) {
			isPlayerInTheRight = true;
			isPlayerInTheLeft = false;
		}
		else if (mousePosX < WIN_WIDTH / 2) {
			isPlayerInTheRight = false;
			isPlayerInTheLeft = true;
		}

		if (isPlayerInTheLeft) {
			playerColor = RED;
		}

		if (isPlayerInTheRight) {
			playerColor = WHITE;
		}

		player.pos.x += player.speed.x;

		//player.speed.x += player.acceleration.x;

		player.leftTop.x = player.pos.x - player.halfWidth;
		player.leftTop.y = player.pos.y - player.halfHeight;

		player.rightTop.x = player.pos.x + player.halfWidth;
		player.rightTop.y = player.pos.y - player.halfHeight;


		player.leftBottom.x = player.pos.x - player.halfWidth;
		player.leftBottom.y = player.pos.y + player.halfHeight;

		player.rightBottom.x = player.pos.x + player.halfWidth;
		player.rightBottom.y = player.pos.y + player.halfHeight;



		Novice::GetMousePosition(&mousePosX, &mousePosY);

		if (mousePosX >= WIN_WIDTH / 2 && mousePosX < furthermoreASL1) {
			player.speed.x = 3;
			//player.acceleration.x = 0.1f;
		}
		else if (mousePosX >= furthermoreASL1 && mousePosX < furthermoreASL2) {
			player.speed.x = 6;
			//player.acceleration.x = 0.125f;
		}
		else if (mousePosX >= furthermoreASL2) {
			player.speed.x = 9;
			//player.acceleration.x = 0.2f;
		}
		else if (mousePosX < WIN_WIDTH / 2 && mousePosX > furthermoreASL3) {
			player.speed.x = -3;
			//player.acceleration.x = -0.1f;
		}
		else if (mousePosX <= furthermoreASL3 && mousePosX > furthermoreASL4) {
			player.speed.x = -6;
			//player.acceleration.x = -0.125f;
		}
		else if (mousePosX <= furthermoreASL4) {
			player.speed.x = -9;
			//player.acceleration.x = -0.2f;
		}

		if (player.pos.x < player.halfWidth) {

			player.speed.x = 0;
			player.pos.x = player.halfWidth;

		}
		else if (player.pos.x > WIN_WIDTH - player.halfWidth) {
			player.speed.x = 0;
			//player.acceleration.x = 0;
			player.pos.x = WIN_WIDTH - player.halfWidth;
		}


		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///

		Novice::ScreenPrintf(0, 0, "LEFT FLAG=%d", isPlayerInTheLeft);

		Novice::ScreenPrintf(0, 50, "RIGHT FLAG=%d", isPlayerInTheRight);

		Novice::DrawLine(WIN_WIDTH / 2, 0, WIN_WIDTH / 2, WIN_HEIGHT, RED);
		Novice::DrawLine(furthermoreASL1, 0, furthermoreASL1, WIN_HEIGHT, WHITE);
		Novice::DrawLine(furthermoreASL2, 0, furthermoreASL2, WIN_HEIGHT, WHITE);
		Novice::DrawLine(furthermoreASL3, 0, furthermoreASL3, WIN_HEIGHT, WHITE);
		Novice::DrawLine(furthermoreASL4, 0, furthermoreASL4, WIN_HEIGHT, WHITE);

		Novice::DrawQuad
		(
			int(player.leftTop.x),
			int(player.leftTop.y),
			int(player.rightTop.x),
			int(player.rightTop.y),
			int(player.leftBottom.x),
			int(player.leftBottom.y),
			int(player.rightBottom.x),
			int(player.rightBottom.y),
			0, 0, 16, 16, rightTexture[playerAnimationIndex], playerColor
		);

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
