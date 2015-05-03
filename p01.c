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

Animal newAnimal(char type, int x, int y){
	Animal anim;
	anim.x = x;
	anim.y = y;
	anim.type = type;
	return anim;
}
Move newMove(char type, int prevX, int prevY, int dirc){
	Move move;
	move.type = type;
	move.prevX = prevX;
	move.prevY = prevY;
	move.dirc = dirc;
	return move;
}
void inputBoard(Group* onBoard, Group* myHand, Group* enemyHand){
	int i,j;
	char buff[MAX_ANIMALS];
	onBoard->num = 0;
	myHand->num = 0;
	enemyHand->num = 0;
	for(i=0;i<4;i++){
        fgets(buff,10,stdin);
        for(j=0;j<3;j++){
            if(buff[j] == 'o')
				continue;
            onBoard->animal[onBoard->num] = newAnimal(buff[j],j,i);
            onBoard->num ++;
        }
    }
	fgets(buff,MAX_ANIMALS,stdin);
	for(i=0;buff[i]!='\n';i++){
		myHand->animal[myHand->num] = newAnimal(buff[i],0,0);
		myHand->num++;
	}
    fgets(buff,MAX_ANIMALS,stdin);
	for(i=0;buff[i]!='\n';i++){
		enemyHand->animal[enemyHand->num] = newAnimal(buff[i],0,0);
		enemyHand->num++;
	}
}
int isInBoard(int x, int y){
	if(x>2 || x<0 || y>3 || y<0)
		return 0;
	else
		return 1;
}
char getTile(int x, int y, Group group){
	if(isInBoard(x,y)==0)
		return OUT_OF_BOARD;
	int i;
	for(i=0;i<group.num;i++){
		if(x==group.animal[i].x 
			&& y==group.animal[i].y)
			return group.animal[i].type;
	}
	return 'o';
}

int moveX(int x, int dirc){
	if(dirc%3 == 1)
		return x-1;
	else if(dirc%3 == 0)
		return x+1;
	else return x;
}
int moveY(int y, int dirc){
	dirc--;
	if(dirc/3 == 2)
		return y-1;
	else if(dirc/3 == 1)
		return y;
	else
		return y+1;
}
int isMine(char target){
	if(target == 'L'
		|| target == 'G'
		|| target == 'C'
		|| target == 'E'
		|| target == 'H')
		return 1;
	else return 0;
}
int isEnemy(char target){
	if(target == 'l'
		|| target == 'g'
		|| target == 'c'
		|| target == 'e'
		|| target == 'h')
		return 1;
	else return 0;
}
int testDirc(int x, int y, int dirc, Group onBoard){
	x = moveX(x,dirc);
	y = moveY(y,dirc);
	char target = getTile(x,y,onBoard);
	if(!isMine(target) && target!=OUT_OF_BOARD)
		return 1;
	else
		return 0;
}
void addMove(char type, int x, int y, int dirc, Group onBoard, Movelist* list){
	if(dirc==PLACE){
		if(getTile(x,y,onBoard)=='o'){
			list->list[list->num] = newMove(type,x,y,PLACE);
			list->num++;
		}else
			return;
	}else{
		if(testDirc(x,y,dirc,onBoard)){
			list->list[list->num] = newMove(type,x,y,dirc);
			list->num++;
		}else
			return;
	}
}
void moveGiraffe(int x, int y, Group onBoard, Movelist* list){
	addMove('G',x,y,2,onBoard,list);
	addMove('G',x,y,4,onBoard,list);
	addMove('G',x,y,6,onBoard,list);
	addMove('G',x,y,8,onBoard,list);
}
void moveChick(int x, int y, Group onBoard, Movelist* list){
	addMove('C',x,y,8,onBoard,list);
}
void moveHen(int x, int y, Group onBoard, Movelist* list){
	addMove('H',x,y,2,onBoard,list);
	addMove('H',x,y,4,onBoard,list);
	addMove('H',x,y,6,onBoard,list);
	addMove('H',x,y,7,onBoard,list);
	addMove('H',x,y,8,onBoard,list);
	addMove('H',x,y,9,onBoard,list);
}
void moveLion(int x, int y, Group onBoard, Movelist* list){
	addMove('L',x,y,1,onBoard,list);
	addMove('L',x,y,2,onBoard,list);
	addMove('L',x,y,3,onBoard,list);
	addMove('L',x,y,4,onBoard,list);
	addMove('L',x,y,6,onBoard,list);
	addMove('L',x,y,7,onBoard,list);
	addMove('L',x,y,8,onBoard,list);
	addMove('L',x,y,9,onBoard,list);
}
void moveElephant(int x, int y, Group onBoard, Movelist* list){
	addMove('E',x,y,1,onBoard,list);
	addMove('E',x,y,3,onBoard,list);
	addMove('E',x,y,7,onBoard,list);
	addMove('E',x,y,9,onBoard,list);
}
void animalMove(Animal anim, Group onBoard, Movelist* list){
	switch(anim.type){
	case 'G':
		moveGiraffe(anim.x,anim.y,onBoard,list);
		return;
	case 'C':
		moveChick(anim.x,anim.y,onBoard,list);
		return;
	case 'H':
		moveHen(anim.x,anim.y,onBoard,list);
		return;
	case 'L':
		moveLion(anim.x,anim.y,onBoard,list);
		return;
	case 'E':
		moveElephant(anim.x,anim.y,onBoard,list);
		return;
	default :
		return;
	}
}
void printMove(Move move){
	if(move.dirc == PLACE)
		printf("%c %c %c%d\n", move.type, 'P', 'A'+move.prevX, move.prevY+1);
	else{
		int x = moveX(move.prevX,move.dirc),
			y = moveY(move.prevY,move.dirc);
		printf("%c %c%d %c%d\n", move.type, 'A'+move.prevX, move.prevY+1, 'A'+x, y+1);
	}
}

int main(){
	Group onBoard, myHand, enemyHand;
	Movelist movelist;
	movelist.num = 0;
	int i,a,b;
	inputBoard(&onBoard, &myHand, &enemyHand);
	for(i=0;i<onBoard.num;i++){
		animalMove(onBoard.animal[i],onBoard,&movelist);
	}
	for(a=0;a<3;a++){
		for(b=0;b<4;b++){
			for(i=0;i<myHand.num;i++){
				addMove(myHand.animal[i].type,a,b,PLACE,onBoard,&movelist);
			}
		}
	}
	for(i=0;i<movelist.num;i++){
		printMove(movelist.list[i]);
	}
	return 0;
}
