#include "player.h"

/*
 * Constructor for the player; initialize everything here. The side your AI is
 * on (BLACK or WHITE) is passed in as "side". The constructor must finish 
 * within 30 seconds.
 */
Player::Player(Side side) {
    // Will be set to true in test_minimax.cpp.
    testingMinimax = false;
    this->timing = true;

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
  this->timer.setRemaining(msLeft);
  if(msLeft == -1) this->timing = false;
    /* 
     * TODO: Implement how moves your AI should play here. You should first
     * process the opponent's opponents move before calculating your own move
     */ 
  std::cerr << std::endl << "--------------------------" << std::endl;
  this->board->doMove(opponentsMove, this->oppcolor);
  std::vector<Move*> *moves = nullptr;
  Move* todo = nullptr;
  moves = board->getMoves(this->color);
  todo = chooseMove(moves);
  this->board->doMove(todo, this->color);
  this->timer.progressTurn();
  delete moves;
  std::cerr << "--------------------------" << std::endl << std::endl;
  return todo;
}

/**
 *@brief Chooses the the next move -- is given the list of next moves
 * -- Searches with Minimax
 *@param std::vector<Move*>* moves -- a vector of Move pointers.
 **/
Move* Player::chooseMove(std::vector<Move*>* moves)
{
  int MAX_DEPTH = 20;
  if(!this->timing) MAX_DEPTH = 4;
  if(moves->size() < 1) return nullptr;

  //preset heuristic for keeping track of max
  double bestheur;
  if(this->color == BLACK)
    {
      bestheur = -infinity;
    }
  else bestheur = infinity;

  Move* winner = nullptr;
  int search_depth = 1; 
  while(search_depth <= MAX_DEPTH)
    { 
      //only let new winner be the best move if we finish
      //searching the entire ply 
      Move* newWinner = winner;
      for(unsigned int i = 0; i < moves->size() && this->timer.canContinue(); i++)
	{
	  double heur;
	  Board* testboard = this->board->copy();
	  testboard->doMove((*moves)[i], this->color);
	  if (this->color == WHITE)
	    heur = alphabeta(testboard, BLACK, search_depth, -infinity, infinity);
	  else
	    heur = alphabeta(testboard, WHITE, search_depth, -infinity, infinity);
	  if(this->color == BLACK and heur > bestheur)
	    {
	      bestheur = heur;
	      newWinner = (*moves)[i];
	    }
	  else if(this->color == WHITE and heur < bestheur) 
	    {
	      bestheur = heur;
	      newWinner = (*moves)[i];
	    }
	}
      if(timer.canContinue() and newWinner) winner = newWinner;
      else break;
      search_depth++; 
    }
  std::cerr << "depth: " << search_depth-1 << std::endl;
  std::cerr << "Evaluation: " << bestheur << std::endl;
  std::cerr << "Best move: " << " (" << winner->getX() << "," << winner->getY() << ")" << std::endl;
  return winner;
}

/**
 *@brief Naive heuristic -- You gotta start somewhere
 **/
double Player::heuristic(Board* board)
{
  return board->countBlack() - board->countWhite();
}


/**
 *@brief The heuristic combination that researchers at the University of
 *of Washington determined to be best
 *@param Board* board -- a pointer to the board representation we wish to evaluate
 **/
double uWashingtonHeuristic(Board* board)  {
	int whitetiles = 0, blacktiles = 0, whitefrontier = 0, blackfrontier = 0;

	int X[] = {-1, -1, 0, 1, 1, 1, 0, -1};
	int Y[] = {0, 1, 1, 1, 0, -1, -1, -1};
	
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
		  diskstabilityscore += weights[i][j];
		  whitetiles++;
		} 
	      else if(board->get(BLACK, i, j))  
		{
		  diskstabilityscore -= weights[i][j];
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

/**
 *@brief Minixmax algorithm for searching moves
 *@param Board* board -- the board to minimax on
 *@param Side s -- the current side of the player
 *@param int depth -- how deep to search
 **/
double Player::minimax(Board* board, Side s, int depth)
{
  //board->printBoard();
  Side opp;
  if (s == BLACK){
    opp = WHITE;
  }
  else{
    opp = BLACK;
  }
  double val;
  if (s == BLACK){
    val = -infinity;
  }
  else{
    val = infinity;
  }
  std::vector<Move*> *moves = board->getMoves(board, s);
  for (unsigned int i = 0; i < moves->size(); i++) 
    {
      //break if out of time.
      if(!this->timer.canContinue()) return val;
      
      Move *m = (*moves)[i];
      Board * temp = board->copy();
      double score = 0;
      temp->doMove(m, s);
      if (depth == 0) {
	score = uWashingtonHeuristic(temp);
      }
      else 
	{
	  if (temp->numValidMoves(opp) == 0) 
	    {
	      score = minimax(temp, s, depth-1);
	    }
	  else
	    {
	      score = minimax(temp, opp, depth-1);
	    }
	}
      
      if (s == BLACK && score > val){
	val = score;
      }
      else if (s == WHITE && score < val){
	val = score;
      }
      //std::cerr << "depth: " << depth << " score: " << score << " " << min <<" Color: " << s <<" (" <<(*moves)[i]->getX() << "," << (*moves)[i]->getY() << ")" << std::endl;
      delete temp;
    }
  return val;
}

/**
 *@brief Minimax with alphabeta pruning
 *@param Board* board -- the board to find the next best move
 *@param Side s -- the color of the current player
 *@param int depth -- how deep to search
 *@param double alpha -- alpha
 *@param double beta -- beta
 *@return heuristic at the principal variation search leaf node
 **/
double Player::alphabeta(Board* board, Side s, int depth, double alpha, double beta)
{
  //board->printBoard();
  Side opp;
  if (s == BLACK){
    opp = WHITE;
  }
  else{
    opp = BLACK;
  }
  double val;
  if (s == BLACK){
    val = -infinity;
  }
  else{
    val = infinity;
  }

  std::vector<Move*> *moves = board->getMoves(board, s);
  for (unsigned int i = 0; i < moves->size(); i++) 
    {
      if(!this->timer.canContinue()) return val;
      
      Move *m = (*moves)[i];
      Board * temp = board->copy();
      double score = 0;
      temp->doMove(m, s);
      if (depth == 0)
	{
	  score = uWashingtonHeuristic(temp);
	}
      else
	{
	  if (temp->numValidMoves(opp) == 0)
	    {
	      score = alphabeta(temp, s, depth-1, alpha, beta);
	    }
	  else
	    {
	      score = alphabeta(temp, opp, depth-1, alpha, beta);
	    }
	}
      
      if (s == BLACK && score > val)
	{
	  val = score;
	  if (score > alpha)
	    {
	      alpha = score;
	    }
	}
      else if (s == WHITE && score < val)
	{
	  val = score;
	  if (score < beta)
	    {
	      beta = score;
	    }
	}
      if (alpha > beta)
	{
	  break;
	}
      //std::cerr << "depth: " << depth << " score: " << score << " " << min <<" Color: " << s <<" (" <<(*moves)[i]->getX() << "," << (*moves)[i]->getY() << ")" << std::endl;
      delete temp;
    }
  return val;
}
