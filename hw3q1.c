/*-------------------------------------------------------------------------
    Include files:
-------------------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

/*-------------------------------------------------------------------------
    Constants and definitions:
-------------------------------------------------------------------------*/
#define N 11
#define EMPTY '_'
#define COL 2
#define EVEN 2
#define PLAYER1 'X'
#define PLAYER2 'O'
#define PLAYERS 2
#define P_1 1
#define P_2 2
#define ERROR 0
#define SUCCESS 1

/*-------------------------------------------------------------------------
    Function declaration
-------------------------------------------------------------------------*/

/*---------------------
 * PRINTING FUNCTIONS *
 *--------------------*/
void print_welcome();
void print_enter_board_size();
void print_board(char board[N][N], int n);
void print_player_turn(int player_index);
void print_error();
void print_winner(int player_index);
void print_tie();


/*-----------------
 * GAME FUNCTIONS *
 -----------------*/
int read_board_size();
void reset_board(char board[N][N], int board_size);
void play_game(char board[N][N], int size, int player, int moves[N*N][COL]);
void read_next_move(char board[N][N], int size, int moves[N*N][COL], int player, int move);
void discard_next_index(int row_index, int size);
int check_if_undo_is_valid(int next_move, char board[N][N], int size, int moves[N*N][COL], int player);
int check_if_index_is_valid(int next_move, int size);
void check_if_board_is_empty_in_next_move(char board[N][N], int moves[N*N][COL],
                                          int index_row, int index_col, int size, int player);
void undo_board(char board[N][N], int undo, int player, int moves[N*N][COL], int size);
int check_if_game_ended(char board[N][N], int size);
int check_who_won(int col_cnt, int row_cnt, int diag1_cnt, int diag2_cnt, int size, char col, char row,
                char diagonal1, char diagonal2);
int winner_is(char symbol);
int count_number_of_symbols(int row, int col, char board[N][N], char symbol);
void check_if_there_is_a_tie(char board[N][N], int size);
void submit_move_to_board(char board[N][N], int moves[N*N][COL], int player);


//10 lines
int main(){

    print_welcome();
    print_enter_board_size();
    int board_size = 0;
    board_size = read_board_size();

    char board[N][N] = {{0}};
    reset_board(board, board_size);

    int player = 0;
    int moves[N*N][COL] = {{0}};
    play_game(board, board_size, player, moves);

    return 0;
}

/*-------------------------------------------------------------------------
    Implementation
-------------------------------------------------------------------------*/
//print welcome message
//1 lines
void print_welcome()
{
    printf("*** Welcome to AVOIDANCE TIC-TAC-TOE game ***\n\n");
}

//print message to enter board size
//1 lines
void print_enter_board_size()
{
    printf("Please enter board size (1 to %d):\n", N);
}

