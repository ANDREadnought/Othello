#include "heuristic.h"

/**
 *@brief The heuristic combination that researchers at the University of
 *of Washington determined to be best
 *@param Board* board -- a pointer to the board representation we wish to evaluate
 **/
double uWashingtonHeuristic(Board* board) {
  int whitetiles = 0, blacktiles = 0, whitefrontier = 0, blackfrontier = 0;

  int X[] = {-1, -1, 0, 1, 1, 1, 0, -1};
  int Y[] = {0, 1, 1, 1, 0, -1, -1, -1};
	
  // Mobility
  double mobscore;
  whitetiles = board->numValidMoves(WHITE);
  blacktiles = board->numValidMoves(BLACK);
  if(whitetiles > blacktiles)
    mobscore = (100.0 * whitetiles)/(whitetiles + blacktiles);
  else if(whitetiles < blacktiles)
    mobscore = -(100.0 * blacktiles)/(whitetiles + blacktiles);
  else mobscore = 0;

  // Piece difference, frontier disks and disk square
  double diskstabilityscore;
  for(int i = 0; i < BOARDSIZE; i++)
    for(int j = 0; j < BOARDSIZE; j++)  
      {
	if(board->get(WHITE, i, j))  
	  {
	    diskstabilityscore += WEIGHTS[i][j];
	    whitetiles++;
	  } 
	else if(board->get(BLACK, i, j))  
	  {
	    diskstabilityscore -= WEIGHTS[i][j];
	    blacktiles++;
	  }
	if(board->occupied(i, j))  
	  {
	    for(int k = 0; k < 8; k++)  {
	      int x = i + X[k]; int y = j + Y[k];
	      if(x >= 0 && x < 8 && y >= 0 && y < 8 && !board->occupied(i, j)) 
		{
		  if(board->get(WHITE, i, j)) whitefrontier++;
		  else blackfrontier++;
		  break;
		}
	    }
	  }
      }
  double parityscore;
  if(whitetiles > blacktiles)
    parityscore = (100.0 * whitetiles)/(whitetiles + blacktiles);
  else if(whitetiles < blacktiles)
    parityscore = -(100.0 * blacktiles)/(whitetiles + blacktiles);
  else parityscore = 0;
	
  double frontierscore;
  if(whitefrontier > blackfrontier)
    frontierscore = -(100.0 * whitefrontier)/(whitefrontier + blackfrontier);
  else if(whitefrontier < blackfrontier)
    frontierscore = (100.0 * blackfrontier)/(whitefrontier + blackfrontier);
  else frontierscore = 0;
	

  // "Corner Closeness"
  // If corner isn't occupied, and the tile next to it is occupied
  double closenessscore;
  whitetiles = 0; blacktiles = 0;
  if(!board->occupied(0,0))  
    {
      if(board->get(WHITE, 0, 1)) whitetiles++;
      else if(board->get(BLACK, 0, 1)) blacktiles++;
      if(board->get(WHITE, 1, 1)) whitetiles++;
      else if(board->get(BLACK, 1, 1)) blacktiles++;
      if(board->get(WHITE, 1, 0)) whitetiles++;
      else if(board->get(BLACK, 1, 0)) blacktiles++;
    }
  if(!board->occupied(0, 7))
    {
      if(board->get(WHITE, 0, 6)) whitetiles++;
      else if(board->get(BLACK, 0, 6)) blacktiles++;
      if(board->get(WHITE, 1, 6)) whitetiles++;
      else if(board->get(BLACK, 1, 6)) blacktiles++;
      if(board->get(WHITE, 1, 7)) whitetiles++;
      else if(board->get(BLACK, 1, 7)) blacktiles++;
    }
  if(!board->occupied(7,0))
    {
      if(board->get(WHITE, 7, 1)) whitetiles++;
      else if(board->get(BLACK, 7, 1)) blacktiles++;
      if(board->get(WHITE, 6, 1)) whitetiles++;
      else if(board->get(BLACK, 6, 1)) blacktiles++;
      if(board->get(WHITE, 6, 0)) whitetiles++;
      else if(board->get(BLACK, 6, 0)) blacktiles++;
    }
  if(!board->occupied(7,7)) 
    {
      if(board->get(WHITE, 6, 7)) whitetiles++;
      else if(board->get(BLACK, 6, 7)) blacktiles++;
      if(board->get(WHITE, 6, 6)) whitetiles++;
      else if(board->get(BLACK, 6, 6)) blacktiles++;
      if(board->get(WHITE, 7, 6)) whitetiles++;
      else if(board->get(BLACK, 7, 6)) blacktiles++;
    }
  closenessscore = -12.5 * (whitetiles - blacktiles);

  // Holding Corners?
  double cornerscore;
  whitetiles = 0; blacktiles = 0;
  if(board->get(WHITE, 0, 0)) whitetiles++;
  else if(board->get(BLACK, 0, 0)) blacktiles++;
  if(board->get(WHITE, 0, 7)) whitetiles++;
  else if(board->get(BLACK, 0, 7)) blacktiles++;
  if(board->get(WHITE, 7, 0)) whitetiles++;
  else if(board->get(BLACK, 7, 0)) blacktiles++;
  if(board->get(WHITE, 7, 7)) whitetiles++;
  else if(board->get(BLACK, 7, 7)) blacktiles++;
  cornerscore = 25 * (whitetiles - blacktiles);
	
  // final weighted score
  double score = (10 * parityscore) + (801.724 * cornerscore) + 
    (382.026 * closenessscore) + (78.922 * mobscore) + 
    (74.396 * frontierscore) + (10 * diskstabilityscore);
  return -1*score;
}
