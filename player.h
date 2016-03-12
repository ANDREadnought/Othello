#ifndef __PLAYER_H__
#define __PLAYER_H__

#include <iostream>
#include "common.h"
#include "board.h"
#include <string>
#include <vector>
#include "table.h"
#include "timer.h"

const double infinity = 987654321.32658;
const std::string openingfile = "tables/openingbook.txt";
const std::string transfile = "tables/transtable.txt";
const std::string closingfile = "tables/closingbook.txt";
const int transmem = 300000000;
const int closingmem = 100000000;
const int openingmem = 100000000;

class Player {
protected:
  Table *openings, *trans, *closings;
  Board* board;
  std::vector<Move*>* getMoves();
  Move* chooseMove(std::vector<Move*>* moves);
  double minimax(Board* board, Side s, int depth);
  double alphabeta(Board* board, Side s, int depth, double alpha, double beta);
  Side color, oppcolor;
  double heuristic(Board* board);
  void saveTables();
  Timer timer;
  bool timing;
  bool _solved;
  bool _saved;

public:
    Player(Side side);
    ~Player();

    Move *doMove(Move *opponentsMove, int msLeft);

    // Flag to tell if the player is running within the test_minimax context
    bool testingMinimax;
};

std::vector<Move*>* getMoves(Board* board, Side color);
double uWashingtonHeuristic(Board* board, Side my_color, Side opp_color);
double minimax(Board* board, Side s, int depth);
#endif
