/**
 * Andre Barrett
 * 12/4/2023
 * Tic Tac Toe game that allows a user to play against the computer or another user
 * The game is saved to a text file which allows it to be closed and loaded
*/

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>


typedef struct {
    char board[3][3];
    char current_player;
} TicTacToeBoard;

// Function Prototype to display the Tic Tac Toe board
void display_board(TicTacToeBoard* board);


// Function Prototype to initialize the Tic Tac Toe board with empty cells
void initialize_board(TicTacToeBoard* board);

// Function Prototype to get selected row and column from computer
void get_CPU_move(TicTacToeBoard* board, int* row, int* col);

// Function Prototype to make a move on the Tic Tac Toe board
int make_move(TicTacToeBoard* board, int row, int col);

// Function Prototype to check if a player has won
int check_win(TicTacToeBoard* board, char player);


// Function Prototype to check if the game is a draw
int check_draw(TicTacToeBoard* board);

// Function Prototype to save the game state to a file
void save_board_to_file(TicTacToeBoard* board, char* file_path);

// Function Prototype to load the game state from a file
void load_board_from_file(TicTacToeBoard* board, char* file_path);


int main() {
	//Initialize
    srand(time(NULL));
    char* file_path = "tic_tac_toe_board.txt";
    TicTacToeBoard board;
    

    char player1[50], player2[50];
    int is_singleplayer;

    // Singleplayer or Mulitplayer
    char choice;
    printf("Welcome to Tic Tac Toe\n");
    printf("Do you want to play singleplayer? (y/n): ");
    scanf(" %c", &choice);
    if (choice == 'y' || choice == 'Y') { is_singleplayer = 1; }
    else { is_singleplayer = 0; }

	//Player Name input
    printf("Enter Player 1 name: ");
    scanf("%s", player1);
    if ( is_singleplayer ) {
        strcpy(player2, "The Computer");
    }
    else {
        printf("Enter Player 2 name: ");
        scanf("%s", player2);
    }

	//Load Previous Game
    printf("Do you want to continue the previous game? (y/n): ");
    scanf(" %c", &choice);
    if (choice == 'y' || choice == 'Y') {
        load_board_from_file(&board, file_path);
        printf("Loaded the previous game board:\n");
    }
    else {
        // If player does not load previous game, initialize a new one
        initialize_board(&board);
        save_board_to_file(&board, file_path);
        printf("Created new board:\n");
    }

    int row, col;
    while (1) {
        // Your code here
		
		// 1. Display Board
		display_board(&board);

		// 2. Ask for move (row, col)
        printf("It's %s's turn\n", (board.current_player == 'X') ? player1 : player2);
        if (!is_singleplayer || board.current_player == 'X') {
            // Get move from player
            printf("Enter the row and column of your move: ");
            scanf("%d %d", &row, &col);
        }
        else {
            // Get move from computer
            get_CPU_move(&board, &row, &col);
        }

		// 3. Check if valid move using make_move
		// Provided: use similar method for the rest.
		int move_result = make_move(&board, row, col);
        if (move_result == 0) {
            printf("Cell is already occupied or invalid input. Try again.\n");
            continue;
        }

		// 4. Check for a win or draw.
        int player_won = check_win(&board, 'X');
        int opponent_won = check_win(&board, 'O');
        int is_draw = check_draw(&board);
		
		// 5. Construct if statements based on the returned values from
		//    check_win and check_draw
        if (player_won || opponent_won || is_draw) { display_board(&board); }
        if (player_won) { printf("%s is the winner!", player1); break; }  
        else if (opponent_won) { printf("%s is the winner!", player2); break; }
        else if (is_draw) { printf("It's a draw!"); break; }
        
        // 6. Save board after every move.
		save_board_to_file(&board, file_path);
    }


    return 0;
}

// Function to display the Tic Tac Toe board
void display_board(TicTacToeBoard* board) {
    // Your code here
    for(int i = 0; i < 3; i++) {
        printf("     |     |     \n");
        printf("  %c  |  %c  |  %c  \n", (board->board)[i][0], (board->board)[i][1], (board->board)[i][2]);
        if (i < 2) { printf("_____|_____|_____\n"); }
        else { printf("     |     |     \n"); }
    }
}

// Function to initialize the Tic Tac Toe board with empty cells
void initialize_board(TicTacToeBoard* board) {
    // Your code here
    // Fill entire board with spaces
    for(int row = 0; row < 3; row++) {
        for(int col = 0; col < 3; col++) {
            (board->board)[row][col] = ' ';
        }
    }

    // Set current player to X
    board->current_player = 'X';
}

