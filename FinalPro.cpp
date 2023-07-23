//① ファイルの導入
#include <DXLib.h> //公開されている「DXライブラリ」の追加
#include <iostream>
#include <string>
#include <vector>
using namespace std;



//② 主な定義
int WIN_WIDTH = 800; //ウィンドウの横幅
int WIN_HEIGHT = 600; //ウィンドウの縦幅
int CELL = 40; //ウィンドウの最小単位を40×40ピクセルと定義
int WIN_Quan_X = 800 / 40; //ウィンドウの横幅をCELLで分割した個数（ = 20個）
int WIN_Quan_Y = 600 / 40; //ウィンドウの縦幅をCELLで分割した個数（ = 15個）
int WIN_POS_X = 200; //ウィンドウのx座標
int WIN_POS_Y = 20; //ウィンドウのy座標
int Count = 0; //カウンタを設定

float PI = 3.141592654f; //円周率π
float yadd = 0.0f; //重力加速度座標として用意
float JumpSpeed = -8.0f; //ジャンプの初速度
float gravity = 0.2f; //重力加速度

bool isJumping = false; //ジャンプ中か否かを判定する演算子「isJumping」を作成



//③ 各シーンを設定する列挙型「SEE」を作成
enum SEE
{
	SEE_Title,
	SEE_00_Intro,
	SEE_01_Action1,
	SEE_02_Action2,
	SEE_03_Action3,
	SEE_END,
	SEE_Fail_Game,
	SEE_Fail_Sake,
	SEE_Fail_Fall,
	SEE_Fail_Kadai,
};

//画面の初期値をタイトルに設定
int Sce = SEE::SEE_Title;



//④ 色を定義するクラス「MYColor」を作成
class MYColor
{
public:

	int White;
	int Black;
	int Green;
	int Red;
	int Blue;

	//色を読み込むmyColor型の関数「Read」を作成
	void Read()
	{
		White = GetColor(255, 255, 255);
		Black = GetColor(0, 0, 0);
		Green = GetColor(0, 255, 0);
		Red = GetColor(255, 0, 0);
		Blue = GetColor(0, 0, 255);
	}
};

MYColor myColor; //クラス「MYColor」のインスタンス「myColor」を作成



//⑤ 文字のフォント、大きさ、太さを定義するクラス「MYFont」を作成
class MYFont
{
public:

	vector<int> C1; //空のvector「C1」を作成

	//myFont型の関数「Read」を作成し、vector 「C1」にMSゴシック体の文字の大きさ、太さを記録する（50段階）                              
	void Read()
	{
		for (int i = 0; i < 50; i++)
		{
			C1.push_back(CreateFontToHandle("MSゴシック", i, 5, DX_FONTTYPE_NORMAL));
		}
	}
};

MYFont myFont; //クラス「MYFont」のインスタンス「myFont」の作成



vector<int> Key(256); // キーが押されているフレーム数を格納する、要素数256のvector「key」を作成

//⑥ キーの入力状態を更新する関数「myUpdateKey」を作成
int myUpdateKey()
{
	vector<char> TmpKey(256); // 現在のキーの入力状態を格納する、要素数256のvector「TmpKey」を作成

	GetHitKeyStateAll(TmpKey.data()); // 全てのキーの入力状態を得る

	for (vector<char>::iterator it = TmpKey.begin(); it != TmpKey.end(); ++it)
	{
		int KeyNum = distance(TmpKey.begin(), it); // イテレータの位置からキーの番号を計算

		if (*it != 0) // i番に対応するキーが押されていたら
		{
			Key[KeyNum] = Key[KeyNum] + 1; // 加算
		}
		else // 押されていなければ
		{
			Key[KeyNum] = 0; // 0にする
		}
	}
	return 0;
}



//⑦ 画像を定義するクラス「myPicture」を作成
class MYPicture
{
public:

