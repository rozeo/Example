#include <iostream>
#include <random>
#include <ctime>

#define SIZE_X 7
#define SIZE_Y 10
#define ELEMENTS 5
#define DEL_OVER 4

char map[SIZE_X+1][SIZE_Y+1];
bool f_map[SIZE_X][SIZE_Y];

void del();
int del_loop( int x,int y,char c,int cnt);

int main(){

	std::mt19937 mt((unsigned int)time(NULL));

	//盤面状態の生成
	for( int i = 0;i < SIZE_Y;i++ ){
		for( int j = 0;j < SIZE_X;j++ )
			map[i][j] = (char)( mt()%ELEMENTS + '1' ),f_map[i][j] = false;
	}

	for( int i = 0;i < SIZE_Y;i++ ){
		for( int j = 0;j < SIZE_X;j++ )
			std::cout << map[i][j];
		std::cout << std::endl;
	}
	std::cout << std::endl;

	clock_t start = clock();
	del();
	clock_t end = clock();

	for( int i = 0;i < SIZE_Y;i++ ){
		for( int j = 0;j < SIZE_X;j++ )
			std::cout << map[i][j];
		std::cout << std::endl;
	}

	return 0;
}

void del(){
	for( int i = 0;i < SIZE_X;i++ )
		for( int j = 0;j < SIZE_Y;j++ )
			del_loop(i,j,map[i][j],0);
}
int del_loop(int x,int y,char c,int cnt){

	//探索済みかチェック、していなけばフラグをtrue
	if( f_map[x][y] )
		return cnt;
	else
		f_map[x][y] = true;

	//対象のナンバーと同じかチェック、違った場合はフラグを折って抜ける
	if( map[x][y] == '0' )
		return cnt;
	if( map[x][y] == c ) cnt++;
	else{
		f_map[x][y] = false;
		return cnt;
	}
	//４方向
	if( x - 1 >= 0 )
		cnt = del_loop( x-1,y,map[x][y],cnt );

	if( x + 1 < SIZE_Y )
		cnt = del_loop( x+1,y,map[x][y],cnt );

	if( y - 1 >= 0 )
		cnt = del_loop( x,y-1,map[x][y],cnt );

	if( y + 1 < SIZE_X )
		cnt = del_loop( x,y+1,map[x][y],cnt );

	//消去判定
	if( cnt >= DEL_OVER ) map[x][y] = '0';
	//フラグ折り
	f_map[x][y] = false;
	return cnt;
}