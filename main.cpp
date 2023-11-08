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

//スクリーン座標変換用の関数
Vector2 ToScreen(Vector2 world)
{
	const Vector2 kworldToScreenTranslate = { 0.0f,720.0f };
	const Vector2 kworldToScreenScale = { 1.0f,-1.0f };
	return
	{
		(world.x * kworldToScreenScale.x) + kworldToScreenTranslate.x,
		(world.y * kworldToScreenScale.y) + kworldToScreenTranslate.y
	};
}

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	const int WIN_WIDTH = 1280;
	const int WIN_HEIGHT = 720;

	Novice::Initialize(kWindowTitle, WIN_WIDTH, WIN_HEIGHT);

	Quad player = {
		{640,32},
		32,32,
		{player.pos.x - player.halfWidth,player.pos.y + player.halfHeight},
		{player.pos.x + player.halfWidth,player.pos.y + player.halfHeight},
		{player.pos.x - player.halfWidth,player.pos.y - player.halfHeight},
		{player.pos.x + player.halfWidth,player.pos.y - player.halfHeight},
		{0,0},
		//{0,0},

	};

	//スクリーン座標に変換した値を格納する変数
	Vector2 scsPlayerPos = ToScreen(player.pos);
	Vector2 scsLeftTop;
	Vector2 scsRightTop;
	Vector2 scsLeftBottom;
	Vector2 scsRightBottom;

	//マウスカーソルの座標取得用変数
	int mousePosX = 0;
	int mousePosY = 0;

	//画面中央を基準としてスピードを変える
	int accelerateStartLine = WIN_WIDTH / 2;//略称ASL

	//画面左エリア
	int furthermoreASL1 = accelerateStartLine + 213;
	int furthermoreASL2 = furthermoreASL1 + 213;

	//画面右エリア
	int furthermoreASL3 = accelerateStartLine - 213;
	int furthermoreASL4 = furthermoreASL3 - 213;

	//マウスカーソルが中央線に対して左右どちらにいるかを判別する為のフラグ
	bool isMouseInTheLeft = false;
	bool isMouseInTheRight = false;

	unsigned int playerColor = WHITE;

	//インデックスの値を変える間隔を調整するための変数
	int playerAnimationTimer = 0;

	//インデックスの値によって描画される画像が変わる
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

		//15フレーム毎に画像が切り替わる
		if (playerAnimationTimer % 15 == 0) {
			playerAnimationIndex++;
			if (playerAnimationIndex > 3) {
				playerAnimationIndex = 0;
			}
		}



		if (mousePosX >= WIN_WIDTH / 2) {
			isMouseInTheRight = true;
			isMouseInTheLeft = false;
		}
		else if (mousePosX < WIN_WIDTH / 2) {
			isMouseInTheRight = false;
			isMouseInTheLeft = true;
		}

		//マウスカーソルの位置によって色が変わる
		if (isMouseInTheLeft) {
			playerColor = RED;
		}
		if (isMouseInTheRight) {
			playerColor = WHITE;
		}

		player.pos.x += player.speed.x;

		player.leftTop.x = player.pos.x - player.halfWidth;
		player.leftTop.y = player.pos.y + player.halfHeight;

		player.rightTop.x = player.pos.x + player.halfWidth;
		player.rightTop.y = player.pos.y + player.halfHeight;


		player.leftBottom.x = player.pos.x - player.halfWidth;
		player.leftBottom.y = player.pos.y - player.halfHeight;

		player.rightBottom.x = player.pos.x + player.halfWidth;
		player.rightBottom.y = player.pos.y - player.halfHeight;

		//マウスの位置を取得する
		Novice::GetMousePosition(&mousePosX, &mousePosY);

		if (mousePosX >= WIN_WIDTH / 2 && mousePosX < furthermoreASL1) {
			player.speed.x = 3;
		}
		else if (mousePosX >= furthermoreASL1 && mousePosX < furthermoreASL2) {
			player.speed.x = 6;

		}
		else if (mousePosX >= furthermoreASL2) {
			player.speed.x = 9;

		}
		else if (mousePosX < WIN_WIDTH / 2 && mousePosX > furthermoreASL3) {
			player.speed.x = -3;

		}
		else if (mousePosX <= furthermoreASL3 && mousePosX > furthermoreASL4) {
			player.speed.x = -6;

		}
		else if (mousePosX <= furthermoreASL4) {
			player.speed.x = -9;

		}

		//プレイヤーが画面外にいかないようにする処理
		if (player.pos.x < player.halfWidth) {
			player.speed.x = 0;
			player.pos.x = player.halfWidth;

		}
		else if (player.pos.x > WIN_WIDTH - player.halfWidth) {
			player.speed.x = 0;
			player.pos.x = WIN_WIDTH - player.halfWidth;
		}

		//中心座標と各頂点をスクリーン座標に変換
		ToScreen(player.pos);
		scsLeftTop = ToScreen(player.leftTop);
		scsRightTop = ToScreen(player.rightTop);
		scsLeftBottom = ToScreen(player.leftBottom);
		scsRightBottom = ToScreen(player.rightBottom);

		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///

		Novice::ScreenPrintf(0, 0, "LEFT FLAG=%d", isMouseInTheLeft);

		Novice::ScreenPrintf(0, 50, "RIGHT FLAG=%d", isMouseInTheRight);

		Novice::DrawLine(WIN_WIDTH / 2, 0, WIN_WIDTH / 2, WIN_HEIGHT, RED);
		Novice::DrawLine(furthermoreASL1, 0, furthermoreASL1, WIN_HEIGHT, WHITE);
		Novice::DrawLine(furthermoreASL2, 0, furthermoreASL2, WIN_HEIGHT, WHITE);
		Novice::DrawLine(furthermoreASL3, 0, furthermoreASL3, WIN_HEIGHT, WHITE);
		Novice::DrawLine(furthermoreASL4, 0, furthermoreASL4, WIN_HEIGHT, WHITE);

		Novice::DrawQuad
		(
			int(scsLeftTop.x),
			int(scsLeftTop.y),
			int(scsRightTop.x),
			int(scsRightTop.y),
			int(scsLeftBottom.x),
			int(scsLeftBottom.y),
			int(scsRightBottom.x),
			int(scsRightBottom.y),
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
