#ifndef CONNECT4_FUNCTIONS
#define CONNECT4_FUNCTIONS

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

#define BOARD_SIZE_HORIZ 7
#define BOARD_SIZE_VERT 6

//global variables
int depth;

int print_welcome(void);

int random_move(int board[][BOARD_SIZE_VERT], int computer_num);

int player_move(int board[][BOARD_SIZE_VERT], int player_num);

bool check_win_or_tie(int board[][BOARD_SIZE_VERT], int last_move);

bool is_column_full(int board[][BOARD_SIZE_VERT], int m);

void update_board(int board[][BOARD_SIZE_VERT], int m, int player_num);

int check_winner(int board[][BOARD_SIZE_VERT], int last_move);

int best_move(int board[][BOARD_SIZE_VERT], int computer_num, int depth);

#endif

int scoring(int score[][BOARD_SIZE_VERT]);
void MinMax_Recur(int MinMax, int player, int depth, int minmaxBoard[][BOARD_SIZE_VERT], int *m, int *score);
int calcRow(int board[][BOARD_SIZE_VERT], int col, int player);

void display_board(int board[][BOARD_SIZE_VERT]){
	printf("\n");
	for (int i=0; i<BOARD_SIZE_VERT; i++) {
		for (int j=0; j<BOARD_SIZE_HORIZ; j++) {
			printf("+---");
		}
		printf("+\n");
		for (int k=0; k<BOARD_SIZE_HORIZ; k++) {
			printf("| ");
			if (board[k][i]==1) printf("X ");
			else if (board[k][i]==2) printf("O ");
			else printf("  ");
		}
		printf("|\n");
	}
	for (int i=0; i<BOARD_SIZE_HORIZ; i++) {
		printf("+---");
	}
	printf("+\n");
	for (int i=0; i<BOARD_SIZE_HORIZ; i++) {
		printf("  %d ",i+1);
	}
	printf(" \n\n");
}

int random_move(int board[][BOARD_SIZE_VERT], int computer_num){
	int m = rand() % (BOARD_SIZE_HORIZ - 1 + 1) + 1;
	while (is_column_full(board,m)) m = rand() % (BOARD_SIZE_HORIZ - 1 + 1) + 1;
	update_board(board, m, computer_num);
	return m;
}

int player_move(int board[][BOARD_SIZE_VERT], int player_num){
	int m;

	while (1) {
		printf("Please enter your move: ");
		if ((scanf("%d",&m)!=1) || (m<1) || (m>BOARD_SIZE_HORIZ)) {
			printf("Not a valid move. Enter a column number!\n");
			while (getchar()!='\n');
			continue;
		}
		if (is_column_full(board,m)){
			printf("This column is full. Try again!\n");
			while (getchar()!='\n');
			continue;
		}
		update_board(board,m,player_num);
		while (getchar()!='\n');
		break;
	}
	return m;
}

void update_board(int board[][BOARD_SIZE_VERT], int m, int player_num){
	for (int i=BOARD_SIZE_VERT-1; i>=0; i--){
		if (board[m-1][i]==0) {
			board[m-1][i]=player_num;
			break;
		}
	}
}

bool is_column_full(int board[][BOARD_SIZE_VERT], int m){
	for (int i=0; i<BOARD_SIZE_VERT; i++){
		if (board[m-1][i] == 0) return false;
	}
	return true;
}

bool check_win_or_tie(int board[][BOARD_SIZE_VERT], int last_move){
	int check;

	check = check_winner(board,last_move);
	if (check != 0) {
		printf("*****************************\n");
		printf("* Player %c won!!! Game over *\n", (check==1)?'X':'O');
		printf("*****************************\n");
		return true;
	}
	else if (check == 0) {
		for (int i=0; i<BOARD_SIZE_VERT; i++){
			for (int j=0; j<BOARD_SIZE_HORIZ; j++){
				if (board[j][i]==0) return false;
			}
		}
		printf("*****************************\n");
		printf("* Game is a tie!! No winner *\n");
		printf("*****************************\n");
		return true;
	}
}