	int Title;
	int Intro;
	int Stage1;
	int Stage2;
	int Stage3;
	int END;
	int END_P;
	int Fail_Game;
	int Fail_Sake;
	int Fail_Fall;
	int Fail_Kadai;
	int Block1;
	int Block2;
	int Block3;
	int Arrow;
	int Player;
	int Enemy1;
	int Enemy2;
	int Enemy3;
	int Professor;


	void Read()
	{
		//画像のロード
		Title = LoadGraph("画像/タイトル.png"); //タイトル画面の背景
		Intro = LoadGraph("画像/イントロ.png"); //イントロ画面の背景
		Stage1 = LoadGraph("画像/ステージ1.png"); //1stステージ画面の背景
		Stage2 = LoadGraph("画像/ステージ2.png"); //2ndステージ画面の背景
		Stage3 = LoadGraph("画像/ステージ3.png"); //3rdステージ画面の背景
		END = LoadGraph("画像/エンド.png"); //クリア画面の背景
		END_P = LoadGraph("画像/エンド_人.png"); //クリア画面の人の画像
		Fail_Game = LoadGraph("画像/失敗_ゲーム.png"); //失敗画面（ゲーム）の背景
		Fail_Sake = LoadGraph("画像/失敗_酒.png"); //失敗画面（酒）の背景
		Fail_Fall = LoadGraph("画像/失敗_落下.png"); //失敗画面（落下）の背景
		Fail_Kadai = LoadGraph("画像/失敗_課題.png"); //失敗画面（課題）の背景
		Block1 = LoadGraph("画像/ブロック1.png"); //土ブロックの画像
		Block2 = LoadGraph("画像/ブロック2.png"); //コンクリートブロックの画像
		Block3 = LoadGraph("画像/ブロック3.png"); //濃い目のコンクリートブロックの画像
		Arrow = LoadGraph("画像/矢印.png"); //矢印の画像
		Player = LoadGraph("画像/プレーヤー.png"); //プレーヤーの画像
		Enemy1 = LoadGraph("画像/ゲーム機.png"); //ゲーム機の画像
		Enemy2 = LoadGraph("画像/酒.png"); //酒の画像
		Enemy3 = LoadGraph("画像/課題.png"); //課題の画像
		Professor = LoadGraph("画像/教授.png"); //教授の画像
	}
};

MYPicture myPicture; //クラス「MYPicture」のインスタンス「myPicture」を作成



//⑧ サウンドを定義するクラス「MYSound」を作成
class MYSound
{
public:
	int BGM;

	void Read()
	{
		//サウンドのロード
		BGM = LoadSoundMem("サウンド/天国と地獄.ogg"); //プレイ中のBGM「天国と地獄」
	}
};

MYSound mySound; //クラス「MYSound」のインスタンス「mySound」の作成



//⑨ タイトル画面を出力するクラス「MYTitle」の作成
class MYTitle
{
public:

	void Out()
	{
		DrawGraph(0, 0, myPicture.Title, TRUE); //タイトル画面を描画
		DrawFormatStringToHandle(280, 80, myColor.Red, myFont.C1[20], "誘惑に負けず、"); //ゲームタイトルを表示
		DrawFormatStringToHandle(300, 100, myColor.Red, myFont.C1[38], "単位を取れ!"); 
		DrawFormatStringToHandle(250, 500, myColor.Green, myFont.C1[30], "Enter : ゲームスタート"); 

		if (Key[KEY_INPUT_RETURN] == 1) //Enterキーを押すと
		{
			Sce = SEE::SEE_00_Intro; //シーンを「イントロ」に切り替え
		}
	}
};

MYTitle myTitle; //クラス「MYTitle」のインスタンス「myTitle」を作成



//⑩ イントロ画面を出力するクラス「MYIntro」を作成
class MYIntro
{
public:

	void Out()
	{
		DrawGraph(0, 0, myPicture.Intro, TRUE); //イントロ画面を描画
		DrawFormatStringToHandle(500, 500, myColor.Green, myFont.C1[30], "Enter : プレイ");

		if (Key[KEY_INPUT_RETURN] == 1) //Enterキーを押すと
		{
			Sce = SEE::SEE_01_Action1; //シーンを「アクション1」に切り替え
		}
	}
};

