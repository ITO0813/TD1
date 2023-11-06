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
	Vector2 acceleration;//加速度
};

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化

	const int WIN_WIDTH = 1280;
	const int WIN_HEIGHT = 720;

	Novice::Initialize(kWindowTitle, WIN_WIDTH, WIN_HEIGHT);

	Quad player = {
		{640,360},
		16,16,
		{player.pos.x - player.halfWidth,player.pos.y - player.halfHeight},
		{player.pos.x + player.halfWidth,player.pos.y - player.halfHeight},
		{player.pos.x - player.halfWidth,player.pos.y + player.halfHeight},
		{player.pos.x + player.halfWidth,player.pos.y + player.halfHeight},
		{0,0},
		{0,0},

	};

	int mousePosX = 0;
	int mousePosY = 0;

	int playerTexture = Novice::LoadTexture("white1x1.png");



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

		player.pos.x += player.speed.x;

		player.speed.x += player.acceleration.x;

		player.leftTop.x = player.pos.x - player.halfWidth;
		player.leftTop.y = player.pos.y - player.halfHeight;

		player.rightTop.x = player.pos.x + player.halfWidth;
		player.rightTop.y = player.pos.y - player.halfHeight;


		player.leftBottom.x = player.pos.x - player.halfWidth;
		player.leftBottom.y = player.pos.y + player.halfHeight;

		player.rightBottom.x = player.pos.x + player.halfWidth;
		player.rightBottom.y = player.pos.y + player.halfHeight;


		Novice::GetMousePosition(&mousePosX, &mousePosY);

		if (mousePosX < WIN_WIDTH / 2) {

			player.acceleration.x = -0.1f;
		}
		else if (mousePosX >= WIN_WIDTH / 2) {
			player.acceleration.x = 0.1f;
		}
		///
		/// 
		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///

		Novice::DrawLine(WIN_WIDTH / 2, 0, WIN_WIDTH / 2, WIN_HEIGHT, WHITE);

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
			0, 0, 1, 1, playerTexture, WHITE


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
