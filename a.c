#include <stdio.h>
#include <string.h>
int MAX;

int main(int args,char *argv[]){
    MAX = args;
    loop(args,argv,0);
}

int Print(char *a,int b,int op){
	if((op == 1 && b < 0)||(op == 0 && b >= strlen(a))){
		printf(" ");
		return 0;
	}
	printf("%c",a[b]);
	return Print(a,(op == 0)?b+1:b-1,op);
}


int f0(int a,char *b[],int c){
	if(c == a) return 0;
	Print(b[c],0,0);
	return f0(a,b,c+1);
}

int f1(int a,char *b[],int c){
	if(c <= 0) return 0;
	Print(b[c-1],0,0);
	return f1(a,b,c-1);
}

int f2(int a,char *b[],int c){
	if(c == a) return 0;
	Print(b[c],strlen(b[c])-1,1);
	return f2(a,b,c+1);
}

int f3(int a,char *b[],int c){
	if(c <= 0) return 0;
	Print(b[c-1],strlen(b[c-1])-1,1);
	return f3(a,b,c-1);
}

int loop(int a,char *b[],int c){
	switch(c){
	case 0:
		f0(a,b,0);
		break;
	case 1:
		f1(a,b,MAX);
		break;
	case 2:
		f2(a,b,0);
		break;
	case 3:
		f3(a,b,MAX);
		break;
	}

	printf("\n");
	if(c == MAX) return 0;
	else		 return loop(a,b,c+1);
}