MYIntro myIntro; //クラス「MYIntro」のインスタンス「myIntro」を作成



//⑪ 1stステージ画面を出力するクラス「MYStage1」の作成
class MYStage1
{
public:

	// 1stステージのマップデータを配列「MapData1」に格納
	char MapData1[15][20] =
	{
		0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0,
		0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0,
		0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0,
		0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0,
		0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0,

		0,0,0,0,0, 0,0,0,0,0, 0,0,0,1,1, 1,1,0,0,0,
		0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0,
		0,0,0,0,0, 0,0,0,0,0, 1,0,0,0,0, 0,0,0,0,0,
		0,0,0,0,0, 0,0,0,0,0, 1,0,0,0,0, 0,0,0,0,0,
		0,0,0,0,1, 0,0,0,0,0, 1,0,0,0,0, 0,0,0,0,0,

		1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1,
		0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0,
		0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0,
		0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0,
		0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0,
	};


	void Out()
	{
		DrawGraph(0, 0, myPicture.Stage1, TRUE); //1stステージの背景を描画

		// 1stステージマップの描画
			for (int i = 0; i < 15; i++)
			{
				for (int j = 0; j < 20; j++)
				{
					// 土ブロックの描画
					if (MapData1[i][j] == 1) //配列「MapData」に格納された値が1ならば
					{
						DrawGraph(CELL * j, CELL * i, myPicture.Block1, TRUE); //40×40ピクセルで土ブロックを描画
					}
				}
			}
		
		DrawGraph(760, 340, myPicture.Arrow, TRUE); //矢印を(760,360)に描画
		DrawFormatStringToHandle(760, 330, myColor.Black, myFont.C1[15], "Next"); //矢印の上に「Next」の文字を描画
	
	}
};

MYStage1 myStage1; //クラス「MYStage1」のインスタンス「myStage1」を作成




//⑫ 2ndステージ画面を出力するクラス「MYStage2」を作成
class MYStage2
{
public:

	// 2ndステージのマップデータを配列「MapData2」に格納
	char MapData2[15][20] =
	{
		0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0,
		0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0,
		0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0,
		0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0,
		0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0,

		0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0,
		0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0,
		0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0,
		0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,2,2,2,
		0,0,0,0,0, 0,0,2,0,0, 0,0,0,0,0, 0,0,0,0,0,

		2,2,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0,
		0,0,0,0,0, 0,0,0,0,0, 0,0,0,2,0, 0,0,0,0,0,
		0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0,
		0,0,0,2,2, 2,0,0,0,0, 0,2,0,0,0, 0,0,0,0,0,
		0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0,
	};


	void Out()
	{
		DrawGraph(0, 0, myPicture.Stage2, TRUE); //2ndステージの背景を描画

		// 2ndステージマップの描画
		{
			int i, j;

			for (i = 0; i < 15; i++)
			{
				for (j = 0; j < 20; j++)
				{
					// コンクリートブロックの描画
					if (MapData2[i][j] == 2) //配列「MapData2」に格納された値が2ならば
					{
						DrawGraph(CELL * j, CELL * i, myPicture.Block2, TRUE); //40×40ピクセルでコンクリートブロックを描画
					}
				}
			}
		}

		DrawGraph(760, 280, myPicture.Arrow, TRUE); //矢印を(760,280)に描画
		DrawFormatStringToHandle(760, 270, myColor.Black, myFont.C1[15], "Next"); //矢印の上に「Next」の文字を描画
	}
};

MYStage2 myStage2; //クラス「MYStage2」のインスタンス「myStage2」を作成



//⑬ 3rdステージ画面を出力するクラス「MYStage3」を作成
class MYStage3
{
public:

	// 3rdステージのマップデータを配列「MapData3」に格納
	char MapData3[15][20] =
	{
		0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0,
		0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0,
		0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0,
		0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0,
		0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0,

		0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0,
		0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0,
		0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0,
		0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0,
		0,0,0,0,0, 0,3,3,3,3, 3,3,3,3,3, 3,3,3,3,3,

		0,0,0,0,0, 3,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0,
		0,0,0,0,3, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0,
		0,0,0,3,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0,
		3,3,3,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0,
		0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0,
	};


	void Out()
	{
		DrawGraph(0, 0, myPicture.Stage3, TRUE); //3rdステージの背景を描画

		// 3rdステージマップの描画
		{
			int i, j;

			for (i = 0; i < 15; i++)
			{
				for (j = 0; j < 20; j++)
				{
					// 濃い目のコンクリートブロックの描画
					if (MapData3[i][j] == 3) //配列「MapData3」に格納された値が3ならば
					{
						DrawGraph(CELL * j, CELL * i, myPicture.Block3, TRUE); //40×40ピクセルで濃い目のコンクリートブロックを描画
					}
				}
			}
		}
	}
};

MYStage3 myStage3; //クラス「MYStage3」のインスタンス「myStage3」を作成



//⑭ 失敗画面を描画するクラス「MYFail_Game」を作成
class MYFail
{
public:

	int a;
	int b;
	int handle;


	void Out(int newA, int newB,int newHandle)
	{
		a = newA;
		b = newB;
		handle = newHandle;

		DrawGraph(a, b, handle, TRUE); //失敗画面を描画
		DrawFormatStringToHandle(400, 50, myColor.Red, myFont.C1[38], "不可");
		DrawFormatStringToHandle(50, 500, myColor.Green, myFont.C1[30], "Enter : 続きから");
		DrawFormatStringToHandle(50, 530, myColor.Green, myFont.C1[30], "Escape : タイトルに戻る");

		if (Key[KEY_INPUT_RETURN] == 1) //Enterキーを押すと
		{
			Sce = SEE::SEE_01_Action1; //シーンを「アクション1」に切り替え
		}

		if (Key[KEY_INPUT_ESCAPE] == 1) //Escapeキーを押すと
		{
			Sce = SEE::SEE_Title; //シーンを「タイトル」に切り替え
		}
	}
};
 
MYFail myFail_Game; //失敗画面（ゲーム）を描画するインスタンス「myFail_Game」を作成
MYFail myFail_Sake; //失敗画面（酒）を描画するインスタンス「myFail_Sake」を作成
MYFail myFail_Fall; //失敗画面（落下）を描画するインスタンス「myFail_Fall」を作成
MYFail myFail_Kadai; //失敗画面（課題）を描画するインスタンス「myFail_Kadai」を作成


//⑮ クリア画面を表示するクラス「MYEND」を作成
class MYEND
{
public:

	void Out()
	{
		DrawGraph(0, 0, myPicture.END, TRUE); //クリア画面を描画
		DrawGraph(200, 100, myPicture.END_P, TRUE); //クリア画面の人を描画
		DrawFormatStringToHandle(400, 50, myColor.Blue, myFont.C1[38], "秀");
		DrawFormatStringToHandle(50, 500, myColor.Green, myFont.C1[30], "Enter : タイトルに戻る");

		if (Key[KEY_INPUT_RETURN] == 1) //Enterキーを押すと
		{
			Sce = SEE::SEE_Title; //シーンを「タイトル」に切り替え
		}
	}
};

MYEND myEND; //クラス「MYEND」のインスタンス「myEND」を作成



//⑯ キャラクターの基本情報を設定する構造体「MYChara」を作成
struct MYChara
{
	//設定する5つの情報
	const int WIDTH; //横幅
	const int HEIGHT; //縦幅
	const int Ini_X; //初期x座標
	const int Ini_Y; //初期y座標
	const int Speed; //移動速度

	//その他、計算される情報
	int X = Ini_X; //x座標
	int Y = Ini_Y; //y座標
	int Left = X; //左側座標
	int Right = X + WIDTH; //〃右
	int Top = Y; //〃上
	int Bottom = Y + HEIGHT; //〃下

};

