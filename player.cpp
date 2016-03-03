#include "player.h"

/*
 * Constructor for the player; initialize everything here. The side your AI is
 * on (BLACK or WHITE) is passed in as "side". The constructor must finish 
 * within 30 seconds.
 */
Player::Player(Side side) {
    // Will be set to true in test_minimax.cpp.
    testingMinimax = false;
    this->trans = new Table(transfile);
    this->openings = new Table(openingfile);
    this->board = new Board();
    this->color = side;
    if(side == WHITE) this->oppcolor = BLACK;
    else this->oppcolor = WHITE;
    /* 
     * TODO: Do any initialization you need to do here (setting up the board,
     * precalculating things, etc.) However, remember that you will only have
     * 30 seconds.
     */
}

/*
 * Destructor for the player.
 */
Player::~Player() {
  delete board;
  if(this->trans) delete trans;
  if(this->openings) delete openings;
}

/*
 * Compute the next move given the opponent's last move. Your AI is
 * expected to keep track of the board on its own. If this is the first move,
 * or if the opponent passed on the last move, then opponentsMove will be NULL.
 *
 * msLeft represents the time your AI has left for the total game, in
 * milliseconds. doMove() must take no longer than msLeft, or your AI will
 * be disqualified! An msLeft value of -1 indicates no time limit.
 *
 * The move returned must be legal; if there are no valid moves for your side,
 * return NULL.
 */
Move *Player::doMove(Move *opponentsMove, int msLeft) {
    /* 
     * TODO: Implement how moves your AI should play here. You should first
     * process the opponent's opponents move before calculating your own move
     */ 
  this->board->doMove(opponentsMove, this->oppcolor);
  std::vector<Move*> *moves;
  Move* todo;
  moves = getMoves();
  todo = chooseMove(moves);
  this->board->doMove(todo, this->color);
  delete moves;
  return todo;
}

std::vector<Move*>* Player::getMoves()
{
  std::vector<Move*>* ret = new std::vector<Move*>;
  for(int i = 0; i < BOARDSIZE; i++)
    {
      for(int j = 0; j < BOARDSIZE; j++)
	{
	  Move* test = new Move(i, j);
	  if(this->board->checkMove(test, this->color))
	    {
	      ret->push_back(test);
	    } 
	  else delete test;
	}
    }
  // for(int i = 0; i < ret->size(); i++)
  //   {
  //     Move* test2 = (*ret)[i];
  //     std::cerr << test2->getX() << " " << test2->getY() << std::endl;
  //   }
  // std::cerr << std::endl;
  return ret;
}

Move* Player::chooseMove(std::vector<Move*>* moves)
{
  if(moves->size() < 1) return nullptr;
  double bestheur;
  if(this->color == BLACK)
    {
      bestheur = -infinity;
    }
  else bestheur = infinity;
  Move* winner;
  
  //Make much better heuristic here.
  //pick first move found
  //winner = (*moves)[0];
  //maximize number of pieces on next turn.
  for(unsigned int i = 0; i < moves->size(); i++)
    {
      double heur;
      Board* testboard = this->board->copy();
      testboard->doMove((*moves)[i], this->color);
      //heur = heuristic(testboard);
      if(this->color == WHITE) heur = uWashingtonHeuristic(testboard, WHITE, BLACK);
      else heur = uWashingtonHeuristic(testboard, BLACK, WHITE); 
      if(this->color == BLACK and heur > bestheur)
	{
	  bestheur = heur;
	  winner = (*moves)[i];
	}
      else if(this->color == WHITE and heur < bestheur) 
	{
	  bestheur = heur;
	  winner = (*moves)[i];
	}
    }
  std::cerr << bestheur << " (" << winner->getX() << "," << winner->getY() << ")" << std::endl;
  return winner;
}

double Player::heuristic(Board* board)
{
  return board->countBlack() - board->countWhite();
}