// Function to get selected row and column from computer
void get_CPU_move(TicTacToeBoard* board, int* row, int* col) {
    // This is an array of all the possible moves the computer can make
    int possible_moves[9][2];

    // Populate possible_moves with all the spaces that are empty
    int idx = 0;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if ((board->board)[i][j] == ' ') {
                possible_moves[idx][0] = i;
                possible_moves[idx][1] = j;
                idx++;
            }
        }
    }
    if (idx == 0) { return; }

    // At this point, idx equals the total number of possible moves the computer can make
    // Use rand() to get a random move for possible_moves
    int move = rand() % idx;

    // Set the value of row and col based on the chosen move
    *row = possible_moves[move][0];
    *col = possible_moves[move][1];
}

// Function to make a move on the Tic Tac Toe board
int make_move(TicTacToeBoard* board, int row, int col) {
    // Your code here
    // If either row or col are out of array bounds, return 0 for invalid move
    if (!(row >= 0 && row <= 2) || !(col >= 0 && col <= 2)) { return 0; }

    // If selected cell is empty, add move to board and toggle current player
    if ((board->board)[row][col] == ' ') {
        (board->board)[row][col] = board->current_player;
        board->current_player = (board->current_player == 'X') ? 'O' : 'X';
        return 1;
    }
    // Else, return 0 for invalid move
    else { return 0; }
}

// Function to check if a player has won
int check_win(TicTacToeBoard* board, char player) {
    // Your code here
    // Check every row for a win
    for (int row = 0; row < 3; row++) {
        if(((board->board)[row][0] == player) && ((board->board)[row][1] == player) && ((board->board)[row][2] == player)) {
            return 1;
        }
    }

    // Check every column for a win
    for (int col = 0; col < 3; col++) {
        if(((board->board)[0][col] == player) && ((board->board)[1][col] == player) && ((board->board)[2][col] == player)) {
            return 1;
        }
    }

    // Check every diagonal for a win
    if(((board->board)[0][0] == player) && ((board->board)[1][1] == player) && ((board->board)[2][2] == player)) {
        return 1;
    }
    if(((board->board)[0][2] == player) && ((board->board)[1][1] == player) && ((board->board)[2][0] == player)) {
        return 1;
    }

    // Return 0 at this point since none of the previous checks passed
    return 0;
}

// Function to check if the game is a draw
int check_draw(TicTacToeBoard* board) {
    // Your code here
    // Check if any of the players won
    if (check_win(board, 'X') || check_win(board, 'O')) { return 0; }

    // Check each spot on the board for an empty space
    for (int row = 0; row < 3; row++) {
        for (int col = 0; col < 3; col++) {
            if ((board->board)[row][col] == ' ') { return 0; }
        }
    }

    // Return 1 at this point since there is grounds for a draw
    return 1;
}

// Function to save the game state to a file
void save_board_to_file(TicTacToeBoard* board, char* file_path) {
    // Your code here
    // Open the file in write mode
    FILE* board_file = fopen(file_path, "w");

    // Print current player as first line of file
    fprintf(board_file, "%c\n", board->current_player);

    // Print each row of board to each line of file
    for (int row = 0; row < 3; row++) {
        for (int col = 0; col < 3; col++) {
            // Empty spaces will be saved as dashes for visual clarity
            fprintf(board_file, "%c", ((board->board)[row][col] == ' ') ? '-' : (board->board)[row][col]);
            if (col < 2) { fprintf(board_file, " "); }
        }
        if (row < 2) { fprintf(board_file, "\n"); }
    }

    // Close file
    fclose(board_file);
}

// Function to load the game state from a file
void load_board_from_file(TicTacToeBoard* board, char* file_path) {
    // Your code here
    // Open the file
    FILE* board_file = fopen(file_path, "r");

    // Read the current player from the first line of the file
    fscanf(board_file, "%c\n", &(board->current_player));

    // Load the board
    for (int row = 0; row < 3; row++) {
        fscanf(board_file, "%c %c %c\n", (board->board)[row], (board->board)[row] + 1, (board->board)[row] + 2);
        for (int col = 0; col < 3; col++) {
            // Convert any dashes back to spaces
            if ((board->board)[row][col] == '-') { (board->board)[row][col] = ' '; }
        }
    }

    // Close the file
    fclose(board_file);
}
