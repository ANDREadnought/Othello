#ifndef __BOARD_H__
#define __BOARD_H__

#include <bitset>
#include <vector>
#include "common.h"

class Board {
   
private:
  std::bitset<64> black;
  std::bitset<64> taken;    
    void set(Side side, int x, int y);

 public:
    bool occupied(int x, int y);
    bool get(Side side, int x, int y);
    bool onBoard(int x, int y);
      
    Board();
    ~Board();
    Board *copy();
        
    bool isDone();
    bool hasMoves(Side side);
    bool checkMove(Move *m, Side side);
    int numValidMoves(Side side);
    void doMove(Move *m, Side side);
    int count(Side side);
    int countBlack();
    int countWhite();
    std::vector<Move*>* getMoves(Board* board, Side s);
    std::vector<Move*>* getMoves(Side color);
    std::string toString();

    void setBoard(char data[]);
};

#endif