MYChara Player{ 75, 90, 0, 310, 2 }; //構造体「MYChara」のインスタンス「Player」を作成
MYChara Game1{ 30, 38, 325, 362, 0 }; //構造体「MYChara」のインスタンス「Game1」を作成（1stステージ用）
MYChara Sake1{ 30, 38, 605, 160, 0 }; //構造体「MYChara」のインスタンス「Sake1」を作成
MYChara Game2{ 30, 38, 160 + 80, 240 + 80, 0 }; //構造体「MYChara」のインスタンス「Game2」を作成（回転中心[160,240]、回転半径80）（2ndステージ用）
MYChara Sake2{ 30, 38, 600, 250, 0 }; //構造体「MYChara」のインスタンス「Sake2」を作成
MYChara Professor{ 110, 150, 680, 210, 0 }; //構造体「MYChara」のインスタンス「Professor」を作成
MYChara Kadai{ 30, 38, 660, 260, 4 }; //構造体「MYChara」のインスタンス「Kadai」を作成



//⑰ クラス「MYStage1」を継承し、1stステージ内のプレーヤーの動作を実現するクラス「MYAction1」を作成
class MYAction1 : public MYStage1
{
public:

	void Out()
	{
		myCal();
		myCheckHit_Block(MapData1,1);
		myCheckHit_Enemy1();
		myChange1_2();
		myDraw1();
	};


	//座標計算を行う関数「myCal」を作成
	void myCal()
	{
		if (Key[KEY_INPUT_RIGHT] >= 1)  Player.X += Player.Speed; //→キーが押されている間は右に移動
		else if (Key[KEY_INPUT_LEFT] >= 1)  Player.X -= Player.Speed; //←キーが押されている間は左に移動

		if (!isJumping && Key[KEY_INPUT_UP] == 1) //ジャンプ中でなく、↑キーが押されたら
		{
			// ジャンプ開始時に速度を初期化
			yadd = JumpSpeed;
			isJumping = true;
		}

		Player.Y += yadd;
		yadd += gravity;
	}


	//ブロックとの当たり判定を行う関数「myCheckHit」を作成
	void myCheckHit_Block(char Ary[][20], int num)
	{
		//マップデータを用いたブロックとの当たり判定
		for (int i = 0; i <= 15; i++)
		{
			for (int j = 0; j <= 20; j++)
			{
				int T = num;

				if (Ary[i][j] == T) //配列に格納された値がTならば
				{
					//ブロックの座標を変数として記録
					int Block_Left = CELL * j; //ブロックの左側座標
					int Block_Right = Block_Left + 40; //〃右
					int Block_Top = CELL * i; //〃上
					int Block_Bottom = Block_Top + 40; //〃下


					if (Block_Left < Player.X + Player.WIDTH && Player.X < Block_Right && Block_Top < Player.Y + Player.HEIGHT && Player.Y < Block_Bottom) //プレーヤーがブロックにめり込んだ時に
					{
						// ブロックの上側との判定
						if (Block_Top < Player.Y + Player.HEIGHT && Player.Y + Player.HEIGHT - yadd < Block_Top) //ブロックに上から接触したら
						{
							Player.Y = Block_Top - Player.HEIGHT; //プレーヤーのy座標をブロックの上で固定
							yadd = 0.0f; //落下を停止
							isJumping = false; //ジャンプ中ではない
						}

						//ブロックの下側との判定
						else if (Block_Bottom < Player.Y + yadd && Player.Y < Block_Bottom) //ブロックに下から接触したら
						{
							Player.Y = Block_Bottom; //プレーヤーのy座標をブロックの下で固定し
							yadd = 0.2f; //重力をかける
						}

						// ブロックの左側との判定
						else if (Block_Left < Player.X + Player.WIDTH && Player.X < Block_Left) //ブロックに左から接触したら
						{
							Player.X = Block_Left - Player.WIDTH; //プレーヤーのx座標をブロックの左で固定
						}

						// ブロックの右側との判定
						else if (Block_Right < Player.X + Player.WIDTH && Player.X < Block_Right) //ブロックに右から接触したら
						{
							Player.X = Block_Right; //プレーヤーのx座標をブロックの右で固定
						}
					}
				}
			}
		}
	}



