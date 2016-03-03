#include "player.h"

/*
 * Constructor for the player; initialize everything here. The side your AI is
 * on (BLACK or WHITE) is passed in as "side". The constructor must finish 
 * within 30 seconds.
 */
Player::Player(Side side) {
    // Will be set to true in test_minimax.cpp.
    testingMinimax = false;

    // Transposition Table
    this->trans = new Table(transfile);
    try // Are we going to be using this across games?
      {
	this->trans->load();
      }
    catch(FileNotOpenError())
      {
	std::cerr << "Transpositon not loaded" << std::endl;
      }
    
    // Opening Book
    this->openings = new Table(openingfile);
    try
      {
	this->openings->load();
      }
    catch(FileNotOpenError())
      {
	std::cerr << "Opening book not loaded" << std::endl;
      }

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
  if(this->trans)
    {
      try
	{
	  this->trans->save();
	}
      catch(FileNotOpenError())
	{
	  std::cerr << "Transposition Table not saved" << std::endl;
	}
      delete trans;
    }
  if(this->openings)
    {
      try
	{
	  this->openings->save();
	}
      catch(FileNotOpenError())
	{
	  std::cerr << "Transposition Table not saved" << std::endl;
	}
      delete openings;
    }
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

std::vector<Move*>* getMoves(Board *board, Side s){
  std::vector<Move*>* ret = new std::vector<Move*>;
  for(int i = 0; i < BOARDSIZE; i++)
  {
    for(int j = 0; j < BOARDSIZE; j++)
      {
	  Move* test = new Move(i, j);
	  if(board->checkMove(test, s))
	  {
	      ret->push_back(test);
	  } 
	  else delete test;
      }
  }
  return ret;
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
      /*if(this->color == WHITE) heur = uWashingtonHeuristic(testboard, WHITE, BLACK);
	else heur = uWashingtonHeuristic(testboard, BLACK, WHITE);*/
      if (this->color == WHITE)
	heur = minimax(testboard, BLACK, 1);
      else
	heur = minimax(testboard, WHITE, 1);
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
      //std::cerr << heur << " (" << winner->getX() << "," << winner->getY() << ")" << std::endl;
    }
  std::cerr << bestheur << " (" << winner->getX() << "," << winner->getY() << ")" << std::endl;
  return winner;
}

double Player::heuristic(Board* board)
{
  return board->countBlack() - board->countWhite();
}

