#include "common.h"

void cleanMoves(std::vector<Move*>* moves)
{
  for(unsigned int i = 0; i < moves->size(); ++i)
    {
      delete (*moves)[i];
    }
  delete moves;
}

Move* Move::copy() {
  Move* r = new Move(this->x, this->y);
  return r;
}