int check_winner (int board[][BOARD_SIZE_VERT], int last_move) {
	int r; 
	for (int i=0; i<BOARD_SIZE_VERT; i++){
		if (board[last_move-1][i]!=0){
			r = i;
			break;
		}
	}
	//check horizontally
	int temp, count;
	for (int i=0; i<4+BOARD_SIZE_HORIZ-7; i++){
		count = 1;
		temp = i;
		for (int j=0; j<3; j++){
			if (board[temp][r]!=0 && board[temp][r]==board[temp+1][r]) count++; 
			temp++;
		}
		if (count == 4) return board[last_move-1][r];
	}
	//check vertically
	for (int i=0; i<3+BOARD_SIZE_VERT-6; i++){
		count = 1;
		temp = i;
		for (int j=0; j<3; j++){
			if (board[last_move-1][temp]!=0 && board[last_move-1][temp]==board[last_move-1][temp+1]) count++;
			temp++;
		}
		if (count == 4) return board[last_move-1][r];
	}
	//check diagonally
	int diX, diY; 
	int xLim, yLim; 
	int stp; 
	for (int x=0; x<BOARD_SIZE_HORIZ; x++){
		for (int y=0; y<BOARD_SIZE_VERT; y++){
			diX = 1; diY = -1;
			xLim = x+3*diX; yLim = y+3*diY;
			if (xLim<BOARD_SIZE_HORIZ && yLim<BOARD_SIZE_VERT && xLim>=0 && yLim>=0){
				stp = board[x][y];
				if (stp!=0 && stp==board[x+diX][y+diY] && stp==board[x+2*diX][y+2*diY] && stp==board[xLim][yLim])
					return stp;
			}
		}
	}
	for (int x=0; x<BOARD_SIZE_HORIZ; x++){
		for (int y=0; y<BOARD_SIZE_VERT; y++){
			diX = 1; diY = 1;
			xLim = x+3*diX; yLim = y+3*diY;
			if (xLim<BOARD_SIZE_HORIZ && yLim<BOARD_SIZE_VERT && xLim>=0 && yLim>=0){
				stp = board[x][y];
				if (stp!=0 && stp==board[x+diX][y+diY] && stp==board[x+2*diX][y+2*diY] && stp==board[xLim][yLim])
					return stp;
			}
		}
	}
	return 0;
}    

int best_move (int board[][BOARD_SIZE_VERT], int computer_num, int depth){

	int player_num = (computer_num==1)?2:1;
	int minmaxBoard[BOARD_SIZE_HORIZ][BOARD_SIZE_VERT]={{0}};
	int computer = 1, player = -1;

	// copy the board 
	for (int i=0; i<BOARD_SIZE_HORIZ; i++){
		for (int j=0; j<BOARD_SIZE_VERT; j++){
			if (board[i][j]==computer_num) minmaxBoard[i][j]=1; 
			if (board[i][j]==player_num) minmaxBoard[i][j]=-1; 
		}
	}

	int m, score; 
	MinMax_Recur(1,computer,depth,minmaxBoard,&m,&score); 
	update_board(board,m+1,computer_num);
	return m+1;
}

int calcRow(int board[][BOARD_SIZE_VERT], int col, int player){
	int y;
	for (y=BOARD_SIZE_VERT-1; y>=0; y--){
		if (board[col][y] == 0) {
			board[col][y] = player;
			return y;
		}
	}
	return BOARD_SIZE_VERT;
}

int Scoring(int scoring[][BOARD_SIZE_VERT]){
	int count[9] = {0};
	int x,y;

	//score of horizontal alignment
	for(y=0; y<BOARD_SIZE_VERT; y++){
		int score = scoring[0][y] + scoring[1][y] + scoring[2][y];
		for(x=3; x<BOARD_SIZE_HORIZ; x++){
			score+=scoring[x][y];
			count[score+4]++;
			score-=scoring[x-3][y];
		}
	}
	//score of vertical alignment
	for(x=0; x<BOARD_SIZE_HORIZ; x++){
		int score = scoring[x][0] + scoring[x][1] + scoring[x][2];
		for(y=3; y<BOARD_SIZE_VERT; y++){
			score+=scoring[x][y];
			count[score+4]++;
			score-=scoring[x][y-3];
		}
	}
	//calculate the scores of diagonal alignment from bottom left to top right
	for(y=0; y<BOARD_SIZE_VERT-3; y++){
		for(x=0; x<BOARD_SIZE_HORIZ-3; x++){
			int score=0, diX=0;
			for(diX=0; diX<4; diX++){
				score+=scoring[x+diX][y+diX];
			}
			count[score+4]++;
		}
	}
	//calculate the scores of diagonal alignment from top left to bottom right
	for(y=3; y<BOARD_SIZE_VERT; y++){
		for(x=0; x<BOARD_SIZE_HORIZ-3; x++){
			int score=0, diX=0;
			for(diX=0; diX<4; diX++){
				score+=scoring[x+diX][y-diX];
			}
			count[score+4]++;
		}
	}

	if (count[0]!=0)
		return -1000000;
	else if (count[8] != 0)
		return 1000000;
	else
		return -count[1]*5-count[2]*2-count[3]+count[7]*5+count[6]*2+count[5];
}

