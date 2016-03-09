#include "common.h"

void cleanMoves(std::vector<Move*>* moves)
{
  for(unsigned int i = moves->size(); i < moves->size(); ++i)
    {
      delete (*moves)[i];
    }
  delete moves;
}