double uWashingtonHeuristic(Board* board, Side my_color, Side opp_color)  {
	int mytiles = 0, opptiles = 0, myfrontier = 0, oppfrontier = 0;

	int X1[] = {-1, -1, 0, 1, 1, 1, 0, -1};
	int Y1[] = {0, 1, 1, 1, 0, -1, -1, -1};
	
	//Weights for Disk Stability
	std::vector<std::vector<int>> weights;
	std::vector<int> temp;
	temp.push_back(4); temp.push_back(-3); temp.push_back(2); temp.push_back(2);
	temp.push_back(2); temp.push_back(2); temp.push_back(-3); temp.push_back(4);
	weights.push_back(temp);

	temp.clear();
	temp.push_back(-3); temp.push_back(-4); temp.push_back(-1); temp.push_back(-1);
	temp.push_back(-1); temp.push_back(-1); temp.push_back(-4); temp.push_back(-3);
	weights.push_back(temp);
	
	temp.clear();
	temp.push_back(2); temp.push_back(-1); temp.push_back(1); temp.push_back(0);
	temp.push_back(0); temp.push_back(1); temp.push_back(-1); temp.push_back(2);
	weights.push_back(temp);

	temp.clear();
	temp.push_back(2); temp.push_back(-1); temp.push_back(0); temp.push_back(1);
	temp.push_back(1); temp.push_back(0); temp.push_back(-1); temp.push_back(2);
	weights.push_back(temp);

	temp.clear();
	temp.push_back(2); temp.push_back(-1); temp.push_back(0); temp.push_back(1);
	temp.push_back(1); temp.push_back(0); temp.push_back(-1); temp.push_back(2);
	weights.push_back(temp);

	temp.clear();
	temp.push_back(2); temp.push_back(-1); temp.push_back(1); temp.push_back(0);
	temp.push_back(0); temp.push_back(1); temp.push_back(-1); temp.push_back(2);
	weights.push_back(temp);

	temp.clear();
	temp.push_back(-3); temp.push_back(-4); temp.push_back(-1); temp.push_back(-1);
	temp.push_back(-1); temp.push_back(-1); temp.push_back(-4); temp.push_back(-3);
	weights.push_back(temp);

	temp.clear();
	temp.push_back(4); temp.push_back(-3); temp.push_back(2); temp.push_back(2);
	temp.push_back(2); temp.push_back(2); temp.push_back(-3); temp.push_back(4);
	weights.push_back(temp);


	// Mobility
	double mobscore;
	mytiles = board->numValidMoves(my_color);
	opptiles = board->numValidMoves(opp_color);
	if(mytiles > opptiles)
	  mobscore = (100.0 * mytiles)/(mytiles + opptiles);
	else if(mytiles < opptiles)
	  mobscore = -(100.0 * opptiles)/(mytiles + opptiles);
	else mobscore = 0;

	// Piece difference, frontier disks and disk square
	double diskstabilityscore;
	for(int i = 0; i < BOARDSIZE; i++)
	  for(int j = 0; j < BOARDSIZE; j++)  
	    {
	      if(board->get(my_color, i, j))  
		{
		  diskstabilityscore += weights[i][j];
		  mytiles++;
		} 
	      else if(board->get(opp_color, i, j))  
		{
		  diskstabilityscore -= weights[i][j];
		  opptiles++;
		}
	      if(board->occupied(i, j))  
		{
		  for(int k = 0; k < 8; k++)  {
		    int x = i + X1[k]; int y = j + Y1[k];
		    if(x >= 0 && x < 8 && y >= 0 && y < 8 && !board->occupied(i, j)) 
		      {
			if(board->get(my_color, i, j)) myfrontier++;
			else oppfrontier++;
			break;
		      }
		  }
		}
	    }
	double parityscore;
	if(mytiles > opptiles)
	  parityscore = (100.0 * mytiles)/(mytiles + opptiles);
	else if(mytiles < opptiles)
	  parityscore = -(100.0 * opptiles)/(mytiles + opptiles);
	else parityscore = 0;
	
	double frontierscore;
	if(myfrontier > oppfrontier)
	  frontierscore = -(100.0 * myfrontier)/(myfrontier + oppfrontier);
	else if(myfrontier < oppfrontier)
	  frontierscore = (100.0 * oppfrontier)/(myfrontier + oppfrontier);
	else frontierscore = 0;
	

	// "Corner Closeness"
	// If corner isn't occupied, and the tile next to it is occupied
	double closenessscore;
	mytiles = 0; opptiles = 0;
	if(!board->occupied(0,0))  
	  {
	    if(board->get(my_color, 0, 1)) mytiles++;
	    else if(board->get(opp_color, 0, 1)) opptiles++;
	    if(board->get(my_color, 1, 1)) mytiles++;
	    else if(board->get(opp_color, 1, 1)) opptiles++;
	    if(board->get(my_color, 1, 0)) mytiles++;
	    else if(board->get(opp_color, 1, 0)) opptiles++;
	  }
	if(!board->occupied(0, 7))
	  {
	    if(board->get(my_color, 0, 6)) mytiles++;
	    else if(board->get(opp_color, 0, 6)) opptiles++;
	    if(board->get(my_color, 1, 6)) mytiles++;
	    else if(board->get(opp_color, 1, 6)) opptiles++;
	    if(board->get(my_color, 1, 7)) mytiles++;
	    else if(board->get(opp_color, 1, 7)) opptiles++;
	  }
	if(!board->occupied(7,0))
	  {
	    if(board->get(my_color, 7, 1)) mytiles++;
	    else if(board->get(opp_color, 7, 1)) opptiles++;
	    if(board->get(my_color, 6, 1)) mytiles++;
	    else if(board->get(opp_color, 6, 1)) opptiles++;
	    if(board->get(my_color, 6, 0)) mytiles++;
	    else if(board->get(opp_color, 6, 0)) opptiles++;
	  }
	if(!board->occupied(7,7)) 
	  {
	    if(board->get(my_color, 6, 7)) mytiles++;
	    else if(board->get(opp_color, 6, 7)) opptiles++;
	    if(board->get(my_color, 6, 6)) mytiles++;
	    else if(board->get(opp_color, 6, 6)) opptiles++;
	    if(board->get(my_color, 7, 6)) mytiles++;
	    else if(board->get(opp_color, 7, 6)) opptiles++;
	  }
	closenessscore = -12.5 * (mytiles - opptiles);

	// Holding Corners?
	double cornerscore;
	mytiles = 0; opptiles = 0;
	if(board->get(my_color, 0, 0)) mytiles++;
	else if(board->get(opp_color, 0, 0)) opptiles++;
	if(board->get(my_color, 0, 7)) mytiles++;
	else if(board->get(opp_color, 0, 7)) opptiles++;
	if(board->get(my_color, 7, 0)) mytiles++;
	else if(board->get(opp_color, 7, 0)) opptiles++;
	if(board->get(my_color, 7, 7)) mytiles++;
	else if(board->get(opp_color, 7, 7)) opptiles++;
	cornerscore = 25 * (mytiles - opptiles);
	
	// final weighted score
	double score = (10 * parityscore) + (801.724 * cornerscore) + 
	  (382.026 * closenessscore) + (78.922 * mobscore) + 
	  (74.396 * frontierscore) + (10 * diskstabilityscore);
	return score;
}

double minimax(Board* board, Side s, int depth)
{
  //board->printBoard();
  int MAX_DEPTH = 1;
  double min =(infinity);
  std::vector<Move*> *moves = getMoves(board, s);
  for (unsigned int i = 0; i < moves->size(); i++) {
    Move *m = (*moves)[i];
    Board * temp = board->copy();
    double score = 0;
    //std::cerr << "depth: " << depth << " (" <<(*moves)[i]->getX() << "," << (*moves)[i]->getY() << ")" << std::endl;
    if (s == BLACK){
      temp->doMove(m, BLACK);
      if (depth < MAX_DEPTH){
	score = minimax(temp, WHITE, depth+1);
      }
      else{
	//temp->printBoard();
	score = uWashingtonHeuristic(temp, BLACK, WHITE);
      }
      if (-score < min){
	min = -score;
      }
    }
    else{
      temp->doMove(m, WHITE);
      if (depth < MAX_DEPTH){
	score = minimax(temp, BLACK, depth+1);
      }
      else{
	//temp->printBoard();
	score = uWashingtonHeuristic(temp, WHITE, BLACK);
      }
      if (score < min){
	min = score;
      }
    }
    //std::cerr << "depth: " << depth << " score: " << score << " " << min <<" Color: " << s <<" (" <<(*moves)[i]->getX() << "," << (*moves)[i]->getY() << ")" << std::endl;
    delete temp;
  }
  if (s == BLACK){
    return -min;
  }
  return min;
}