void MinMax_Recur(int MinMax, int player, int depth, int minmaxBoard[][BOARD_SIZE_VERT], int *m, int *score){
	int best_score = MinMax?-10000000:10000000;
	int best_move = -1, y;
	for (y = 0; y<BOARD_SIZE_HORIZ; y++){
		if (minmaxBoard[y][0]!=0) continue; 
		
		int r = calcRow(minmaxBoard, y, player);
		if (r == BOARD_SIZE_VERT) continue; 
		
		int s = Scoring(minmaxBoard);

		if (s == (MinMax?1000000:-1000000)){
			best_score = s;
			best_move = y;
			minmaxBoard[y][r] = 0;
			break;
		}
		
		int recurMove, recurScore;
		if (depth>1)
			MinMax_Recur(!MinMax, player==1?-1:1, depth-1, minmaxBoard, &recurMove, &recurScore); 
		else {
			recurMove = -1;
			recurScore = s;
		}

		minmaxBoard[y][r] = 0;

		if (recurScore == 1000000 || recurScore == -1000000)
			recurScore -= depth*(int)player;
		if (MinMax) {
			if (recurScore >= best_score){
				best_score = recurScore;
				best_move = y;
			}
		}
		else {
			if (recurScore <= best_score){
				best_score = recurScore;
				best_move = y;
			}
		}
	}

	*m = best_move;
	*score = best_score;
}
/*******************MULTIPLAYER**************************/


int main()
{
   int board[BOARD_SIZE_HORIZ][BOARD_SIZE_VERT] = { {0} };
   int mode_choice;
    int player_num, computer_num;
    int last_move; int choice;
	//char C;
   printf("*** Welcome to the Connect Four game!!! ***\n");
   printf("Choose Mode: \n");
   printf("[1] SinglePLayer\n");
   printf("[2] MultiPLayer\n");
   scanf("%d", &mode_choice);
   

   if (mode_choice == 1){
    //Single Player
	/*printf("Would you like to make the first move: \n");
	printf("[1] Yes\n");
	printf("[2] No\n");
	scanf("%d", &C);
	if (C == 2){
		player_num = 2;
		computer_num = 1;

	}
	else if (C == 1){
		player_num = 1;
		computer_num = 2;
	}
	else{
		printf("Invalid Argument!\n");
	}*/
    //choosing difficulty
    printf("Select Difficulty: \n");
    printf("[1] Easy\n");
    printf("[2] Medium\n");
    printf("[3] Hard ");
    scanf("%d", &choice);
	if (choice == 1){
		depth = 2;

	}
	else if (choice == 2){
		depth = 5;

	}
	else if (choice == 3){
		depth = 9;

	}
	else{
		printf("Invalid argument\n");
		
	}
	//flip a coin to determine who goes first
	srand(time(NULL));
	int coin = rand() % 2;
	if (coin == 0){
		player_num = 1;
		computer_num = 2;
	}
	else{
		player_num = 2;
		computer_num = 1;
	}

    if (player_num == 1)
    {
        display_board(board);
        last_move = player_move(board,player_num);
        display_board(board);
    }
    
    while (1)
    {
        last_move = best_move(board,computer_num,depth);
        printf("Computer moved in column: %d\n", last_move);
        display_board(board);

        if (check_win_or_tie(board,last_move)) return 0;

        last_move = player_move(board,player_num);
        display_board(board);

        if (check_win_or_tie(board,last_move)) return 0;

    } 
    } //End Single Player

    //Multiplayer
    else if (mode_choice == 2){
	//computer_num is actually used as second player num
	//flip a coin to determine who goes first
	srand(time(NULL));
	int coin = rand() % 2;
	if (coin == 0){
		player_num = 1;
		computer_num = 2;
	}
	else{
		player_num = 2;
		computer_num = 1;
	}

    if (player_num == 1)
    {
        display_board(board);
        last_move = player_move(board,player_num);
        display_board(board);
    }
	while (1)
	{
		last_move = player_move(board,player_num);
		display_board(board);

		if (check_win_or_tie(board,last_move)) return 0;

		last_move = player_move(board,computer_num);
		display_board(board);

		if (check_win_or_tie(board,last_move)) return 0;

    }
	}
    //End MultiPlayer
    else{
        printf("Choose a valid mode");
    }

   return 0;
   

}

