#include "main.h"

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
