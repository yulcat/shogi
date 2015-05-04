#include "main.h"

int main(){
	Group onBoard, myHand, enemyHand;
	inputBoard(&onBoard, &myHand, &enemyHand);
	Movelist movelist = getMovelist(onBoard,myHand);
	int i;
	for(i=0;i<movelist.num;i++){
			printMove(movelist.list[i]);
	}
	return 0;
}
