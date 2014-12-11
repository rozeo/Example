#include <stdio.h>
#include <string.h>

#define NODE 7
#define TREE 4

#define SIZE 64

int branch[NODE][TREE]={
  1,  2, -1, -1,
  0,  2,  3, -1,
  0,  1,  4, -1,
  1,  4,  5, -1,
  2,  3,  6, -1,
  3, -1, -1, -1,
  4, -1, -1, -1, 
};

char que[SIZE][NODE];
char length[SIZE];

void search( int start,int goal ){
  int rear = 1,front = 0;
  que[0][0] = start;
  length[0] = 0;
}
