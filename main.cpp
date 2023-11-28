#include<Novice.h>
#include <stdlib.h>
#include <time.h>

const char kWindowTitle[] = "GC1D_03_イトウヒビキ_タイトル";

struct Vector2 {
	float x;
	float y;
};

struct Quad {
	Vector2 pos; // 中心座標
	float halfWidth;
	float halfHeight;
	float radius;
	Vector2 speed;
	Vector2 jump;
	Vector2 leftTop;
	Vector2 rightTop;
	Vector2 leftBottom;
	Vector2 rightBottom;
	Vector2 speed1;
	Vector2 speed2;
	Vector2 speed3;

	Vector2 leftBottomX;
	Vector2 leftBottomY;
	Vector2 rightBottomX;
	Vector2 rightBottomY;
	Vector2 rightTopX;
	Vector2 rightTopY;
	Vector2 leftTopX;
	Vector2 leftTopY;
};

// スクリーン座標変換用の関数
// Vector2 ToScreen(Vector2 world) {
//	const Vector2 kworldToScreenTranslate = {0.0f, 720.0f};
//	const Vector2 kworldToScreenScale = {1.0f, -1.0f};
//	return {
//	    (world.x * kworldToScreenScale.x) + kworldToScreenTranslate.x,
//	    (world.y * kworldToScreenScale.y) + kworldToScreenTranslate.y};
//}

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	const int WIN_WIDTH = 1280;
	const int WIN_HEIGHT = 720;

	Novice::Initialize(kWindowTitle, WIN_WIDTH, WIN_HEIGHT);

	Quad player = {
	    {640, 580},
	    32,
	    32,
	    10.0f,
	    {0.0f, 0.0f},
	    {0.0f, 1.8f},
	    {player.pos.x - player.halfWidth, player.pos.y + player.halfHeight},
	    {player.pos.x + player.halfWidth, player.pos.y + player.halfHeight},
	    {player.pos.x - player.halfWidth, player.pos.y - player.halfHeight},
	    {player.pos.x + player.halfWidth, player.pos.y - player.halfHeight},
	    {3, 0},
	    {6, 0},
	    {9, 0},

	    {(player.pos.x - player.radius) / 32},
	    {(player.pos.y + player.radius - 1) / 32},
	    {(player.pos.x + player.radius - 1) / 32},
	    {(player.pos.y + player.radius - 1) / 32},
	    {(player.pos.x + player.radius - 1) / 32},
	    {(player.pos.y - player.radius) / 32},
	    {(player.pos.x - player.radius) / 32},
	    {(player.pos.y - player.radius) / 32},
	};

	float playerAcceleration = 0;

	// スクリーン座標に変換した値を格納する変数
	// Vector2 scsPlayerPos = ToScreen(player.pos);
	// Vector2 scsLeftTop = ToScreen(player.leftTop);
	// Vector2 scsRightTop = ToScreen(player.rightTop);
	// Vector2 scsLeftBottom = ToScreen(player.leftBottom);
	// Vector2 scsRightBottom = ToScreen(player.rightBottom);
	//
	// Vector2 scsLeftBottomX=ToScreen(player.leftBottomX);
	// Vector2 scsLeftBottomY=ToScreen(player.leftBottomY);
	// Vector2 scsLeftTopX = ToScreen(player.leftTopX);
	// Vector2 scsLeftTopY = ToScreen(player.leftTopY);
	// Vector2 scsRightBottomX = ToScreen(player.rightBottomX);
	// Vector2 scsRightBottomY = ToScreen(player.rightBottomY);
	// Vector2 scsRightTopX=ToScreen(player.rightTopX);
	// Vector2 scsRightTopY=ToScreen(player.rightTopY);

	// 一度に存在できる粒子の最大数
	const int ELLIPSE_NUM_MAX = 500;

	struct Particle {
		Vector2 center;
		int radius;
		Vector2 speed;
		bool isAppear;
		int moveTimer;
		Vector2 acceleration;
	};

	Particle particle[ELLIPSE_NUM_MAX];
	for (int i = 0; i < ELLIPSE_NUM_MAX; i++) {
		particle[i].center.x = player.leftBottom.x;
		particle[i].center.y = player.leftBottom.y;
		particle[i].radius = 2;
		particle[i].speed.x = 0;
		particle[i].speed.y = 0;
		particle[i].isAppear = false;
		particle[i].moveTimer = 30;
		particle[i].acceleration.x = 0.2f;
		particle[i].acceleration.y = 0.2f;
	}

	int particleCoolTimer = 10;

	unsigned int currentTime = int(time(nullptr));
	srand(currentTime);

	// マウスカーソルの座標取得用変数
	int mousePosX = 0;
	int mousePosY = 0;

	const int lineWide = 213;

	// 画面中央を基準としてスピードを変える
	int accelerateStartLine = WIN_WIDTH / 2; // 略称ASL

	// 画面左エリア
	int furthermoreASL1 = accelerateStartLine + lineWide;
	int furthermoreASL2 = furthermoreASL1 + lineWide;

	// 画面右エリア
	int furthermoreASL3 = accelerateStartLine - lineWide;
	int furthermoreASL4 = furthermoreASL3 - lineWide;

	// マウスカーソルが中央線に対して左右どちらにいるかを判別する為のフラグ
	bool isMouseInTheLeft = false;
	bool isMouseInTheRight = false;

	unsigned int playerColor = WHITE;

	enum Direction {
		LEFT,  // 0
		RIGHT, // 1
	};

	// プレイヤーの右向き画像
	int rightTexture[4] = {
	    Novice::LoadTexture("./Resources/player/PLAYER1.png"),
	    Novice::LoadTexture("./Resources/player/PLAYER2.png"),
	    Novice::LoadTexture("./Resources/player/PLAYER3.png"),
	    Novice::LoadTexture("./Resources/player/PLAYER4.png"),
	};
	int playerRights[4] = {rightTexture[0], rightTexture[1], rightTexture[2], rightTexture[3]};

	// プレイヤーの左向き画像(後で左向きの画像に差し替え)
	int leftTexture[4] = {
	    Novice::LoadTexture("./Resources/player/PLAYER1.png"),
	    Novice::LoadTexture("./Resources/player/PLAYER2.png"),
	    Novice::LoadTexture("./Resources/player/PLAYER3.png"),
	    Novice::LoadTexture("./Resources/player/PLAYER4.png"),
	};
	// マップチップ、ブロック画像
	int blockImges1 = Novice::LoadTexture("./block4.png"); // ブロックの画像 　　マップ番号が1の時
	int blockImges2 = Novice::LoadTexture("./Obstacle.png"); // 障害物の画像　 マップ番号が2の時
	int blockSize = 32;

	// 変数
	int mapY = 24; // マップ縦幅
	int mapX = 40; // マップ横幅

	// 当たり判定の変数
	int leftBottomX = 0;
	int leftBottomY = 0;
	int leftTopX = 0;
	int leftTopY = 0;
	int rightBottomX = 0;
	int rightBottomY = 0;

	int rightTopX = 0;
	int rightTopY = 0;
	int isJump = 0;
	int isFly = 0;

	int Overflag = true; // ゲームオーバーフラグ

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
	    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 1},
	    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 2, 2, 2, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 2, 2, 2, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	    {1, 0, 0, 2, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	};

	int playerLefts[4] = {leftTexture[0], leftTexture[1], leftTexture[2], leftTexture[3]};

	int playerDirection = Direction::RIGHT; // プレイヤーの向き

	int playerCurrentTexture = playerRights[0]; // 現在のプレイヤーの画像

	// インデックスの値を変える間隔を調整するための変数
	int playerAnimationTimer = 0;

	// インデックスの値によって描画される画像が変わる
	int playerAnimationIndex = 0;

	// 残像処理用変数
	const int afterImageLength = 10;
	int isDraw[afterImageLength] = {};      // 残像が出ているかの管理フラグ
	int afterImageX[afterImageLength] = {}; // 各残像のX座標
	int timer = 0;

	// キー入力結果を受け取る箱
	char keys[256] = {0};
	char preKeys[256] = {0};

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

		// 当たり判定の変数

		//===========================================================================
		// 自機の更新処理
		//===========================================================================
		playerAnimationTimer++;

		// 15フレーム毎に画像が切り替わる
		if (playerAnimationTimer % 15 == 0) {
			playerAnimationIndex++;
			if (playerAnimationIndex > 3) {
				playerAnimationIndex = 0;
			}
		}

		// 中心座標と各頂点をスクリーン座標に変換
		// scsPlayerPos = ToScreen(player.pos);
		// scsLeftTop = ToScreen(player.leftTop);
		// scsRightTop = ToScreen(player.rightTop);
		// scsLeftBottom = ToScreen(player.leftBottom);
		// scsRightBottom = ToScreen(player.rightBottom);
		// scsLeftBottomX = ToScreen(player.leftBottomX);
		// scsLeftBottomY = ToScreen(player.leftBottomY);
		// scsLeftTopX = ToScreen(player.leftTopX);
		// scsLeftTopY = ToScreen(player.leftTopY);
		// scsRightBottomX = ToScreen(player.rightBottomX);
		// scsRightBottomY = ToScreen(player.rightBottomY);
		// scsRightTopX = ToScreen(player.rightTopX);
		// scsRightTopY = ToScreen(player.rightTopY);

		// マウスの座標が画面中央よりどちらにいるかによって方向管理フラグが切り替わる
		if (mousePosX >= WIN_WIDTH / 2) {
			isMouseInTheRight = true;
			isMouseInTheLeft = false;
			playerDirection = Direction::RIGHT;
		} else if (mousePosX < WIN_WIDTH / 2) {
			isMouseInTheRight = false;
			isMouseInTheLeft = true;
			playerDirection = Direction::LEFT;
		}

		// プレイヤーの描画向き切り替え処理
		if (playerDirection == Direction::RIGHT) {
			playerCurrentTexture = playerRights[playerAnimationIndex];
		} else if (playerDirection == Direction::LEFT) {
			playerCurrentTexture = playerLefts[playerAnimationIndex];
		}

		// マウスカーソルの位置によって色が変わる
		if (isMouseInTheLeft) {
			playerColor = RED;
		}
		if (isMouseInTheRight) {
			playerColor = WHITE;
		}

		// 各頂点座標を中心座標を基に毎フレーム更新
		player.leftTop.x = player.pos.x - player.halfWidth;
		player.leftTop.y = player.pos.y + player.halfHeight;

		player.rightTop.x = player.pos.x + player.halfWidth;
		player.rightTop.y = player.pos.y + player.halfHeight;

		player.leftBottom.x = player.pos.x - player.halfWidth;
		player.leftBottom.y = player.pos.y - player.halfHeight;

		player.rightBottom.x = player.pos.x + player.halfWidth;
		player.rightBottom.y = player.pos.y - player.halfHeight;

		// マウスの位置を取得する
		Novice::GetMousePosition(&mousePosX, &mousePosY);

		// マウスの座標によってスピードが変わる
		if (mousePosX >= WIN_WIDTH / 2 && mousePosX < furthermoreASL1) {
			player.pos.x += player.speed1.x;
		} else if (mousePosX >= furthermoreASL1 && mousePosX < furthermoreASL2) {
			player.pos.x += player.speed2.x;
		} else if (mousePosX >= furthermoreASL2) {
			player.pos.x += player.speed3.x;
		}

		// ここから左方向
		else if (mousePosX < WIN_WIDTH / 2 && mousePosX > furthermoreASL3) {
			player.pos.x -= player.speed1.x;
		} else if (mousePosX <= furthermoreASL3 && mousePosX > furthermoreASL4) {
			player.pos.x -= player.speed2.x;

		} else if (mousePosX <= furthermoreASL4) {
			player.pos.x -= player.speed3.x;
		}

		if (keys[DIK_SPACE] && preKeys[DIK_SPACE] == 0) {
			player.speed1.y = -20.0f;
			//playerAcceleration = -0.8f; // ジャンプ処理
		}

		// プレイヤーが画面外にいかないようにする処理
		if (player.pos.x < player.halfWidth) {

			player.pos.x = player.halfWidth;

		} else if (player.pos.x > WIN_WIDTH - player.halfWidth) {

			player.pos.x = WIN_WIDTH - player.halfWidth;
		}

		if (player.pos.y < player.halfHeight) {
			player.speed1.y = 0;
			playerAcceleration = 0;
			player.pos.y = player.halfHeight;
		}

		//player.speed1.y += playerAcceleration;

		player.speed1.y += player.jump.y; // 重力を加算
		player.pos.y += player.speed1.y;

		if (map[rightTopY][rightTopX] == 0 && map[rightBottomY][rightBottomX] == 0) {
			player.speed.x = 2;
			player.pos.x = player.pos.x + player.speed.x; // 本体を進める
		}



		//if (isJump == 0) {
		//	if (keys[DIK_SPACE] && preKeys[DIK_SPACE] == 0) {
		//		player.jump.y = 1.8f; // ジャンプ
		//		isJump = 1;
		//	}
		//} else if (isFly == 0 && isJump == 1) {
		//	if (preKeys[DIK_SPACE] && keys[DIK_SPACE]) {
		//		player.jump.y = 0.5f;
		//		isFly = 1;
		//	}
		//}


		 //player.pos.y += player.speed.y;//プレイヤーに重力を加算

		// rightBottomX = ((int)player.pos.x + (int)player.radius - 1) / blockSize;
		// rightBottomY = ((int)player.pos.y + (int)player.radius - 1 + (int)player.speed.y) /
		// blockSize; leftBottomX = ((int)player.pos.x - (int)player.radius) / blockSize;
		// leftBottomY = ((int)player.pos.y + (int)player.radius - 1 + (int)player.speed.y) /
		// blockSize; rightTopX = ((int)player.pos.x + (int)player.radius - 1) / blockSize;
		// rightTopY = ((int)player.pos.y - (int)player.radius + (int)player.speed.y) / blockSize;
		// leftTopX = ((int)player.pos.x - (int)player.radius) / blockSize;
		// leftTopY = ((int)player.pos.y - (int)player.radius + (int)player.speed.y) / blockSize;

		rightBottomX = (int)player.rightBottom.x / blockSize;
		rightBottomY = ((int)player.rightBottom.y + (int)player.speed.y) / blockSize;
		leftBottomX = (int)player.leftBottom.x / blockSize;
		leftBottomY = ((int)player.leftBottom.y + (int)player.speed.y) / blockSize;
		rightTopX = (int)player.rightTop.x / blockSize;
		rightTopY = ((int)player.rightTop.y + (int)player.speed.y) / blockSize;
		leftTopX = (int)player.leftTop.x / blockSize;
		leftTopY = ((int)player.leftTop.y + (int)player.speed.y) / blockSize;

		// 衝突判定
		if (map[leftTopY][leftTopX] != 0 && map[rightTopY][rightTopX] != 0) {
			player.speed.y = 0.0f;
			isJump = 0;
			isFly = 0;
		}
		if (map[rightBottomY][rightBottomX] != 0 && map[leftBottomY][leftBottomX] != 0) {
			player.speed.y = (rightBottomY * blockSize) - player.radius;
			player.speed.y = 0.0f;
			isJump = 0;
			isFly = 0;
		}

		if (map[leftTopY][leftTopX] == 2 && map[rightTopY][rightTopX]) {

			player.speed.y = 0.0f;
			isJump = 0;
			isFly = 0;
			Overflag = false; // ゲームオーバーフラグ
		}
		if (map[rightBottomY][rightBottomX] == 2 && map[leftBottomY][leftBottomX] == 2) {
			player.speed.y = (rightBottomY * blockSize) - player.radius;
			player.speed.y = 0.0f;
			isJump = 0;
			isFly = 0;
			Overflag = false; // ゲームオーバーフラグ
		}
		if (Overflag == false) { // ゲームオーバーフラグ
			player.pos.x = 200;
			player.pos.y = 200;
		}

		//===========================================================================
		// パーティクルの更新処理
		//===========================================================================

		if (particleCoolTimer > 0) {
			particleCoolTimer--;
		} else {
			particleCoolTimer = 2; // 1秒あたり30個の粒子が出る
		}
		// particleCoolTimerが0になった時i番目の粒子が出ていなかったらプレイヤーの足元に出現させる
		if (particleCoolTimer <= 0) {
			for (int i = 0; i < ELLIPSE_NUM_MAX; i++) {

				if (particle[i].isAppear == false) {

					particle[i].isAppear = true;

					// パーティクルの発生位置を足元に合わせる
					particle[i].center.x = player.leftTop.x + 30;
					particle[i].center.y = player.leftTop.y;
					break;
				}
			}
		}

		for (int i = 0; i < ELLIPSE_NUM_MAX; i++) {

			if (particle[i].isAppear == true) {

				// 粒子1個あたりの存在出来る時間(0.5秒)
				if (particle[i].moveTimer > 0) {
					particle[i].moveTimer--;
				} else if (particle[i].moveTimer == 0) {
					particle[i].moveTimer = 30;
				}

				particle[i].speed.x = float(rand() % 4);
				particle[i].speed.y = float(rand() % 5 - 0);

				// マウスの座標が中央より右に位置するときプレイヤの進行方向の反対側に向かってパーティクルが発生する
				if (isMouseInTheRight == true) {
					particle[i].center.x -= particle[i].speed.x;
					particle[i].center.y -= particle[i].speed.y;
				} else { // 左も同様
					particle[i].center.x += particle[i].speed.x;
					particle[i].center.y -= particle[i].speed.y;
				}
			}

			// ムーブタイマーが0になったら粒子を消す
			if (particle[i].moveTimer == 0) {
				particle[i].isAppear = false;

				particle[i].speed.x = 0;

				particle[i].speed.y = 0;
			}
		}

		//===========================================================================
		// 残像の更新処理
		//===========================================================================
		timer++;

		if (timer > 80) {
			timer = 0;
		}

		for (int i = 0; i <= afterImageLength; i++) {
			if (timer == 8 * i) {

				//afterImageX[i] = (int)player.leftBottom.x;
				//isDraw[i] = 1;

				isDraw[i - 1] = 0;
			}
		}
		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///
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
		Novice::DrawEllipse(
		    (int)player.pos.x, (int)player.pos.y, (int)player.radius, (int)player.radius, 0.0f, RED,
		    kFillModeSolid);

		Novice::ScreenPrintf(0, 0, "timer=%d", timer);

		Novice::DrawLine(WIN_WIDTH / 2, 0, WIN_WIDTH / 2, WIN_HEIGHT, RED);
		Novice::DrawLine(furthermoreASL1, 0, furthermoreASL1, WIN_HEIGHT, WHITE);
		Novice::DrawLine(furthermoreASL2, 0, furthermoreASL2, WIN_HEIGHT, WHITE);
		Novice::DrawLine(furthermoreASL3, 0, furthermoreASL3, WIN_HEIGHT, WHITE);
		Novice::DrawLine(furthermoreASL4, 0, furthermoreASL4, WIN_HEIGHT, WHITE);

		// 残像
		for (int i = 0; i < afterImageLength; i++) {
			if (isDraw[i] == 1) {
				Novice::DrawSpriteRect(
				    int(afterImageX[i]), int(player.leftBottom.y), 0, 0, 16, 16,
				    playerCurrentTexture, 4, 4, 0.0f, BLUE);
			}
		}

		// プレイヤー
		Novice::DrawQuad(
		    int(player.leftBottom.x), int(player.leftBottom.y), int(player.rightBottom.x),
		    int(player.rightBottom.y), int(player.leftTop.x), int(player.leftTop.y),
		    int(player.rightTop.x), int(player.rightTop.y), 0, 0, 16, 16, playerCurrentTexture,
		    playerColor);

		// パーティクル
		for (int i = 0; i < ELLIPSE_NUM_MAX; i++) {
			if (particle[i].isAppear == true) {
				Novice::DrawEllipse(
				    (int)particle[i].center.x, (int)particle[i].center.y, particle[i].radius,
				    particle[i].radius, 0.0f, 0x50FA23FF, kFillModeSolid);
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