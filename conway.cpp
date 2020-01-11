#include <chrono>
#include <ctime>
#include <curses.h>
#include <iostream>
#include <stdlib.h>
#include <thread>
using namespace std;

const int ROWS = 48;
const int COLUMNS = 160;
const char ALIVE = '*';
const char DEAD = ' ';


void initialize_curses() {
  // Initialize curses to control where to output to the terminal. No need to
  // modify this function, but make sure it is called in main at the beginning.
  initscr();
  cbreak();
  noecho();
  clear();
}

void print_exit_instructions() {
  // Prints instructions near the bottom of the screen for how to exit. No need
  // to modify this function.
  mvaddstr(ROWS + 1, 0, "Press ctrl+c to quit ");
  refresh();
}


void PrintRow(string row_to_print, int row) {
  mvaddstr(row, 0, row_to_print.c_str());
  refresh();
  this_thread::sleep_for(chrono::milliseconds(10));
}

void init_world(int board[ROWS][COLUMNS]){
    for(int row = 0; row < ROWS; row++){
        for(int col = 0; col < COLUMNS; col++){
            board[row][col] = 0;
        }   
    }
}

void init_glider(int board[ROWS][COLUMNS]){
    board[23][80] = 1;
    board[24][81] = 1;
        for(int col = 79; col < 82; col++){
        board[25][col] = 1;
    }
}

void init_pulsar(int board[ROWS][COLUMNS]){
    for(int row = 18; row < 31; row++){
        for(int col = 76; col < 85; col++){
            if(col < 79 || col > 81){
                if(row == 18 || row == 23 || row == 25 || row == 30){
                    board[row][col] = 1;
                }
            }
        }
    }
    
    for(int col = 74; col < 87; col++){
        for(int row = 20; row < 29; row++){
            if(row < 23 || row > 25){
                if(col == 74 || col == 79 || col == 81 || col == 86){
                    board[row][col] = 1;
                }
            }
        }
    }
   
}

void build_world(int board[ROWS][COLUMNS]){
    for(int row = 0; row < ROWS; row ++){
        string row_string;
        for(int col = 0; col < COLUMNS; col++){
            if(board[row][col] == 0){
                row_string += DEAD;
            } else {
                row_string += ALIVE;
            }
        }
        PrintRow(row_string,row);
    }
   
}
int diag_nbs(int board[ROWS][COLUMNS], int row, int col){
    int alive = 0;
    
    if(board[((row - 1) + 48) % 48][(col + 1) % 160] == 1){
        alive++;
    }
    if(board[((row - 1) + 48) % 48][((col - 1) + 160) % 160] == 1){
        alive++;
    }
    if(board[(row + 1) % 48][(col + 1) % 160] == 1){
        alive++;
    }
    if(board[(row + 1) % 48][((col - 1) + 160) % 160] == 1){
        alive++;
    }
    return alive;
}

int row_nbs(int board[ROWS][COLUMNS], int row, int col){
    int alive = 0;

    if(board[(row + 1) % 48][col] == 1){
        alive++;
    }
    
    if(board[((row - 1) + 48) % 48][col] == 1){
        alive++;
    }
    return alive;
    
}

int col_nbs(int board[ROWS][COLUMNS], int row, int col){
    int alive = 0;
     if(board[row][(col + 1) % 160] == 1){
       alive++;
        
    }
    
   if(board[row][((col - 1) + 160) % 160] == 1){
       alive++;
        
    }
    return alive;
}

void determin_cell_status(int write_board[ROWS][COLUMNS], int num_of_nbs, int row, int col){
    if(num_of_nbs < 2 ){
        write_board[row][col] = 0;
    }
    else if(num_of_nbs > 3){
        write_board[row][col] = 0;
    }
    else if(num_of_nbs == 3){
        write_board[row][col] = 1;
    }
}

void check_neigbors(int read_board[ROWS][COLUMNS], int write_board[ROWS][COLUMNS]){
    for(int row = 0; row < ROWS; row++){
        for(int col = 0; col < COLUMNS; col++){
            int nbs_sum = 0;
            nbs_sum += diag_nbs(read_board, row, col);
            nbs_sum += row_nbs(read_board,row,col);
            nbs_sum += col_nbs(read_board, row, col);
            determin_cell_status(write_board,nbs_sum,row,col);
        }
    }
}

void copy_world(int read_board[ROWS][COLUMNS], int write_board[ROWS][COLUMNS]){
    for(int row = 0; row < ROWS; row++){
        for(int col = 0; col < COLUMNS; col++){
            write_board[row][col] = read_board[row][col];
        }
    }
}

int main(int argc, char* argv[]) {
    if(argc != 2){
        cout << "Pick Pulsar or Glider" << endl;
        return 0;
    }
    string choice = argv[1];
    initialize_curses();
    print_exit_instructions();
    
    int current_gen[ROWS][COLUMNS];
    int next_gen[ROWS][COLUMNS];
    init_world(current_gen);
    if(choice == "Pulsar"){
        init_pulsar(current_gen);
    } else {
        init_glider(current_gen);
    }
    
    
    
    while (true) {
      build_world(current_gen);
      copy_world(current_gen, next_gen);
      check_neigbors(current_gen, next_gen);
      copy_world(next_gen, current_gen);
    }

    endwin();
    return 0;
}
