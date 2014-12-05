#include <stdio.h>
#include <stdlib.h>

int main( int args,char *argv[] ){
	if( args < 4 ){
		printf( "ユーザー名/リポジトリ名/コメントの指定がありません。\n" );
		return -1;
	}
	char command[256];
	
	sprintf( command,"git init" );
	printf( "%s\n",command );
	system( command );
	
	sprintf( command,"git add ." );
	printf( "%s\n",command );
	system( command );
	
	if( argv[3][0] == '\"' )
		sprintf( command,"git commit -m \"%s\"",&argv[3][1] );
	else
		sprintf( command,"git commit -m \"%s\"",&argv[3][1] );
	printf( "%s\n",command );
	system( command );
	
	sprintf( command,"git remote add origin git@github.com:%s/%s",argv[1],argv[2] );
	printf( "%s\n",command );
	system( command );
	
	sprintf( command,"git push origin master" );
	printf( "%s\n",command );
	system( command );
	
	printf( "%s\n",argv[0] );
	return 0;
}
