#include <Novice.h>
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
	Vector2 leftTop;
	Vector2 rightTop;
	Vector2 leftBottom;
	Vector2 rightBottom;
	Vector2 speed1;
	Vector2 speed2;
	Vector2 speed3;
};
#include <Vector2.h>
const char kWindowTitle[] = "GC1D_03_イトウヒビキ_タイトル";

// スクリーン座標変換用の関数
Vector2 ToScreen(Vector2 world) {
	const Vector2 kworldToScreenTranslate = {0.0f, 720.0f};
	const Vector2 kworldToScreenScale = {1.0f, -1.0f};
	return {
	    (world.x * kworldToScreenScale.x) + kworldToScreenTranslate.x,
	    (world.y * kworldToScreenScale.y) + kworldToScreenTranslate.y};
}

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	const int WIN_WIDTH = 1280;
	const int WIN_HEIGHT = 720;

	Novice::Initialize(kWindowTitle, WIN_WIDTH, WIN_HEIGHT);

	Quad player = {
	    {640,32},32,32,
	    {player.pos.x - player.halfWidth, player.pos.y + player.halfHeight},
	    {player.pos.x + player.halfWidth, player.pos.y + player.halfHeight},
	    {player.pos.x - player.halfWidth, player.pos.y - player.halfHeight},
	    {player.pos.x + player.halfWidth, player.pos.y - player.halfHeight},
	    {3,0},
	    {6,0},
	    {9,0},
	};

	// スクリーン座標に変換した値を格納する変数
	Vector2 scsPlayerPos = ToScreen(player.pos);
	Vector2 scsLeftTop = ToScreen(player.leftTop);
	Vector2 scsRightTop = ToScreen(player.rightTop);
	Vector2 scsLeftBottom = ToScreen(player.leftBottom);
	Vector2 scsRightBottom = ToScreen(player.rightBottom);

	const int ELLIPSE_NUM_MAX = 500;

	struct Particle {
		Vector2 pos; // particleの出現位置
		int radius;
		Vector2 speed;
		bool isAppear;
		int moveTimer;
		Vector2 acceleration;
	};

	Particle particle[ELLIPSE_NUM_MAX];
	for (int i = 0; i < ELLIPSE_NUM_MAX; i++) {
		particle[i].pos.x = scsLeftBottom.x;
		particle[i].pos.y = scsLeftBottom.y;
		particle[i].radius = 4;
		particle[i].speed.x = 0;
		particle[i].speed.y = 0;
		particle[i].isAppear = false;
		particle[i].moveTimer = 30;
		particle[i].acceleration.x = 0.2f;
		particle[i].acceleration.y = 0.2f;
	}

	int particleCoolTimer = 10;
