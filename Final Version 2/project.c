#ifndef CONNECT4_FUNCTIONS
#define CONNECT4_FUNCTIONS

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

#define column 7
#define row 6

//global variables
int depth;
int count_time_for_player = 0;
int count_time_for_computer = 0;


//function

//requires: current board of integers and integer that indicates it is the computer's turn
//effects: returns an integer that is the column chosen randomly by the computer to be used by the last_move variabel
int random_move(int board[][row], int computer_num);

//requires: current board of integers and integer that indicates it is the player's turn
//effects: returns an integer that is the column chosen by the player to be used by the last_move variabel
int player_move(int board[][row], int player_num);

//requires: current board of integers and integer that the chosen last move done by either the computer or player / player 1 or player 2
//effects: returns true if there is a winner or tie and false otherwise. Also, prints the winner depending on who did the last winning move indicated by the variable "last_move"
//		   Note that this is for multiplayer mode
bool check_win_or_tie(int board[][row], int last_move);

//requires: current board of integers and integer that the chosen last move done by either the computer or player / player 1 or player 2
//effects: returns true if there is a winner or a tie and false otherwise. Doesn't print the winner between the computer and the player
//		   Note: same functionality as check_win_or_tie but it is used for single player mode
bool check_win_or_tie_singleplayer(int board[][row], int last_move);

//requires: the current integer board and chosen column
//effects: returns true if the column is full and false otherwise
bool is_column_full(int board[][row], int m);

//requires: the current integer board - chosen column - integer relating to player number turn
//effects: the last cell in the chosen column is transforemd from 0 to 1 or 2 depending on player_num
void update_board(int board[][row], int m, int player_num);

int check_winner(int board[][row], int last_move);

int best_move(int board[][row], int computer_num, int depth);

#endif

int scoring(int score[][row]);

void MinMax_Recur(int MinMax, int player, int depth, int minmaxBoard[][row], int *m, int *score);

int calcRow(int board[][row], int col, int player);

void display_board(int board[][row]);

void display_board(int board[][row]){
	printf("\n");
	for (int i=0; i<row; i++) {
		for (int j=0; j<column; j++) {
			printf("+---");
		}
		printf("+\n");
		for (int k=0; k<column; k++) {
			printf("| ");
			if (board[k][i]==1) printf("1 ");
			else if (board[k][i]==2) printf("2 ");
			else printf("  ");
		}
		printf("|\n");
	}
	for (int i=0; i<column; i++) {
		printf("+---");
	}
	printf("+\n");
	for (int i=0; i<column; i++) {
		printf("  %d ",i+1);
	}
	printf(" \n\n");
}

int random_move(int board[][row], int computer_num){
	int m = rand() % (column) + 1;
	while (is_column_full(board,m)) m = rand() % (column) + 1;
	update_board(board, m, computer_num);
	return m;
}

