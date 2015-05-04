#include <stdio.h>
#include <string.h>
#define MAX_ANIMALS 8
#define ON_BOARD 0
#define MY_HAND 1
#define ENEMY_HAND 2
#define MAX_MOVE 50
#define PLACE 5
#define OUT_OF_BOARD '\0'

typedef struct{
	int x, y;
	char type;
} Animal;
typedef struct{
	Animal animal[MAX_ANIMALS];
	int num;
} Group;
typedef struct{
	char type;
	int prevX,prevY,dirc;
} Move;
typedef struct{
	Move list[MAX_MOVE];
	int num;
} Movelist;

Animal newAnimal(char type, int x, int y);
Move newMove(char type, int prevX, int prevY, int dirc);
void inputBoard(Group* onBoard, Group* myHand, Group* enemyHand);
int isInBoard(int x, int y);
char getTile(int x, int y, Group group);
int moveX(int x, int dirc);
int moveY(int y, int dirc);
int isMine(char target);
int isEnemy(char target);
int testDirc(int x, int y, int dirc, Group onBoard);
void addMove(char type, int x, int y, int dirc, Group onBoard, Movelist* list);
void moveGiraffe(int x, int y, Group onBoard, Movelist* list);
void moveChick(int x, int y, Group onBoard, Movelist* list);
void moveHen(int x, int y, Group onBoard, Movelist* list);
void moveLion(int x, int y, Group onBoard, Movelist* list);
void moveElephant(int x, int y, Group onBoard, Movelist* list);
void animalMove(Animal anim, Group onBoard, Movelist* list);
void printMove(Move move);
Movelist getMovelist(Group onBoard, Group myHand);