#include "main.h"

int main(){
	Group onBoard, myHand, enemyHand;
	inputBoard(&onBoard, &myHand, &enemyHand);
	// Get Animal pieces info
	Movelist movelist = getMovelist(onBoard,myHand);
	// List every possible moves
	
	int i, maxScore=0, maxId;
	for(i=0;i<movelist.num;i++){
		printMove(movelist.list[i]);
		int score = rate(onBoard, movelist.list[i]);
		// Score each moves
		printf("%d\n",score);
		if(score>maxScore){
			maxId = i;
			maxScore = score;
			// Get the move with highest score
		}
	}
	printMove(movelist.list[maxId]);
	// Print the move with highest score
	
	return 0;
}
