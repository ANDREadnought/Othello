#include <cstdio>
#include "common.h"
#include "player.h"
#include "board.h"
#include <iostream>

// Use this file to test your minimax implementation (2-ply depth, with a
// heuristic of the difference in number of pieces).
int main(int argc, char *argv[]) {

    // Create board with example state. You do not necessarily need to use
    // this, but it's provided for convenience.
    char boardData[64] = {
        ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
        ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', 
        ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', 
        ' ', ' ', ' ', 'w', 'b', ' ', ' ', ' ', 
        ' ', ' ', ' ', 'b', 'w', ' ', ' ', ' ', 
        ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', 
        ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', 
        ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '
    };
    Board *board = new Board();
    board->setBoard(boardData);

    // Initialize player as the white player, and set testing_minimax flag.
    Player *player1 = new Player(BLACK);    
    Player *player2 = new Player(WHITE);
    Side turn = BLACK;

    Move *last = NULL;
    while (!board->isDone()){
      Move *m;
      if (turn == BLACK) {
	m = player1->doMove(last, -1);
      }
      else if (turn == WHITE) {
	m = player2->doMove(last, -1);
      }
      board->doMove(m, turn);
      if (turn == BLACK) {
	if (board->numValidMoves(WHITE) > 0){
	  turn = WHITE;
	}
      }
      else{
	if (board->numValidMoves(BLACK) > 0) {
	  turn = BLACK;
	}
      }
      delete last;
      last = m;
      board->printBoard();
    }
}
