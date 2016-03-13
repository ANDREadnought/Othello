#ifndef __COMMON_H__
#define __COMMON_H__

#include <exception>
#include <vector>
#include <mutex> 
#include <thread>

class FileNotOpenError : public std::exception {};

enum Side { 
    WHITE, BLACK
};

const int BOARDSIZE = 8;

class Move {
   
public:
    int x, y;
    Move(int x, int y) {
        this->x = x;
        this->y = y;        
    }
    ~Move() {}

    Move* copy();
    
    int getX() { return x; }
    int getY() { return y; }

    void setX(int x) { this->x = x; }
    void setY(int y) { this->y = y; }
};

void cleanMoves(std::vector<Move*>* moves);

#endif