	//1stステージ内の敵との当たり判定を行う関数「myCheckHit_Enemy1」を作成
	void myCheckHit_Enemy1()
	{
			//ゲーム機との当たり判定（PC内の計算の都合上?、Left、Right等を使用すると当たり判定が大きくずれてしまう）
		    if (Game1.X - Player.WIDTH <= Player.X && Player.X <= Game1.X + Game1.WIDTH && Game1.Y - Player.HEIGHT <= Player.Y && Player.Y <= Game1.Y + Game1.HEIGHT - Player.HEIGHT) //上下左右から接触したら
	     	{
			    Player.X = Player.Ini_X; //プレーヤーを初期位置に戻し（戻さないと、失敗した座標からゲームが始まるバグが発生）
			    Player.Y = Player.Ini_Y;

			    Sce = SEE::SEE_Fail_Game; //シーンを「失敗画面（ゲーム）」に切り替え
		    }
		
			//酒との当たり判定
			if (Sake1.X - Player.WIDTH <= Player.X && Player.X <= Sake1.X + Sake1.WIDTH && Sake1.Y - Player.HEIGHT <= Player.Y && Player.Y <= Sake1.Y + Sake1.HEIGHT - Player.HEIGHT) //上下左右から接触したら
			{
				Player.X = Player.Ini_X; //プレーヤーを初期位置に戻し
				Player.Y = Player.Ini_Y;

				Sce = SEE::SEE_Fail_Sake; //シーンを「失敗画面（酒）」に切り替え
			}
	}


	//ステージ1→2へのシーン切り替えを行う関数「myChange1_2」を作成
	void myChange1_2()
	{
		//2ndステージへの移行
		if (Player.X >= 800) //プレーヤーのx座標が800を超えたら
		{
			Player.X = Player.Ini_X; //プレーヤーを初期位置に戻し
			Player.Y = Player.Ini_Y;
			
			Sce = SEE::SEE_02_Action2; //シーンを「アクション2」に切り替える
		}
	}


	//1stステージに関する全ての描画を行う関数「myDraw1」を作成
	void myDraw1()
	{
		myStage1.Out();
		DrawGraph(Game1.X, Game1.Y, myPicture.Enemy1, TRUE); //ゲーム機を描画
		DrawGraph(Sake1.X, Sake1.Y, myPicture.Enemy2, TRUE); //酒を描画
		DrawGraph(Player.X, Player.Y, myPicture.Player, TRUE); //プレーヤーを描画
	}
};

MYAction1 myAction1; //クラス「MYAction1」のインスタンス「myAction1」を作成



//⑱ クラス「MYAction1」「MYStage2」を継承し、2ndステージ内のプレーヤー・敵の動作を実現するクラス「MYAction2」を作成
class MYAction2 : public MYAction1,public MYStage2
{
public:

	void Out()
	{
		myCal();
		myCheckHit_Block(MapData2,2);
		myCheckHit_Enemy2();
		myChange2_3();
		myDraw2();
	}