double uWashingtonHeuristic(Board* board, Side my_color, Side opp_color)  {
	int my_tiles = 0, opp_tiles = 0, my_front_tiles = 0, opp_front_tiles = 0, x, y;
	double p = 0, c = 0, l = 0, m = 0, f = 0, d = 0;

	int X1[] = {-1, -1, 0, 1, 1, 1, 0, -1};
	int Y1[] = {0, 1, 1, 1, 0, -1, -1, -1};

	// int V[8][8];
	// Tile weights
	// V[0] = {20, -3, 11, 8, 8, 11, -3, 20};
    	// V[1] = {-3, -7, -4, 1, 1, -4, -7, -3};
    	// V[2] = {11, -4, 2, 2, 2, 2, -4, 11};
    	// V[3] = {8, 1, 2, -3, -3, 2, 1, 8};
    	// V[4] = {8, 1, 2, -3, -3, 2, 1, 8};
    	// V[5] = {11, -4, 2, 2, 2, 2, -4, 11};
    	// V[6] = {-3, -7, -4, 1, 1, -4, -7, -3};
    	// V[7] = {20, -3, 11, 8, 8, 11, -3, 20};
	
	std::vector<std::vector<int>> V;
	std::vector<int> temp;
	temp.push_back(20); temp.push_back(-3); temp.push_back(11); temp.push_back(8);
	temp.push_back(8); temp.push_back(11); temp.push_back(-3); temp.push_back(20);
	V.push_back(temp);

	temp.clear();
	temp.push_back(-3); temp.push_back(-7); temp.push_back(-4); temp.push_back(1);
	temp.push_back(1); temp.push_back(-4); temp.push_back(-7); temp.push_back(-3);
	V.push_back(temp);
	
	temp.clear();
	temp.push_back(11); temp.push_back(-4); temp.push_back(2); temp.push_back(2);
	temp.push_back(2); temp.push_back(2); temp.push_back(-4); temp.push_back(11);
	V.push_back(temp);

	temp.clear();
	temp.push_back(8); temp.push_back(1); temp.push_back(2); temp.push_back(-3);
	temp.push_back(-3); temp.push_back(2); temp.push_back(1); temp.push_back(8);
	V.push_back(temp);
	
	temp.clear();
	temp.push_back(8); temp.push_back(1); temp.push_back(2); temp.push_back(-3);
	temp.push_back(-3); temp.push_back(2); temp.push_back(1); temp.push_back(8);
	V.push_back(temp);
	
	temp.clear();
	temp.push_back(11); temp.push_back(-4); temp.push_back(2); temp.push_back(2);
	temp.push_back(2); temp.push_back(2); temp.push_back(-4); temp.push_back(11);
	V.push_back(temp);

	temp.clear();
	temp.push_back(-3); temp.push_back(-7); temp.push_back(-4); temp.push_back(1);
	temp.push_back(1); temp.push_back(-4); temp.push_back(-7); temp.push_back(-3);
	V.push_back(temp);
	
	temp.clear();
	temp.push_back(20); temp.push_back(-3); temp.push_back(11); temp.push_back(8);
	temp.push_back(8); temp.push_back(11); temp.push_back(-3); temp.push_back(20);
	V.push_back(temp);

// Piece difference, frontier disks and disk squares
	for(int i = 0; i < 8; i++)
	  for(int j = 0; j < 8; j++)  
	    {
	      if(board->get(my_color, i, j))  
		{
		  d += V[i][j];
		  my_tiles++;
		} 
	      else if(board->get(opp_color, i, j))  
		{
		  d -= V[i][j];
		  opp_tiles++;
		}
	      if(board->occupied(i, j))  
		{
		  for(int k = 0; k < 8; k++)  {
		    x = i + X1[k]; y = j + Y1[k];
		    if(x >= 0 && x < 8 && y >= 0 && y < 8 && !board->occupied(i, j)) 
		      {
			if(board->get(my_color, i, j)) my_front_tiles++;
			else opp_front_tiles++;
			break;
		      }
		  }
		}
	    }
	if(my_tiles > opp_tiles)
	  p = (100.0 * my_tiles)/(my_tiles + opp_tiles);
	else if(my_tiles < opp_tiles)
	  p = -(100.0 * opp_tiles)/(my_tiles + opp_tiles);
	else p = 0;
	
	if(my_front_tiles > opp_front_tiles)
	  f = -(100.0 * my_front_tiles)/(my_front_tiles + opp_front_tiles);
	else if(my_front_tiles < opp_front_tiles)
	  f = (100.0 * opp_front_tiles)/(my_front_tiles + opp_front_tiles);
	else f = 0;
	
	// Corner occupancy
	my_tiles = opp_tiles = 0;
	if(board->get(my_color, 0, 0)) my_tiles++;
	else if(board->get(opp_color, 0, 0)) opp_tiles++;
	if(board->get(my_color, 0, 7)) my_tiles++;
	else if(board->get(opp_color, 0, 7)) opp_tiles++;
	if(board->get(my_color, 7, 0)) my_tiles++;
	else if(board->get(opp_color, 7, 0)) opp_tiles++;
	if(board->get(my_color, 7, 7)) my_tiles++;
	else if(board->get(opp_color, 7, 7)) opp_tiles++;
	c = 25 * (my_tiles - opp_tiles);
	
	// Corner closeness
	my_tiles = opp_tiles = 0;
	if(!board->occupied(0,0))  
	  {
	    if(board->get(my_color, 0, 1)) my_tiles++;
	    else if(board->get(opp_color, 0, 1)) opp_tiles++;
	    if(board->get(my_color, 1, 1)) my_tiles++;
	    else if(board->get(opp_color, 1, 1)) opp_tiles++;
	    if(board->get(my_color, 1, 0)) my_tiles++;
	    else if(board->get(opp_color, 1, 0)) opp_tiles++;
	  }
	if(!board->occupied(0, 7))
	  {
	    if(board->get(my_color, 0, 6)) my_tiles++;
	    else if(board->get(opp_color, 0, 6)) opp_tiles++;
	    if(board->get(my_color, 1, 6)) my_tiles++;
	    else if(board->get(opp_color, 1, 6)) opp_tiles++;
	    if(board->get(my_color, 1, 7)) my_tiles++;
	    else if(board->get(opp_color, 1, 7)) opp_tiles++;
	  }
	if(!board->occupied(7,0))
	  {
	    if(board->get(my_color, 7, 1)) my_tiles++;
	    else if(board->get(opp_color, 7, 1)) opp_tiles++;
	    if(board->get(my_color, 6, 1)) my_tiles++;
	    else if(board->get(opp_color, 6, 1)) opp_tiles++;
	    if(board->get(my_color, 6, 0)) my_tiles++;
	    else if(board->get(opp_color, 6, 0)) opp_tiles++;
	  }
	if(!board->occupied(7,7)) 
	  {
	    if(board->get(my_color, 6, 7)) my_tiles++;
	    else if(board->get(opp_color, 6, 7)) opp_tiles++;
	    if(board->get(my_color, 6, 6)) my_tiles++;
	    else if(board->get(opp_color, 6, 6)) opp_tiles++;
	    if(board->get(my_color, 7, 6)) my_tiles++;
	    else if(board->get(opp_color, 7, 6)) opp_tiles++;
	  }
	l = -12.5 * (my_tiles - opp_tiles);

// Mobility
	my_tiles = board->numValidMoves(my_color);
	opp_tiles = board->numValidMoves(opp_color);
	if(my_tiles > opp_tiles)
	  m = (100.0 * my_tiles)/(my_tiles + opp_tiles);
	else if(my_tiles < opp_tiles)
	  m = -(100.0 * opp_tiles)/(my_tiles + opp_tiles);
	else m = 0;
	
	// final weighted score
	double score = (10 * p) + (801.724 * c) + (382.026 * l) + (78.922 * m) + (74.396 * f) + (10 * d);
	return score;
}
