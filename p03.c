#include <stdio.h>
#include <string.h>

char animals[10];
int xPos[10];
int yPos[10];
int onBoard = 0;
char handEnemy[10];
char handMy[10];

void inputBoard(){
	int i,j;
	char buff[10];
	for(i=0;i<4;i++){
        fgets(buff,10,stdin);
        for(j=0;j<3;j++){
            if(buff[j] != 'o'){
                animals[onBoard] = buff[j];
                xPos[onBoard] = j;
                yPos[onBoard] = i;
                onBoard++;
            }
        }
    }
	fgets(handMy,10,stdin);
    fgets(handEnemy,10,stdin);
}

char getTile(int x, int y){
	if(!isInBoard(x,y))
		return '\0';
	int i;
	for(i=0;i<onBoard;i++){
		if(x==xPos[i] && y==yPos[i])
			return animals[i];
	}
	return 'o';
}

int isInBoard(int x, int y){
	if(x>2 || x<0 || y>3 || y<0)
		return 0;
	else
		return 1;
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

int testDirc(int x, int y, int dirc){
	x = moveX(x,dirc);
	y = moveY(y,dirc);
	char target = getTile(x,y);
	if(target == 'o' 
		|| target == 'l'
		|| target == 'g'
		|| target == 'c'
		|| target == 'e'
		|| target == 'h')
		return 1;
	else
		return 0;
}

void printMove(char animal, int prevX, int prevY, int dirc){
	if(dirc == -1)
		printf("%c %c %c%d\n", animal, 'P', 'A'+prevX, prevY+1);
	else{
		if(testDirc(prevX,prevY,dirc)){
			int x = moveX(prevX,dirc), y = moveY(prevY,dirc);
			printf("%c %c%d %c%d\n", animal, 'A'+prevX, prevY+1, 'A'+x, y+1);
		}else
			return;
	}
}

int main(){
	int i,G[2],Gnum = 0;
	inputBoard();
	for(i=0;i<onBoard;i++){
		if(animals[i]=='G'){
			G[Gnum] = i;
			Gnum++;
		}
	}
	for(i=0;i<strlen(handMy);i++){
		if(handMy[i]=='G'){
			G[Gnum] = -1;
			Gnum++;
		}
	}
	for(i=0;i<Gnum;i++){
		if(G[i]==-1){
			int a,b;
			for(a=0;a<3;a++){
				for(b=0;b<4;b++){
					if(getTile(a,b)=='o')
						printMove('G',a,b,-1);
				}
			}
		}else{
			printMove('G',xPos[G[i]],yPos[G[i]],2);
			printMove('G',xPos[G[i]],yPos[G[i]],4);
			printMove('G',xPos[G[i]],yPos[G[i]],6);
			printMove('G',xPos[G[i]],yPos[G[i]],8);
		}
	}
	return 0;
}