int player_move(int board[][row], int player_num){
	int m;

	while (1) {
		printf("Please enter your move: ");
		if ((scanf("%d",&m)!=1) || (m<1) || (m>column)) {
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

void update_board(int board[][row], int m, int player_num){
	for (int i=row-1; i>=0; i--){
		if (board[m-1][i]==0) {
			board[m-1][i]=player_num;
			break;
		}
	}
}

bool is_column_full(int board[][row], int m){
	for (int i=0; i<row; i++){
		if (board[m-1][i] == 0) return false;
	}
	return true;
}

bool check_win_or_tie(int board[][row], int last_move){
	int check = check_winner(board,last_move);
	if (check != 0) {
		printf("*****************************\n");
		printf("* Player %c won!!! Game over*\n", (check==1)?'1':'2');
		printf("*****************************\n");
		return true;
	}
	else if (check == 0) {
		for (int i=0; i<row; i++){
			for (int j=0; j<column; j++){
				if (board[j][i]==0) return false;
			}
		}
		printf("*****************************\n");
		printf("* Game is a tie!! No winner *\n");
		printf("*****************************\n");
		return true;
	}
}

bool check_win_or_tie_singleplayer(int board[][row], int last_move){
	int check = check_winner(board,last_move);
	if (check != 0) {
		return true;
	}
	else if (check == 0) {
		for (int i=0; i<row; i++){
			for (int j=0; j<column; j++){
				if (board[j][i]==0) return false;
			}
		}
		return true;
	}
}

int check_winner (int board[][row], int last_move) {
	int r;
	for (int i=0; i<row; i++){
		if (board[last_move-1][i]!=0){
			r = i;
			break;
		}
	}
	//check horizontally
	int temp, count;
	for (int i=0; i<4+column-7; i++){
		count = 1;
		temp = i;
		for (int j=0; j<3; j++){
			if (board[temp][r]!=0 && board[temp][r]==board[temp+1][r]) count++;
			temp++;
		}
		if (count == 4) return board[last_move-1][r];
	}
	//check vertically
	for (int i=0; i<3+row-6; i++){
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
	for (int x=0; x<column; x++){
		for (int y=0; y<row; y++){
			diX = 1; diY = -1;
			xLim = x+3*diX; yLim = y+3*diY;
			if (xLim<column && yLim<row && xLim>=0 && yLim>=0){
				stp = board[x][y];
				if (stp!=0 && stp==board[x+diX][y+diY] && stp==board[x+2*diX][y+2*diY] && stp==board[xLim][yLim])
					return stp;
			}
		}
	}
	for (int x=0; x<column; x++){
		for (int y=0; y<row; y++){
			diX = 1; diY = 1;
			xLim = x+3*diX; yLim = y+3*diY;
			if (xLim<column && yLim<row && xLim>=0 && yLim>=0){
				stp = board[x][y];
				if (stp!=0 && stp==board[x+diX][y+diY] && stp==board[x+2*diX][y+2*diY] && stp==board[xLim][yLim])
					return stp;
			}
		}
	}
	return 0;
}

int best_move (int board[][row], int computer_num, int depth){

	int player_num = (computer_num==1)?2:1;
	int minmaxBoard[column][row]={{0}};
	int computer = 1, player = -1;

	// copy the board
	for (int i=0; i<column; i++){
		for (int j=0; j<row; j++){
			if (board[i][j]==computer_num) minmaxBoard[i][j]=1;
			if (board[i][j]==player_num) minmaxBoard[i][j]=-1;
		}
	}

	int m, score;
	MinMax_Recur(1,computer,depth,minmaxBoard,&m,&score);
	update_board(board,m+1,computer_num);
	return m+1;
}

int calcRow(int board[][row], int col, int player){
	int y;
	for (y=row-1; y>=0; y--){
		if (board[col][y] == 0) {
			board[col][y] = player;
			return y;
		}
	}
	return row;
}

int Scoring(int scoring[][row]){
	int count[9] = {0};
	int x,y;

	//score of horizontal alignment
	for(y=0; y<row; y++){
		int score = scoring[0][y] + scoring[1][y] + scoring[2][y];
		for(x=3; x<column; x++){
			score+=scoring[x][y];
			count[score+4]++;
			score-=scoring[x-3][y];
		}
	}
	//score of vertical alignment
	for(x=0; x<column; x++){
		int score = scoring[x][0] + scoring[x][1] + scoring[x][2];
		for(y=3; y<row; y++){
			score+=scoring[x][y];
			count[score+4]++;
			score-=scoring[x][y-3];
		}
	}
	//calculate the scores of diagonal alignment from bottom left to top right
	for(y=0; y<row-3; y++){
		for(x=0; x<column-3; x++){
			int score=0, diX=0;
			for(diX=0; diX<4; diX++){
				score+=scoring[x+diX][y+diX];
			}
			count[score+4]++;
		}
	}
	//calculate the scores of diagonal alignment from top left to bottom right
	for(y=3; y<row; y++){
		for(x=0; x<column-3; x++){
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

void MinMax_Recur(int MinMax, int player, int depth, int minmaxBoard[][row], int *m, int *score){
	int best_score = MinMax?-10000000:10000000;
	int best_move = -1, y;
	for (y = 0; y<column; y++){
		if (minmaxBoard[y][0]!=0) continue;

		int r = calcRow(minmaxBoard, y, player);
		if (r == row) continue;

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


int main() {
   int board[column][row] = { {0} };
    int player_num, computer_num;
    int last_move; int choice;
	//char C;
	printf("*** Welcome to the Connect Four game!!! ***\n");
   printf("Choose Mode: \n");
   printf("[1] SinglePLayer\n");
   printf("[2] MultiPLayer\n");
char mode_choice = 'c';  //Initializes c to a non-digit so isdigit(c) returns false
    int x = 0;

    /* the loop will repeat until the player enters a digit */
    while (!isdigit(mode_choice))
    {
        printf("Please enter your move: ");

        mode_choice = getchar();
        printf("%c",mode_choice);

        x = mode_choice - '0';
    while (getchar() != '\n') { } //clears stdin
        if (x <1 || x>2)
        {
            printf("\nNot a valid move. Enter mode number!\n");
            mode_choice = 'c';       //reset c to a non-digit to continue looping
                           //in case the user entered a digit that was invalid
        }

    }

   if (x == 1){
    //Single Player

    //choosing difficulty
    printf("Select Difficulty: \n");
	printf("[0] Beginner\n");
    printf("[1] Easy\n");
    printf("[2] Medium\n");
    printf("[3] Hard\n");
    char mode_choice1 = 'c';  //Initializes c to a non-digit so isdigit(c) returns false
    int y = 0;

    /* the loop will repeat until the player enters a digit */
    while (!isdigit(mode_choice1))
    {
        printf("Please enter the Level: ");

        mode_choice1 = getchar();
        printf("%c",mode_choice1);

        y = mode_choice1 - '0';
    while (getchar() != '\n') { } //clears stdin
        if (y <0 || y>3)
        {
            printf("\nNot a valid move. Enter a level number!\n");
            mode_choice1 = 'c';       //reset c to a non-digit to continue looping
                           //in case the user entered a digit that was invalid
        }

    }
    choice=y;


	//if choice is 0, random moves will be made by the computer
	if (choice == 1){
		depth = 2;

	}
	else if (choice == 2){
		depth = 5;

	}
	else if (choice == 3){
		depth = 8;

	}
	// choise is other than 0 1 2 3
	else if (choice > 3 || choice < 0){
		printf("Invalid argument\n");
	}
	//flip a coin to determine who goes first
	srand(time(NULL));
	int coin = rand() % 2;
	if (coin == 0){
		player_num = 1;
		computer_num = 2;
		printf("You go first\n");
	}
	else{
		player_num = 2;
		computer_num = 1;
		printf("Computer goes first\n");
	}

    if (player_num == 1)
    {
        display_board(board);
        last_move = player_move(board,player_num);
        display_board(board);
    }

    while (1)
    {
		// if the player the least diffuclty to train as a beginner - random movies will be done by the computer
		if (choice == 0){

			last_move = random_move(board,computer_num);

		}
		else{

			last_move = best_move(board,computer_num,depth);

		}

        printf("Computer moved in column: %d\n", last_move);
        display_board(board);

        if (check_win_or_tie_singleplayer(board,last_move)) {
			printf("*****************************\n");
			printf("*       Computer Won        *\n");
			printf("*****************************\n");
			return 0;
		}

        last_move = player_move(board,player_num);
        display_board(board);

        if (check_win_or_tie_singleplayer(board,last_move)) {
			printf("*****************************\n");
			printf("*           You Won         *\n");
			printf("*****************************\n");
			return 0;
		}

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
			printf("Player 1 goes first\n");
		}
		else{
			player_num = 2;
			computer_num = 1;
			printf("Player 2 goes first\n");
		}

		if (player_num == 1)
		{

			display_board(board);
			//clock_t start_for_player_one = clock();
			last_move = player_move(board,player_num);
			//clock_t start_for_player_one = clock();
			display_board(board);
		}
		while (1){

			display_board(board);
			//clock_t start_for_player_two = clock();
			last_move = player_move(board,player_num);
			//clock_t end_for_player_two = clock();
			display_board(board);

			if (check_win_or_tie(board,last_move)) return 0;

			//clock_t start_for_player_one = clock();
			last_move = player_move(board,computer_num);
			//clock_t end_for_player_one = clock();
			display_board(board);

			if (check_win_or_tie(board,last_move)) return 0;

		}
	}//End MultiPlayer

    else{
        printf("Choose a valid mode");
    }

   return 0;


}


