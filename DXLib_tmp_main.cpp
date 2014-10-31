#include "DxLib.h"

void Draw();
void Ctrl();
void Init();
void KeyStack();
int CheckStateKey( unsigned char );
void ModeSwitcher();

int stateKey[256];

//システムの初期化系はここ(ほかにChangeWindowModeとか
int SystemInit(){
	if( DXLib_Init() != 0 ) return -1;
	if( SetDrawScreen() != 0 ) return -2;
	return 0;
}

//whileループの条件として行わせるのはここ
int MainProcess(){
	if( ProcessMassage() != 0 ) return -1;
	if( ClearDrawScreen() != 0 ) return -2;
	return 0;
}

//極力さわらない。
int WINAPI WinMain( HINSTANCE hInst,HINSTANCE hPrevInst,LPSTR lpCmdLine,int nCmdShow ){
	if( SystemInit() != 0 ) return -1;

	Init();

	while( MainProcess() == 0 ){
		
		ModeSwitcher();
		Draw();
		
		ScreenFlip();
	}

	Finalize();
	DxLib_Init();
	return 0;
}

//キー入力の受付。基本ノータッチ。
void KeyStack(){
	char key[256];
	GethitKeyStateAll( key );
	for( int i = 0;i < 256;i++ ){
		if( key[i] == 0 ) stateKey[i] = 0;
		else			  stateKey[i]++;
	}
}

//キーの入力状態を呼ぶ。なるべく[x]のような配列参照は使わない
//引数にKEY_INPUT_~を指定する
int CheckStateKey( unsigned char Handle ){ return stateKey[ Handle ]; }

//変数の初期化、画像のロードなどの初期化系はすべてここ
void Init(){

}

//バックグラウンドだったりの半永久的に表示しつづけるものはここ。
void Draw(){

}

//メモリの開放、ハンドルのリリースなどの終了処理はここ。
void Finalize(){

}

//ゲームのシステム部を書くところ。
//ScreenFlipは記述しない←重要
//whileによる処理ストップ系は非推奨、switchにモードを扱う変数で分岐させて、
//#defineなどでモードの識別子を定義するなどして、一本化（上から下までwhileなどの
//内部ループがない。）にする。
void ModeSwitcher(){

	/*例
	//gameMode : モードを扱う変数。
	switch( gameMode ){
	case MENU: //メニュー
	//ブロック崩しでいう、スペースキー待ちうけ中。CheckStateKey( KEY_INPUT_SPACE )で待ち受けて
	//戻り値が1( CheckStateKey( KEY_INPUT_SPACE ) == 1 )のときにgameMode = MAINでゲーム本体部にうつる。
		~~~~省略。
		break;
	
	case MAIN: //ゲーム本体部
	//ブロックの表示、プレイヤーバーの操作部。
	//ゲームオーバー条件になるとgameMode = GAMEOVERでGAMEOVER画面にうつる。
		~~~~省略。
		break;

	case GAMEOVER://ゲームオーバー画面
		~~~~省略
		break;
	}

	*/

	//ScreenFlip() はかかない。

}
//switchのなかで外部の関数を呼んですっきりさせるとあとあと見やすいソースができる。

//インデントはTabキーで一定の空白をあける、字下げをする。

/*字下げ例：

	for( int i = 0;i < 6;i++ ){
		if( i == 1 ) printf( "ba-ka );
	}

	字下げをすると処理の流れがつかみやすい、コードが見やすいなどのメリットがある
*/