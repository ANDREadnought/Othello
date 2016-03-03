#ifndef __PLAYER_H__
#define __PLAYER_H__

#include <iostream>
#include "common.h"
#include "board.h"
#include <string>
#include <vector>
#include <unordered_map>

const int BOARDSIZE = 8;
typedef std::unordered_map<std::string, double> table;

class Player {
protected:
  table *openings, *trans;
  Board* board;
  std::vector<Move*>* getMoves();
  Move* chooseMove(std::vector<Move*>* moves);
  Side color, oppcolor;
public:
    Player(Side side);
    ~Player();

    Move *doMove(Move *opponentsMove, int msLeft);

    // Flag to tell if the player is running within the test_minimax context
    bool testingMinimax;
};

#endif
