#ifndef __HEURISTIC_H__
#define __HEURISTIC_H__

#include "board.h"

const int WEIGHTS[8][8] = {{ 4,-3, 2, 2, 2, 2,-3, 4},
			   {-3,-4,-1,-1,-1,-1,-4,-3},
			   { 2,-1, 1, 0, 0, 1,-1, 2},
			   { 2,-1, 0, 1, 1, 0,-1, 2},
			   { 2,-1, 0, 1, 1, 0,-1, 2},
			   { 2,-1, 1, 0, 0, 1,-1, 2},
			   {-3,-4,-1,-1,-1,-1,-4,-3},
			   { 4,-3, 2, 2, 2, 2,-3, 4}};

double uWashingtonHeuristic(Board* board);
#endif