	//2ndステージ内の敵との当たり判定を行う関数「myCheckHit_Enemy2」を作成
	void myCheckHit_Enemy2()
	{
		//落下判定
		if (Player.Y > 600) //プレーヤーのy座標が600を超えたら
		{
			Player.X = Player.Ini_X; //プレーヤーを初期位置に戻し
			Player.Y = Player.Ini_Y;
	
			Sce = SEE::SEE_Fail_Fall; //シーンを「失敗画面（落下）」に切り替える
		}

		//動くゲーム機との当たり判定（PC内の計算の都合上?、Left、Right等を使用すると当たり判定が大きくずれてしまう）
		if (Game2.X - Player.WIDTH <= Player.X && Player.X <= Game2.X + Game2.WIDTH && Game2.Y - Player.HEIGHT <= Player.Y && Player.Y <= Game2.Y + Game2.HEIGHT - Player.HEIGHT)
		{
			Player.X = Player.Ini_X; //プレーヤーを初期位置に戻し
			Player.Y = Player.Ini_Y;
			
			Sce = SEE::SEE_Fail_Game; //シーンを「失敗画面（ゲーム）」に切り替え
		}

		//動く酒との当たり判定
		if (Sake2.X - Player.WIDTH <= Player.X && Player.X <= Sake2.X + Sake2.WIDTH && Sake2.Y - Player.HEIGHT <= Player.Y && Player.Y <= Sake2.Y + Sake2.HEIGHT - Player.HEIGHT)
		{
			Player.X = Player.Ini_X; //プレーヤーを初期位置に戻し
			Player.Y = Player.Ini_Y;

			Sce = SEE::SEE_Fail_Sake; //シーンを「失敗画面（酒）」に切り替え
		}
	}


	//ステージ2→3へのシーン切り替えを行う関数「myChange2_3」を作成
	void myChange2_3()
	{
		//3rdステージへの移行
		if (Player.X >= 800) //プレーヤーのx座標が800を超えたら
		{
			Player.X = Player.Ini_X; //プレーヤーを初期位置に戻し
			Player.Y = Player.Ini_Y;
			
			Sce = SEE::SEE_03_Action3; //シーンを「アクション3」に切り替える
		}
	}


	//2ndステージに関するすべての描画を行う関数「myDraw2」を作成
	void myDraw2()
	{
		//座標の更新
		Game2.X = Game2.Ini_X + 80 * cos(PI * 2 / 240 * Count); //動くゲーム機のx座標を更新
		Game2.Y = Game2.Ini_Y + 80 * sin(PI * 2 / 240 * Count); //動くゲーム機のx座標を更新
		Sake2.Y = Sake2.Ini_Y + sin(PI * 2 / 120 * Count) * 150; //動く酒のy座標を更新

		//描画
		myStage2.Out();
		DrawRotaGraph(Game2.X, Game2.Y, 1.0, 0.0, myPicture.Enemy1, TRUE);
		DrawRotaGraph(Sake2.X, Sake2.Y, 1.0, 0.0, myPicture.Enemy2, TRUE);
		DrawGraph(Player.X, Player.Y, myPicture.Player, TRUE);

		Count++; //カウンタを加算
	}
};

MYAction2 myAction2; //クラス「MYAction2」のインスタンス「myAction2」を作成



//⑲ クラス「MYAction1」「MYStage3」を継承し、3rdステージ内のプレーヤーの動作を実現するクラス「MYAction3」を作成
class MYAction3 : public MYAction1, public MYStage3
{
public:

	void Out()
	{
		myCal();
		myCheckHit_Block(MapData3, 3);
		myCheckHit_Enemy3();
		myChange3_END();
		myDraw3();
	}


	//3rdステージ内の敵との当たり判定を行う関数「myCheckHit_Enemy3」を作成
	void myCheckHit_Enemy3()
	{
		//動く課題との当たり判定
		if (Kadai.X - Player.WIDTH <= Player.X && Player.X <= Kadai.X + Kadai.WIDTH && Kadai.Y - Player.HEIGHT <= Player.Y && Player.Y <= Kadai.Y + Kadai.HEIGHT - Player.HEIGHT)
		{
			Player.X = Player.Ini_X; //プレーヤーを初期位置に戻し
			Player.Y = Player.Ini_Y;

			Sce = SEE::SEE_Fail_Kadai; //シーンを「失敗画面（課題）」に切り替え
		}
	}


	//ステージ3→ENDへのシーン切り替えを行う関数「myChange3_END」を作成
	void myChange3_END()
	{
		//ENDステージへの移行
		if (Professor.X - Player.WIDTH <= Player.X && Player.X <= Professor.X + Professor.WIDTH && Professor.Y - Player.HEIGHT <= Player.Y && Player.Y <= Professor.Y + Professor.HEIGHT - Player.HEIGHT) //教授と接触したら
		{
			Player.X = Player.Ini_X; //プレーヤーを初期位置に戻し
			Player.Y = Player.Ini_Y;
			
			Sce = SEE::SEE_END; //シーンを「クリア画面」に切り替え
		}
	}