// Windowsアプリでのエントリーポイント(main関数)
	int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

		// float randomAccelX = float(rand() %5);
		// float randomAccelY= float(rand() %5);

		unsigned int currentTime = int(time(nullptr));
		srand(currentTime);
		// キー入力結果を受け取る箱
		char keys[256] = {0};
		char preKeys[256] = {0};
		int mapY = 24; // マップ縦幅
		// マウスカーソルの座標取得用変数
		int mousePosX = 0;
		int mousePosY = 0;

		const int lineWide = 213;

		// 画面中央を基準としてスピードを変える
		int accelerateStartLine = WIN_WIDTH / 2; // 略称ASL
		struct Ball {
			Vector2 position;
			Vector2 speed;
			Vector2 jump;
		};

		int mapY = 24; // マップ縦幅
		int mapX = 40; // マップ横幅
		// 当たり判定の変数
		int leftBottomX = 0;
		int leftBottomY = 0;
		int leftTopX = 0;
		int leftTopY = 0;
		int rightBottomX = 0;
		int rightBottomY = 0;

		int blockImges1 =
		    Novice::LoadTexture("./images/block4.png"); // ブロックの画像 　　マップ番号が1の時
		int blockImges2 =
		    Novice::LoadTexture("./images/Obstacle.png"); // ブロックの画像　 マップ番号が2の時
		int blockSize = 32;
		int blockImges1 =
		    Novice::LoadTexture("./block4.png"); // ブロックの画像 　　マップ番号が1の時
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
		    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
		     1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
		    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		     0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
		    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		     0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
		    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		     0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
		    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		     0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
		    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		     0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
		    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 2, 0, 0, 0, 0, 0, 0, 0,
		     0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
		    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		     0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
		    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		     0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
		    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		     0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
		    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		     0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
		    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		     0, 0, 0, 0, 2, 2, 2, 2, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
		    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		     0, 0, 0, 0, 0, 2, 2, 2, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
		    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 2,
		     2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 1},
		    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		     0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
		    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		     0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
		    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 1, 1,
		     1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
		    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 1, 1,
		     1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
		    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 1, 1,
		     1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
		    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
		     1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
		    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
		     1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
		    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
		     1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
		    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
		     1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
		    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
		     1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
		    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 2,
		     2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 1},
		    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		     0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
		    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		     0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
		    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 1, 1,
		     1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
		    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 1, 1,
		     1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
		    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 1, 1,
		     1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
		    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
		     1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
		    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
		     1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
		    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
		     1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
		    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
		     1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
		    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
		     1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
		};

		unsigned int playerColor = WHITE;

		// インデックスの値を変える間隔を調整するための変数
		int playerAnimationTimer = 0;

		// インデックスの値によって描画される画像が変わる
		int playerAnimationIndex = 0;

		int rightTexture[4] = {
		    Novice::LoadTexture("./images/player/PLAYER1.png"),
		    Novice::LoadTexture("./images/player/PLAYER2.png"),
		    Novice::LoadTexture("./images/player/PLAYER3.png"),
		    Novice::LoadTexture("./images/player/PLAYER4.png"),
		};

		// キー入力結果を受け取る箱
		char keys[256] = {0};
		char preKeys[256] = {0};
	};
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

		if (mousePosX >= WIN_WIDTH / 2) {
			isMouseInTheRight = true;
			isMouseInTheLeft = false;
		} else if (mousePosX < WIN_WIDTH / 2) {
			isMouseInTheRight = false;
			isMouseInTheLeft = true;
		}

		// マウスカーソルの位置によって色が変わる
		if (isMouseInTheLeft) {
			playerColor = RED;
		}
		if (isMouseInTheRight) {
			playerColor = WHITE;
		}

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

		if (mousePosX >= WIN_WIDTH / 2 && mousePosX < furthermoreASL1) {
			player.pos.x += player.speed1.x;
		} else if (mousePosX >= furthermoreASL1 && mousePosX < furthermoreASL2) {
			player.pos.x += player.speed2.x;
		} else if (mousePosX >= furthermoreASL2) {
			player.pos.x += player.speed3.x;
		} else if (mousePosX < WIN_WIDTH / 2 && mousePosX > furthermoreASL3) {
			player.pos.x -= player.speed1.x;
		} else if (mousePosX <= furthermoreASL3 && mousePosX > furthermoreASL4) {
			player.pos.x -= player.speed2.x;

		} else if (mousePosX <= furthermoreASL4) {
			player.pos.x -= player.speed3.x;
		}

		// プレイヤーが画面外にいかないようにする処理
		if (player.pos.x < player.halfWidth) {

			player.pos.x = player.halfWidth;
		// 当たり判定の変数
		leftBottomX = ((int)ball.position.x - (int)ball.radius) / blockSize;
		leftBottomY = ((int)ball.position.y + (int)ball.radius - 1) / blockSize;
		}
		rightBottomX = ((int)ball.position.x + (int)ball.radius - 1) / blockSize;
		rightBottomY = ((int)ball.position.y + (int)ball.radius - 1) / blockSize;
		} else if (player.pos.x > WIN_WIDTH - player.halfWidth) {
		else if (scene == trialscene) {
			player.pos.x = WIN_WIDTH - player.halfWidth;
		}
		rightTopX = ((int)ball.position.x + (int)ball.radius - 1) / blockSize;
		rightTopY = ((int)ball.position.y - (int)ball.radius) / blockSize;
		if (map[rightTopY][rightTopX] == 0 && map[rightBottomY][rightBottomX] == 0) {
			ball.speed.x = 2;
			ball.position.x = ball.position.x + ball.speed.x; // 本体を進める
		}
		// メインシーンの時
		// 中心座標と各頂点をスクリーン座標に変換
		ToScreen(player.pos);
		scsLeftTop = ToScreen(player.leftTop);
		scsRightTop = ToScreen(player.rightTop);
		scsLeftBottom = ToScreen(player.leftBottom);
		scsRightBottom = ToScreen(player.rightBottom);

		//===========================================================================
		// パーティクルの更新処理
		//===========================================================================

		// i番目の玉が出ていなかったらプレイヤーの足元に出現させる
		if (particleCoolTimer > 0) {
			particleCoolTimer--;
		} else {
			particleCoolTimer = 2; // 値によって出現間隔が変わる
		}
		if (isJump == 0) {
			if (keys[DIK_SPACE] && preKeys[DIK_SPACE] == 0) {
				ball.jump.y = -10.8f; // ジャンプ
				isJump = 1;
			}
		} else if (isFly == 0 && isJump == 1) {
			if (preKeys[DIK_SPACE] && keys[DIK_SPACE]) {
				ball.jump.y = 0.5f;
				isFly = 1;
			}
		}
			rightBottomX = ((int)ball.position.x + (int)ball.radius - 1) / blockSize;
		if (particleCoolTimer <= 0) {
			for (int i = 0; i < ELLIPSE_NUM_MAX; i++) {
				if (particle[i].isAppear == false) {
		ball.speed.y += ball.jump.y; // 重力を加算

		ball.position.y += ball.speed.y; // プレイヤーに重力を加算
		rightBottomX = ((int)ball.position.x + (int)ball.radius - 1) / blockSize;
		rightBottomY =
		    ((int)ball.position.y + (int)ball.radius - 1 + (int)ball.speed.y) / blockSize;
				ball.position.x = ball.position.x + ball.speed.x; // 本体を進める
		leftBottomX = ((int)ball.position.x - (int)ball.radius) / blockSize;
		leftBottomY = ((int)ball.position.y + (int)ball.radius - 1 + (int)ball.speed.y) / blockSize;
					particle[i].isAppear = true;
					// particle[i].acceleration.x = randomAccelX;
					// particle[i].acceleration.y = 0.2f;
					particle[i].pos.x = scsLeftBottom.x + 30;
					particle[i].pos.y = scsLeftBottom.y;
					break;
				}
			}
		}
				if (keys[DIK_SPACE] && preKeys[DIK_SPACE] == 0) {
		for (int i = 0; i < ELLIPSE_NUM_MAX; i++) {
		rightTopX = ((int)ball.position.x + (int)ball.radius - 1) / blockSize;
		rightTopY = ((int)ball.position.y - (int)ball.radius + (int)ball.speed.y) / blockSize;
		leftTopX = ((int)ball.position.x - (int)ball.radius) / blockSize;
		leftTopY = ((int)ball.position.y - (int)ball.radius + (int)ball.speed.y) / blockSize;
				if (preKeys[DIK_SPACE] && keys[DIK_SPACE]) {
		if (map[rightTopY][rightTopX] != 0 && map[leftTopY][leftTopX] != 0) {
			if (particle[i].isAppear == true) {
					isFly = 1;
				if (particle[i].moveTimer > 0) {
					particle[i].moveTimer--;
				} else if (particle[i].moveTimer == 0) {
					particle[i].moveTimer = 30;
				}
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
			    ((int)ball.position.y + (int)ball.radius - 1 + (int)ball.speed.y) / blockSize;
				particle[i].speed.x = float(rand() % 5);
				particle[i].speed.y = float(rand() % 5 - 0);
		if (map[rightTopY][rightTopX] == 2 && map[leftTopY][leftTopX] == 2) {
			leftBottomY =
				if (isMouseInTheRight == true) {
					particle[i].pos.x -= particle[i].speed.x;
					particle[i].pos.y -= particle[i].speed.y;
				} else {
					particle[i].pos.x += particle[i].speed.x;
					particle[i].pos.y -= particle[i].speed.y;
				}
			}

			// パーティクルの出現範囲
			if (particle[i].moveTimer == 0) {
				particle[i].isAppear = false;
				particle[i].speed.x = 0;
				particle[i].speed.y = 0;
			}
			ball.speed.y = 0.0f;
			isJump = 0;
			isFly = 0;
			Overflag = false; // ゲームオーバーフラグ
		}
		if (map[rightBottomY][rightBottomX] == 2 && map[leftBottomY][leftBottomX] == 2) {
			ball.position.y = (rightBottomY * blockSize) - ball.radius;
			ball.speed.y = 0.0f;
			isJump = 0;
			isFly = 0;
			Overflag = false; // ゲームオーバーフラグ
		}
		if (Overflag == false) { // ゲームオーバーフラグ
			ball.position.x = 200;
			ball.position.y = 200;
				ball.position.y = (rightBottomY * blockSize) - ball.radius;
				ball.speed.y = 0.0f;
				isJump = 0;
				isFly = 0;
			}

		}
		if (scene != nextScene) {

		Novice::ScreenPrintf(0, 0, "LEFT FLAG=%d", isMouseInTheLeft);

		Novice::ScreenPrintf(0, 50, "RIGHT FLAG=%d", isMouseInTheRight);

		Novice::DrawLine(WIN_WIDTH / 2, 0, WIN_WIDTH / 2, WIN_HEIGHT, RED);
		Novice::DrawLine(furthermoreASL1, 0, furthermoreASL1, WIN_HEIGHT, WHITE);
		Novice::DrawLine(furthermoreASL2, 0, furthermoreASL2, WIN_HEIGHT, WHITE);
		Novice::DrawLine(furthermoreASL3, 0, furthermoreASL3, WIN_HEIGHT, WHITE);
		Novice::DrawLine(furthermoreASL4, 0, furthermoreASL4, WIN_HEIGHT, WHITE);

		Novice::DrawQuad(int(scsLeftTop.x), int(scsLeftTop.y), int(scsRightTop.x), int(scsRightTop.y),int(scsLeftBottom.x), int(scsLeftBottom.y), int(scsRightBottom.x),
		    int(scsRightBottom.y), 0, 0, 16, 16, rightTexture[playerAnimationIndex], playerColor);

		for (int i = 0; i < ELLIPSE_NUM_MAX; i++) {
			if (particle[i].isAppear == true) {
				Novice::DrawEllipse(
				    (int)particle[i].pos.x, (int)particle[i].pos.y, particle[i].radius,
				    particle[i].radius, 0.0f, 0xA0522DFF, kFillModeSolid);
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
		if (scene != nextScene) {
			scene = nextScene;
		}
		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///
		 
		if (scene == mainscene) {
			Novice::DrawEllipse((int)ball.position.x, (int)ball.position.y, (int)ball.radius, (int)ball.radius,0.0f, RED, kFillModeSolid);

			// kFillModeSolid);//プレイヤー
			for (int i = 0; i < mapY; i++) {
				for (int p = 0; p < mapX; p++) {
					if (map[i][p] == 1) { // マップ番号が1の時
						Novice::DrawSprite((p * blockSize), i * blockSize, blockImges1, 1.0f, 1.0f, 0.0f, WHITE);
					}
					if (map[i][p] == 2) { // マップ番号が2の時
						Novice::DrawSprite((p * blockSize), i * blockSize, blockImges2, 1.0f, 1.0f, 0.0f, WHITE);
					}
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
