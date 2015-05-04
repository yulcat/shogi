#include "main.h"

int main(){
	Group onBoard, myHand, enemyHand;
	inputBoard(&onBoard, &myHand, &enemyHand);
	Movelist movelist = getMovelist(onBoard,myHand);
	
	int i, maxScore=0, maxId;
	for(i=0;i<movelist.num;i++){
		printMove(movelist.list[i]);
		int score = rate(onBoard, movelist.list[i]);
		if(score>maxScore){
			maxId = i;
			maxScore = score;
		}
	}
	printMove(movelist.list[maxId]);
	
	return 0;
}