	//3rdステージに関するすべての描画を行う関数「myDraw3」を作成
	void myDraw3()
	{
		//座標の更新
		Kadai.X -= Kadai.Speed; //課題のx座標を更新

		if (Kadai.X < 0) //課題のx座標が0未満となったら
		{
			Kadai.X = Kadai.Ini_X; //x座標を初期値に戻す
		}

		myStage3.Out();
		DrawGraph(Professor.X, Professor.Y, myPicture.Professor, TRUE); //教授を描画
		DrawGraph(Kadai.X, Kadai.Y, myPicture.Enemy3, TRUE); //課題を描画
		DrawGraph(Player.X, Player.Y, myPicture.Player, TRUE); //プレーヤーを描画
	}
};

MYAction3 myAction3; //クラス「MYAction3」のインスタンス「myAction3」を作成



//⑳ メイン関数
int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_  HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd)
{

	//ウィンドウの出力
	ChangeWindowMode(TRUE); // ウィンドウモードに設定
	SetWindowInitPosition(WIN_POS_X, WIN_POS_Y); //ウィンドウ左上の座標を(200,20)に設定
	SetGraphMode(WIN_WIDTH, WIN_HEIGHT, 32); //ウィンドウの大きさを800×600に設定
	SetWindowText("取れ!単"); //ウィンドウ名を「取れ!単」に変更　　　


	//基本設定
	DxLib_Init(); // DXライブラリ初期化処理
	SetDrawScreen(DX_SCREEN_BACK); //描画先を裏画面に設定


	//各クラスにおけるメンバ関数の読み込み　
	myColor.Read();
	myFont.Read();
	myPicture.Read();
	mySound.Read();
	myTitle.Out();
	myIntro.Out();
	myAction1.Out();
	myAction2.Out();
	myAction3.Out();
	myEND.Out();
	myFail_Game.Out(200, 100, myPicture.Fail_Game);
	myFail_Sake.Out(258, 100, myPicture.Fail_Sake);
	myFail_Fall.Out(245, 100,myPicture.Fail_Fall);
	myFail_Kadai.Out(200, 100, myPicture.Fail_Kadai);
	

	PlaySoundMem(mySound.BGM, DX_PLAYTYPE_BACK); //BGMの再生

	// while(メッセージ処理)
	while (ProcessMessage() == 0)
	{
		myUpdateKey(); //キーボードの入力情報を取得する
		ClearDrawScreen(); //画面を消す（書き換えのため）

		//シーンの切り替え
		switch (Sce)
		{
		case SEE::SEE_Title:
			myTitle.Out();
			break;

		case SEE::SEE_00_Intro:
			myIntro.Out();
			break;

		case SEE::SEE_01_Action1:
			myAction1.Out();
			break;

		case SEE::SEE_02_Action2:
			myAction2.Out();
			break;

		case SEE::SEE_03_Action3:
			myAction3.Out();
			break;

		case SEE::SEE_END:
			myEND.Out();
			break;

		case SEE::SEE_Fail_Game:
			myFail_Game.Out(200, 100, myPicture.Fail_Game);
			break;

		case SEE::SEE_Fail_Sake:
			myFail_Sake.Out(258, 100, myPicture.Fail_Sake);
			break;

		case SEE::SEE_Fail_Fall:
			myFail_Fall.Out(250, 100, myPicture.Fail_Fall);
			break;

		case SEE::SEE_Fail_Kadai:
			myFail_Kadai.Out(200, 100, myPicture.Fail_Kadai);
			break;
		}

		ScreenFlip(); //裏画面を表画面に反映
	}

	DxLib_End(); // DXライブラリ終了処理
	return 0;
}
