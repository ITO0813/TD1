#include <Novice.h>
#include<stdlib.h>
#include<time.h>

const char kWindowTitle[] = "GC1D_03_イトウヒビキ_タイトル";

struct Vector2 {
	int x;
	int y;

};

struct Quad {
	Vector2 pos;//中心座標
	Vector2 halfsize;
	Vector2 leftTop;
	Vector2 rightTop;
	Vector2 leftBottom;
	Vector2 rightBottom;

};

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	const int BLOCK_SIZE = 32;
	const int MAP_WIDTH = 80;
	const int MAP_HEIGHT = 24;
	const int WORLD_WIDTH = MAP_WIDTH * BLOCK_SIZE;
	const int WIN_WIDTH = 1280;
	const int WIN_HEIGHT = MAP_HEIGHT * BLOCK_SIZE;

	Novice::Initialize(kWindowTitle, WIN_WIDTH, WIN_HEIGHT);

	Quad player = {
		{48,48},
		16,16,
		//左上 lightTop
		(player.pos.x - player.halfsize.x) / BLOCK_SIZE,
		(player.pos.y - player.halfsize.y) / BLOCK_SIZE,

		//右上 rightTop
		(player.pos.x + player.halfsize.x - 1) / BLOCK_SIZE,
		(player.pos.y - player.halfsize.y) / BLOCK_SIZE,

		//左下 leftBottom
		(player.pos.x - player.halfsize.x) / BLOCK_SIZE,
		(player.pos.y + player.halfsize.y - 1) / BLOCK_SIZE,

		//右下 rightBottom
		(player.pos.x + player.halfsize.x - 1) / BLOCK_SIZE,
		(player.pos.y + player.halfsize.y - 1) / BLOCK_SIZE,
	};

	int scrollX = 0;

	//スクロールの開始位置
	int scrollStartLineX = 720 - 64;

	int playerScsX = int(player.pos.x - scrollStartLineX);

	float gravity = 0.8f;
	//int jumpCount = 0;

	//一度に存在できる粒子の最大数
	const int ELLIPSE_NUM_MAX = 500;

	struct Vector2Float {
		float x;
		float y;
	};

	struct Particle {
		Vector2Float center;
		int radius;
		Vector2Float speed;
		bool isAppear;
		int moveTimer;
		Vector2Float acceleration;
	};

	Particle particle[ELLIPSE_NUM_MAX];
	for (int i = 0; i < ELLIPSE_NUM_MAX; i++) {
		particle[i].center.x = (float)player.leftBottom.x;
		particle[i].center.y = (float)player.leftBottom.y;
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

	//マウスカーソルの座標取得用変数
	int mousePosX = 0;
	int mousePosY = 0;

	const int lineWide = 213;

	//画面中央を基準としてスピードを変える
	int accelerateStartLine = WIN_WIDTH / 2;//略称ASL

	//画面左エリア
	int furthermoreASL1 = accelerateStartLine + lineWide;
	int furthermoreASL2 = furthermoreASL1 + lineWide;

	//画面右エリア
	int furthermoreASL3 = accelerateStartLine - lineWide;
	int furthermoreASL4 = furthermoreASL3 - lineWide;

	//マウスカーソルが中央線に対して左右どちらにいるかを判別する為のフラグ
	bool isMouseInTheLeft = false;
	bool isMouseInTheRight = false;

	unsigned int playerColor = WHITE;

	enum Direction {
		LEFT,//0
		RIGHT,//1
	};

	//プレイヤーの右向き画像
	int rightTexture[4] = {
		Novice::LoadTexture("./Resources/player/PLAYER1.png"),
		Novice::LoadTexture("./Resources/player/PLAYER2.png"),
		Novice::LoadTexture("./Resources/player/PLAYER3.png"),
		Novice::LoadTexture("./Resources/player/PLAYER4.png"),
	};
	int playerRights[4] = { rightTexture[0],rightTexture[1],rightTexture[2],rightTexture[3] };

	//プレイヤーの左向き画像(後で左向きの画像に差し替え)
	int leftTexture[4] = {
		Novice::LoadTexture("./Resources/player/PLAYER1.png"),
		Novice::LoadTexture("./Resources/player/PLAYER2.png"),
		Novice::LoadTexture("./Resources/player/PLAYER3.png"),
		Novice::LoadTexture("./Resources/player/PLAYER4.png"),
	};
	int playerLefts[4] = { leftTexture[0],leftTexture[1],leftTexture[2],leftTexture[3] };

	int playerDirection = Direction::RIGHT;//プレイヤーの向き

	int playerCurrentTexture = playerRights[0];//現在のプレイヤーの画像

	//プレイヤーの移動速度
	Vector2 playerSpeed = { 0,0 };
	Vector2 kTmpSpeed = { 4,4 };
	Vector2 kPSpeed = { 4,4 };



	//ジャンプ力
	float jumpPower = 0;

	//ジャンプしていないときの落下速度用変数
	float downSpeed = 0;

	//プレイヤーがジャンプしているかどうか
	bool isJumpingPlayer = false;

	Vector2 hitCheckLeftT = { 0 };//左上座標
	Vector2 hitCheckRightB = { 0 };//右下座標

	//インデックスの値を変える間隔を調整するための変数
	int playerAnimationTimer = 0;

	//インデックスの値によって描画される画像が変わる
	int playerAnimationIndex = 0;

	//残像処理用変数
	const int afterImageLength = 10;
	int isDraw[afterImageLength] = {};//残像が出ているかの管理フラグ
	int afterImageX[afterImageLength] = {};//各残像のX座標
	int afterImageY[afterImageLength] = {};//各残像のY座標
	int drawCoolTimer = 0;//残像の描画間隔管理用変数
	int afterImageTimer[afterImageLength] = { 0 };//残像が存在できる時間

	enum SCENE {
		TITLE_SCENE,//0
		TUTORIAL_SCENE,
		MAIN_SCENE,
		CLEAR_SCENE,
		GAMEOVER_SCENE
	};

	int sceneNumber = 0;

	int map[MAP_HEIGHT][MAP_WIDTH] = {
		{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
		{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,1 },
		{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,1 },
		{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,1 },
		{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,1 },
		{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,1 },
		{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,1 },
		{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,1 },
		{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,1 },
		{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,1 },
		{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,1 },
		{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 2, 2, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,1 },
		{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 2, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,1 },
		{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,1 },
		{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,1 },
		{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,1 },
		{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,1 },
		{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,1 },
		{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,1 },
		{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,1 },
		{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 1, 0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,1 },
		{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 0, 0, 1, 0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,1 },
		{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 0, 0, 1, 0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,0 ,1 },
		{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	};

	int blockImages1 = Novice::LoadTexture("./Resources/block.png");
	int blockImages2 = Novice::LoadTexture("./Resources/block.png");

	int obstacleTexture = Novice::LoadTexture("white1x1.png");

	const int OB_NUM_MAX = 256;

	struct Obstacle {
		Vector2 pos;//中心座標
		Vector2 halfsize;
		Vector2 leftTop;
		Vector2 rightTop;
		Vector2 leftBottom;
		Vector2 rightBottom;
		bool isAppear;
	};

	Obstacle obstacle[OB_NUM_MAX];
	for (int i = 0; i < OB_NUM_MAX; i++) {
		obstacle[i].pos.x = 0;
		obstacle[i].pos.y = 0;

		obstacle[i].halfsize.x = 16;
		obstacle[i].halfsize.y = 16;

		obstacle[i].leftTop.x = 0;
		obstacle[i].leftTop.y = 0;

		obstacle[i].rightTop.x = 0;
		obstacle[i].rightTop.y = 0;

		obstacle[i].leftBottom.x = 0;
		obstacle[i].leftBottom.y = 0;

		obstacle[i].rightBottom.x = 0;
		obstacle[i].rightBottom.y = 0;

		obstacle[i].isAppear = false;
	}

	int obstacleCoolTimer = 0;

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

		switch (sceneNumber) {
		case TITLE_SCENE:


			//特定のキーで次のシーンに切り替わる
			if (keys[DIK_SPACE] && preKeys[DIK_SPACE]) {
				sceneNumber = TUTORIAL_SCENE;
			}

			//BGM

			//キー効果音

			break;

		case TUTORIAL_SCENE:
			if (keys[DIK_SPACE] && preKeys[DIK_SPACE]) {
				sceneNumber = MAIN_SCENE;
			}
			//BGM

			//キー効果音

			break;

		case MAIN_SCENE:

			//===========================================================================
			//自機の更新処理
			//===========================================================================



			//15フレーム毎に画像が切り替わる
			playerAnimationTimer++;
			if (playerAnimationTimer % 15 == 0) {
				playerAnimationIndex++;
				if (playerAnimationIndex > 3) {
					playerAnimationIndex = 0;
				}
			}

			scrollX = int(player.pos.x - scrollStartLineX);

			if (scrollX < 0) {
				scrollX = 0;
			}

			if (scrollX > WORLD_WIDTH - WIN_WIDTH) {
				scrollX = WORLD_WIDTH - WIN_WIDTH;
			}

			//プレイヤーのスクリーン座標
			playerScsX = int(player.pos.x - scrollX);

			player.pos.x += playerSpeed.x;
			player.pos.y += playerSpeed.y;

			playerSpeed.x = 0;
			playerSpeed.y = 0;

			//マウスの座標が画面中央よりどちらにいるかによって方向管理フラグが切り替わる
			if (mousePosX >= WIN_WIDTH / 2) {
				isMouseInTheRight = true;
				isMouseInTheLeft = false;
				playerDirection = Direction::RIGHT;
			}
			else if (mousePosX < WIN_WIDTH / 2) {
				isMouseInTheRight = false;
				isMouseInTheLeft = true;
				playerDirection = Direction::LEFT;
			}

			//プレイヤーの描画向き切り替え処理
			if (playerDirection == Direction::RIGHT) {//右向き
				playerCurrentTexture = playerRights[playerAnimationIndex];
			}
			else if (playerDirection == Direction::LEFT) {//左向き
				playerCurrentTexture = playerLefts[playerAnimationIndex];
			}

			//マウスカーソルの位置によって色が変わる
			if (isMouseInTheLeft) {
				playerColor = RED;
			}
			if (isMouseInTheRight) {
				playerColor = WHITE;
			}

			///
			///ジャンプの処理ここから↓
			/// 
			if (keys[DIK_SPACE] && preKeys[DIK_SPACE] == 0) {
				if (isJumpingPlayer == false) {
					jumpPower = -16.0f;
					isJumpingPlayer = true;
				}
			}

			//プレイヤーがいる位置のマップ番号取得
			player.leftTop.x = (player.pos.x - player.halfsize.x) / BLOCK_SIZE;
			player.rightTop.x = (player.pos.x + player.halfsize.x - 1) / BLOCK_SIZE;

			player.leftTop.y = (player.pos.y - player.halfsize.y + playerSpeed.y) / BLOCK_SIZE;
			player.rightTop.y = (player.pos.y - player.halfsize.y + playerSpeed.y) / BLOCK_SIZE;

			player.leftBottom.x = (player.pos.x - player.halfsize.x) / BLOCK_SIZE;
			player.rightBottom.x = (player.pos.x + player.halfsize.x - 1) / BLOCK_SIZE;

			player.leftBottom.y = (player.pos.y + player.halfsize.y + playerSpeed.y - 1) / BLOCK_SIZE;
			player.rightBottom.y = (player.pos.y + player.halfsize.y + playerSpeed.y - 1) / BLOCK_SIZE;


			//ジャンプしているとき
			if (isJumpingPlayer == true) {

				//どちら片方でもブロックに当たっていれば跳ね返る
				if (map[player.rightTop.y][player.rightTop.x] != 0) {
					jumpPower = 0;
					//めり込まないように位置を戻す
					player.pos.y = player.rightTop.y * BLOCK_SIZE + (player.halfsize.y * 3);
				}
				else if (map[player.leftTop.y][player.leftTop.x] != 0) {
					jumpPower = 0;
					player.pos.y = player.rightTop.y * BLOCK_SIZE + (player.halfsize.y * 3);
				}

				if (map[player.rightBottom.y][player.rightBottom.x] == 0 && map[player.leftBottom.y][player.leftBottom.x] == 0)
				{
					//下辺がブロックに当たっていなければ落下する
					jumpPower += gravity;
					playerSpeed.y = (int)jumpPower;
				}
				//どちらか片方でもブロックに当たっていれば止まる
				else if (map[player.leftBottom.y][player.leftBottom.x] != 0) {
					player.pos.y = player.leftBottom.y * BLOCK_SIZE - player.halfsize.y;
					jumpPower = 0;
					isJumpingPlayer = false;
				}
				else if (map[player.rightBottom.y][player.rightBottom.x] != 0) {
					player.pos.y = player.leftBottom.y * BLOCK_SIZE - player.halfsize.y;
					jumpPower = 0;
					isJumpingPlayer = false;
				}
			}
			//自然落下処理
			else if (isJumpingPlayer == false) {//プレイヤーがジャンプしていないとき

				if (map[player.rightBottom.y][player.rightBottom.x] == 0 && map[player.leftBottom.y][player.leftBottom.x] == 0)
				{
					downSpeed += gravity;
					playerSpeed.y = (int)downSpeed;
				}
				//どちらか片方でもブロックに乗っていれば落下を止める
				if (map[player.leftBottom.y][player.leftBottom.x] != 0) {
					player.pos.y = player.leftBottom.y * BLOCK_SIZE - player.halfsize.y;
					downSpeed = 0;
				}
				else if (map[player.rightBottom.y][player.rightBottom.x] != 0) {
					player.pos.y = player.leftBottom.y * BLOCK_SIZE - player.halfsize.y;
					downSpeed = 0;
				}
			}
			///
			///ジャンプの処理ここまで↑
			/// 

			//マウスの位置を取得する
			Novice::GetMousePosition(&mousePosX, &mousePosY);

			//マウスの座標と中央線の距離によってスピードが変わる
			if (mousePosX >= WIN_WIDTH / 2 && mousePosX < furthermoreASL1) {
				kTmpSpeed.x = 2;
				kPSpeed.x = 2;
				player.rightTop.x = (player.pos.x + player.halfsize.x - 1 + kTmpSpeed.x) / BLOCK_SIZE;

				player.rightBottom.x = (player.pos.x + player.halfsize.x - 1 + kTmpSpeed.x) / BLOCK_SIZE;

				if (map[player.rightTop.y][player.rightTop.x] == 0 && map[player.rightBottom.y][player.rightBottom.x] == 0)
				{
					playerSpeed.x = kPSpeed.x;
				}
			}
			else if (mousePosX >= furthermoreASL1 && mousePosX < furthermoreASL2) {
				kTmpSpeed.x = 4;
				kPSpeed.x = 4;
				player.rightTop.x = (player.pos.x + player.halfsize.x - 1 + kTmpSpeed.x) / BLOCK_SIZE;

				player.rightBottom.x = (player.pos.x + player.halfsize.x - 1 + kTmpSpeed.x) / BLOCK_SIZE;

				if (map[player.rightTop.y][player.rightTop.x] == 0 && map[player.rightBottom.y][player.rightBottom.x] == 0)
				{
					playerSpeed.x = kPSpeed.x;
				}
			}
			else if (mousePosX >= furthermoreASL2) {
				kTmpSpeed.x = 8;
				kPSpeed.x = 8;
				player.rightTop.x = (player.pos.x + player.halfsize.x - 1 + kTmpSpeed.x) / BLOCK_SIZE;

				player.rightBottom.x = (player.pos.x + player.halfsize.x - 1 + kTmpSpeed.x) / BLOCK_SIZE;

				if (map[player.rightTop.y][player.rightTop.x] == 0 && map[player.rightBottom.y][player.rightBottom.x] == 0)
				{
					playerSpeed.x = kPSpeed.x;
				}
			}
			else if (mousePosX < WIN_WIDTH / 2 && mousePosX > furthermoreASL3) {
				kTmpSpeed.x = 2;
				kPSpeed.x = 2;
				player.leftTop.x = (player.pos.x - player.halfsize.x - kTmpSpeed.x) / BLOCK_SIZE;

				player.leftBottom.x = (player.pos.x - player.halfsize.x - kTmpSpeed.x) / BLOCK_SIZE;

				if (map[player.leftTop.y][player.leftTop.x] == 0 && map[player.leftBottom.y][player.leftBottom.x] == 0)
				{
					playerSpeed.x = -kPSpeed.x;
				}

			}
			else if (mousePosX <= furthermoreASL3 && mousePosX > furthermoreASL4) {
				kTmpSpeed.x = 4;
				kPSpeed.x = 4;
				player.leftTop.x = (player.pos.x - player.halfsize.x - kTmpSpeed.x) / BLOCK_SIZE;

				player.leftBottom.x = (player.pos.x - player.halfsize.x - kTmpSpeed.x) / BLOCK_SIZE;

				if (map[player.leftTop.y][player.leftTop.x] == 0 && map[player.leftBottom.y][player.leftBottom.x] == 0)
				{
					playerSpeed.x = -kPSpeed.x;
				}
			}
			else if (mousePosX <= furthermoreASL4) {
				kTmpSpeed.x = 8;
				kPSpeed.x = 8;
				player.leftTop.x = (player.pos.x - player.halfsize.x - kTmpSpeed.x) / BLOCK_SIZE;

				player.leftBottom.x = (player.pos.x - player.halfsize.x - kTmpSpeed.x) / BLOCK_SIZE;

				if (map[player.leftTop.y][player.leftTop.x] == 0 && map[player.leftBottom.y][player.leftBottom.x] == 0)
				{
					playerSpeed.x = -kPSpeed.x;
				}
			}

			//===========================================================================
			//パーティクルの更新処理
			//===========================================================================

			if (particleCoolTimer > 0) {
				particleCoolTimer--;
			}
			else {
				particleCoolTimer = 2;//1秒あたり30個の粒子が出る
			}
			//particleCoolTimerが0になった時i番目の粒子が出ていなかったらプレイヤーの足元に出現させる
			if (particleCoolTimer <= 0) {
				for (int i = 0; i < ELLIPSE_NUM_MAX; i++) {

					if (particle[i].isAppear == false) {

						particle[i].isAppear = true;

						//パーティクルの発生位置を足元に合わせる
						particle[i].center.x = (float)playerScsX;
						particle[i].center.y = (float)player.pos.y + player.halfsize.y;
						break;
					}
				}
			}

			for (int i = 0; i < ELLIPSE_NUM_MAX; i++) {

				if (particle[i].isAppear == true) {

					//粒子1個あたりの存在出来る時間(0.5秒)
					if (particle[i].moveTimer > 0) {
						particle[i].moveTimer--;
					}
					else if (particle[i].moveTimer == 0) {
						particle[i].moveTimer = 30;
					}

					particle[i].speed.x = float(rand() % 4);
					particle[i].speed.y = float(rand() % 5 - 0);

					//マウスの座標が中央より右に位置するときプレイヤの進行方向の反対側に向かってパーティクルが発生する
					if (isMouseInTheRight == true) {
						particle[i].center.x -= particle[i].speed.x;
						particle[i].center.y -= particle[i].speed.y;
					}
					else {//左も同様
						particle[i].center.x += particle[i].speed.x;
						particle[i].center.y -= particle[i].speed.y;
					}

				}
				//ムーブタイマーが0になったら粒子を消す
				if (particle[i].moveTimer == 0) {
					particle[i].isAppear = false;
					particle[i].speed.x = 0;
					particle[i].speed.y = 0;
				}
			}

			//===========================================================================
			//残像の更新処理
			//===========================================================================
			drawCoolTimer++;

			if (drawCoolTimer > 80) {
				drawCoolTimer = 0;
			}

			for (int i = 0; i < afterImageLength; i++) {
				if (drawCoolTimer == 8 * i) {

					afterImageX[i] = (int)playerScsX - player.halfsize.x;
					afterImageY[i] = (int)player.pos.y - player.halfsize.y;
					isDraw[i] = 1;
				}
				if (isDraw[i] == 1) {
					afterImageTimer[i]++;
				}

				if (afterImageTimer[i] >= 20) {
					isDraw[i] = 0;
					afterImageTimer[i] = 0;
				}
			}

			//===========================================================================
			//障害物の更新処理
			//===========================================================================

			//当たり判定用にプレイヤーの左上と右下座標を取得する
			hitCheckLeftT.x = playerScsX - player.halfsize.x;
			hitCheckLeftT.y = player.pos.y - player.halfsize.y;

			hitCheckRightB.x = playerScsX + player.halfsize.x;
			hitCheckRightB.y = player.pos.y + player.halfsize.y;

			//プレイヤーと障害物の当たり判定
			for (int i = 0; i < OB_NUM_MAX; i++) {
				if (hitCheckLeftT.x < obstacle[i].rightBottom.x && obstacle[i].leftTop.x < hitCheckRightB.x) {
					if (obstacle[i].leftTop.y < hitCheckRightB.y && hitCheckLeftT.y < obstacle[i].rightBottom.y) {
						if (obstacle[i].isAppear == true) {
							//playerHitPoint--;
							playerColor = GREEN;
						}
						obstacle[i].isAppear = false;//当たったら障害物は消える
					}

				}
			}


			//発射間隔の調整用クールタイムの計算
			if (obstacleCoolTimer > 0) {
				obstacleCoolTimer--;
			}
			else {
				obstacleCoolTimer = 15;//個々の数値を変えることで出現間隔を調整
			}

			// i番目の障害物が出ていなかったら発射する
			if (obstacleCoolTimer <= 0) {
				for (int i = 0; i < OB_NUM_MAX; i++) {
					if (obstacle[i].isAppear == false) {

						obstacle[i].isAppear = true;

						//出現位置を画面右端からランダムな高さにする
						obstacle[i].pos.x = (WIN_WIDTH - obstacle[i].halfsize.x);
						obstacle[i].pos.y = rand() % WIN_HEIGHT - obstacle[i].halfsize.y;
						break;
					}
				}
			}

			//障害物の弾道計算
			for (int i = 0; i < OB_NUM_MAX; i++) {
				if (obstacle[i].isAppear == true) {

					obstacle[i].leftTop.x = obstacle[i].pos.x - obstacle[i].halfsize.x;//左上X座標
					obstacle[i].leftTop.y = obstacle[i].pos.y - obstacle[i].halfsize.y;//左上Y座標

					obstacle[i].rightTop.x = obstacle[i].pos.x + obstacle[i].halfsize.x;//左上X座標
					obstacle[i].rightTop.y = obstacle[i].pos.y - obstacle[i].halfsize.y;//左上Y座標

					obstacle[i].leftBottom.x = obstacle[i].pos.x - obstacle[i].halfsize.x;//左上X座標
					obstacle[i].leftBottom.y = obstacle[i].pos.y + obstacle[i].halfsize.y;//左上Y座標

					obstacle[i].rightBottom.x = obstacle[i].pos.x + obstacle[i].halfsize.x;//左上X座標
					obstacle[i].rightBottom.y = obstacle[i].pos.y + obstacle[i].halfsize.y;//左上Y座標


					//障害物の速度
					obstacle[i].pos.x -= 5;//左方向直線に発射

					// 画面外に出たら発射フラグをFalseに変更する
					if (obstacle[i].pos.x < player.halfsize.x) {
						obstacle[i].isAppear = false;
					}
				}
			}

			break;

		case CLEAR_SCENE:
			//キー効果音
			//クリアBGM

			break;

		case GAMEOVER_SCENE:

			//キー効果音
			//ゲームオーバーBGM

			break;

		}


		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///

		switch (sceneNumber) {

		case TITLE_SCENE:

			//タイトル画面


			break;

		case TUTORIAL_SCENE:

			//チュートリアル画面

			break;

		case MAIN_SCENE://ゲームプレイ画面
			//マップ
			for (int y = 0; y < MAP_HEIGHT; y++) {
				for (int x = 0; x < MAP_WIDTH; x++) {
					if (map[y][x] == 1) { // マップ番号が1の時
						Novice::DrawSprite((x * BLOCK_SIZE - scrollX), y * BLOCK_SIZE, blockImages1, 1.0f, 1.0f, 0.0f, RED);
					}
					else if (map[y][x] == 2) { // マップ番号が2の時
						Novice::DrawSprite((x * BLOCK_SIZE - scrollX), y * BLOCK_SIZE, blockImages2, 1.0f, 1.0f, 0.0f, BLUE);
					}
				}
			}

			//デバッグ用
			Novice::ScreenPrintf(0, 0, "leftTop=[%d][%d]", player.leftTop.y, player.leftTop.x);
			Novice::ScreenPrintf(200, 0, "rightTop=[%d][%d]", player.rightTop.y, player.rightTop.x);
			Novice::ScreenPrintf(0, 50, "leftBottom=[%d][%d]", player.leftBottom.y, player.leftBottom.x);
			Novice::ScreenPrintf(200, 50, "rightBottom=[%d][%d]", player.rightBottom.y, player.rightBottom.x);
			Novice::ScreenPrintf(0, 100, "jumpPower=%5.2f", jumpPower);
			Novice::ScreenPrintf(0, 150, "playerSpeed.x=%d", playerSpeed.x);
			Novice::ScreenPrintf(0, 200, "isJumping=%d", isJumpingPlayer);
			Novice::ScreenPrintf(0, 250, "downSpeed=%5.2f", downSpeed);

			Novice::DrawLine(WIN_WIDTH / 2, 0, WIN_WIDTH / 2, WIN_HEIGHT, RED);
			Novice::DrawLine(furthermoreASL1, 0, furthermoreASL1, WIN_HEIGHT, WHITE);
			Novice::DrawLine(furthermoreASL2, 0, furthermoreASL2, WIN_HEIGHT, WHITE);
			Novice::DrawLine(furthermoreASL3, 0, furthermoreASL3, WIN_HEIGHT, WHITE);
			Novice::DrawLine(furthermoreASL4, 0, furthermoreASL4, WIN_HEIGHT, WHITE);


			//残像
			for (int i = 0; i < afterImageLength; i++) {
				if (isDraw[i] == 1) {
					Novice::DrawSpriteRect(
						int(afterImageX[i]),
						int(afterImageY[i]),
						0, 0, 16, 16, playerCurrentTexture, 2, 2, 0.0f, BLUE
					);
				}
			}

			//プレイヤー
			Novice::DrawQuad
			(
				int(playerScsX - player.halfsize.x),
				int(player.pos.y - player.halfsize.y),

				int(playerScsX + player.halfsize.x),
				int(player.pos.y - player.halfsize.y),

				int(playerScsX - player.halfsize.x),
				int(player.pos.y + player.halfsize.y),

				int(playerScsX + player.halfsize.x),
				int(player.pos.y + player.halfsize.y),
				0, 0, 16, 16, playerCurrentTexture, playerColor
			);

			//パーティクル
			for (int i = 0; i < ELLIPSE_NUM_MAX; i++) {
				if (particle[i].isAppear == true) {
					Novice::DrawEllipse
					(
						(int)particle[i].center.x,
						(int)particle[i].center.y,
						particle[i].radius,
						particle[i].radius,
						0.0f, 0x50FA23FF, kFillModeSolid
					);
				}
			}

			//障害物
			for (int i = 0; i < OB_NUM_MAX; i++) {
				if (obstacle[i].isAppear == true) {
					Novice::DrawQuad
					(
						obstacle[i].leftTop.x,
						obstacle[i].leftTop.y,
						obstacle[i].rightTop.x,
						obstacle[i].rightTop.y,
						obstacle[i].leftBottom.x,
						obstacle[i].leftBottom.y,
						obstacle[i].rightBottom.x,
						obstacle[i].rightBottom.y,
						0, 0, 1, 1, obstacleTexture, WHITE
					);
				}
			}

			break;

		case CLEAR_SCENE:

			break;

		case GAMEOVER_SCENE:

			break;
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

	//ライブラリの終了	
	Novice::Finalize();
	return 0;
}
