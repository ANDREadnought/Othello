#ifndef __PLAYER_H__
#define __PLAYER_H__

#include <iostream>
#include "common.h"
#include "board.h"
#include <string>
#include <vector>
#include "table.h"

const double infinity = 987654321.32658;
const std::string openingfile = "openingbook.txt";
const std::string transfile = "transtable.txt";

class Player {
protected:
  Table *openings, *trans;
  Board* board;
  std::vector<Move*>* getMoves();
  Move* chooseMove(std::vector<Move*>* moves);
  Side color, oppcolor;
  double heuristic(Board* board);
public:
    Player(Side side);
    ~Player();

    Move *doMove(Move *opponentsMove, int msLeft);

    // Flag to tell if the player is running within the test_minimax context
    bool testingMinimax;
};

double uWashingtonHeuristic(Board* board, Side my_color, Side opp_color);

#endif