//print the board
//7 lines
void print_board(char board[N][N], int n)
{
    printf("\nCurrent board:\n");
    for (int i = 0; i < n; i++)
    {
        printf("|");
        for (int j = 0; j < n; j++)
        {
            printf("%c|", board[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

//print a request for player with index "player_index" to move
//1 lines
void print_player_turn(int player_index)
{
    printf("\nPlayer ** %d **, enter next move:\n", player_index);
}

//print error message
//1 lines
void print_error()
{
    printf("Illegal move!!!, please try again:\n");
}

//print the winner
//1 lines
void print_winner(int player_index)
{
    printf("Player %d Wins! Hooray!\n", player_index);
}

//print message to announce there is a tie (no one wins)
//1 lines
void print_tie()
{
    printf("It's a tie!\n");
}

//read board size input
//3 lines
int read_board_size(){
    int size = 0;
    scanf("%d", &size);

    return size;
}

//filling the board with the empty symbol
//3 lines
void reset_board(char board[N][N], int size){
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            board[i][j] = EMPTY;
        }
    }
}

//printing next player and receiving next move, ends game when there is a winner
//or a tie
//12 lines
void play_game(char board[N][N], int size, int player, int moves[N*N][COL]){
    int winner = 0;

    while(!winner){

        print_board(board, size);

        check_if_there_is_a_tie(board, size);

        print_player_turn(player % PLAYERS + 1);
        read_next_move(board, size, moves, player, 0);
        submit_move_to_board(board, moves, player);

        winner = check_if_game_ended(board, size);
        ++player;
    }
    print_board(board, size);
    print_winner(winner);
    exit(0);
}

//assign the move to the board
//4 lines
void submit_move_to_board(char board[N][N], int moves[N*N][COL], int player){
    if (player % PLAYERS == 0) {
        board[moves[player][0] - 1][moves[player][1] - 1] = PLAYER1;
    } else {
        board[moves[player][0] - 1][moves[player][1] - 1] = PLAYER2;
    }
}

//read row and column for next move and check if valid
//13 lines
void read_next_move(char board[N][N], int size, int moves[N*N][COL], int player, int move){

    while (move != SUCCESS) {
        scanf(" %d", &moves[player][0]);

        if (moves[player][0] < 0){
            move = check_if_undo_is_valid(moves[player][0], board, size, moves, player);
        } else {
            move = check_if_index_is_valid(moves[player][0], size);
        }

        if (move == SUCCESS) {
            scanf(" %d", &moves[player][1]);
            move = check_if_index_is_valid(moves[player][1], size);
        }
        if (move == ERROR) {
            discard_next_index(moves[player][0], size);
            print_error();
        }
    }
    check_if_board_is_empty_in_next_move(board, moves, moves[player][0] - 1, moves[player][1] - 1, size, player);
}

//clears scanf buffer
//3 lines
void discard_next_index(int row_index, int size){
    int trash = 0;

    if(row_index > size) {
        scanf(" %d", &trash);
    }
}

//if undo move is valid resets the board according to size of undo
//if it's not valid player needs to resubmit
// 10 lines
int check_if_undo_is_valid(int next_move, char board[N][N], int size, int moves[N*N][COL], int player){
    if(next_move % EVEN){
        int undo = (-next_move);

        if (undo > player){
            moves[player][0] += undo;
            return ERROR;
        } else {
            undo_board(board, undo, player, moves, size);
        }
    }
    else if (!(next_move % EVEN)){
        return ERROR;
    }
    return SUCCESS;
}

//if move isn't valid asks player to resubmit
//3 lines
int check_if_index_is_valid(int next_move, int size){
    if (next_move > size){
        return ERROR;
    }
    return SUCCESS;
}

//checks that the board is empty where player wants to make his next move
//3 lines
void check_if_board_is_empty_in_next_move(char board[N][N], int moves[N*N][COL],
                                    int index_row, int index_col, int size, int player){
    if (board[index_row][index_col] != EMPTY || index_row < 0 || index_col < 0) {
        print_error();
        read_next_move(board, size, moves, player, 0);
    }
}

//erases last steps according to undo
//7 lines
void undo_board(char board[N][N], int undo, int player, int moves[N*N][COL],
                int size){

    for (int row = player - undo; row < player; ++row) {
            board[moves[row][0] - 1][moves[row][1] - 1] = EMPTY;
    }
    for (int i = player - undo; i < player; ++i) {
        for (int j = 0; j < COL ; ++j) {
            moves[i][j] = 0;
        }
    }
    player -= undo;
    play_game(board, size, player, moves);
}

//check row, column, diagonal for looser
//13 lines
int check_if_game_ended(char board[N][N], int size){
    int diag1_cnt = 0, diag2_cnt = 0, row_cnt = 0, col_cnt = 0;
    char diagonal1 = board[0][0], diagonal2 = board[0][size - 1],row =0,col=0;

    for (int i = 0; i < size; ++i) {

        row = board[i][0], col = board[0][i];

        diag1_cnt += count_number_of_symbols(i, i, board, diagonal1);
        diag2_cnt += count_number_of_symbols(i, size - 1 - i, board, diagonal2);

        for (int j = 0; j < size; ++j) {

            row_cnt += count_number_of_symbols(i, j, board, row);
            col_cnt += count_number_of_symbols(j, i, board, col);
        }

        if(row_cnt == size || col_cnt == size || diag1_cnt == size || diag2_cnt == size) {
            break;
        }
        col_cnt = 0; row_cnt = 0;
    }
    return check_who_won(col_cnt, row_cnt, diag1_cnt, diag2_cnt, size, col, row, diagonal1, diagonal2);
}

//if the symbol in the current place matches the symbol in the beginning of the
//row, column or diagonal adds 1
//3 lines
int count_number_of_symbols(int row, int col, char board[N][N], char symbol){

    if (board[row][col] == symbol && symbol != EMPTY) {
        return 1;
    }
    return 0;
}

//if the counter of row/column/diagonal equals to size returns the winner
//10 lines
int check_who_won(int col_cnt, int row_cnt, int diag1_cnt, int diag2_cnt,
        int size, char col, char row, char diagonal1, char diagonal2){

    int winner = 0;

    if (col_cnt == size) {
        winner = winner_is(col);
    }
    if (row_cnt == size) {
       winner = winner_is(row);
    }
    if (diag1_cnt == size) {
        winner = winner_is(diagonal1);
    }
    if (diag2_cnt == size) {
        winner = winner_is(diagonal2);
    }
    return winner;
}

//returns winner
//6 lines
int winner_is(char symbol){
    int winner;

    if(symbol == PLAYER1){
        winner = P_2;
    } else {
        winner = P_1;
    }

    return winner;
}

//if there is a tie game ends
//11 lines
void check_if_there_is_a_tie(char board[N][N], int size){
    bool exit_loop = false;

    for (int i = 0; i < size ; ++i) {
        for (int j = 0; j < size; ++j) {
            if (board[i][j] == EMPTY){
                exit_loop = true;
                break;
            }
        }
        if (exit_loop == true) {
            break;
        }
    }
    if (exit_loop == false) {
        print_tie();
        exit(0);
    }
}