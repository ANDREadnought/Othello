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
    this->_solved = false;
    this->_saved = false;

    system("./extract.sh");
    // Transposition Table
    this->trans = new Table(transfile, transmem);
    try // Are we going to be using this across games?
      {
	this->trans->load();
      }
    catch(FileNotOpenError())
      {
	std::cerr << "Transpositon not loaded" << std::endl;
      }
    
    // Opening Book
    this->openings = new Table(openingfile, openingmem);
    try
      {
	this->openings->load();
      }
    catch(FileNotOpenError())
      {
	std::cerr << "Opening book not loaded" << std::endl;
      }
    
    this->closings = new Table(closingfile, closingmem);
    try
      {
	this->closings->load();
      }
    catch(FileNotOpenError())
      {
	std::cerr << "Closing book not loaded" << std::endl;
      }

    this->board = new Board();
    this->color = side;
    if(side == WHITE) this->oppcolor = BLACK;
    else this->oppcolor = WHITE;
    system("./compress.sh");
    std::cerr << "Constructed!" << std::endl;
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
  saveTables();
}

void Player::saveTables()
{
  system("./extract.sh");
  if(this->trans) 
    {
      try
	{
	  this->trans->save();
	}
      catch(FileNotOpenError())
	{
	  std::cerr << "Transposition Table not saved." << std::endl;
	  std::cerr << "File could not be opened." << std::endl;
	}
    }
  if(this->openings)
    {
      try
	{
	  this->openings->save();
	}
      catch(FileNotOpenError())
	{
	  std::cerr << "Opening Table not saved." << std::endl;
	  std::cerr << "File could not be opened." << std::endl;
	}
    }
   if(this->closings)
    {
      try
   	{
   	  this->closings->save();
   	}
      catch(FileNotOpenError())
   	{
   	  std::cerr << "Closing Table not saved." << std::endl;
   	  std::cerr << "File could not be opened." << std::endl;
   	}
    }
   system("./compress.sh");
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
  std::string color = (this->color==WHITE)?"WHITE":"BLACK";
  std::cerr << "ANDREadnought -- " << color << std::endl;
  this->board->doMove(opponentsMove, this->oppcolor);
  std::vector<Move*> *moves = nullptr;
  Move* todo = nullptr;
  moves = board->getMoves(this->color);
  todo = chooseMove(moves);
  if(todo) todo = todo->copy();
  else std::cerr << "Best Move: PASS :( " << std::endl;
  this->board->doMove(todo, this->color);
  this->timer.progressTurn();
  cleanMoves(moves);
  std::cerr << "Opening Table Entries: " << this->openings->size() << std::endl;
  std::cerr << "Opening Table Buckets: " << this->openings->bucket_count() << std::endl;
  std::cerr << "Transposition Table Entries: " << this->trans->size() << std::endl;
  std::cerr << "Transposition Table Buckets: " << this->trans->bucket_count() << std::endl;
  std::cerr << "Transposition Table Memory: "  << this->trans->memory() << std::endl;
  std::cerr << "Closing Table Entries: " << this->closings->size() << std::endl;
  std::cerr << "Closing Table Buckets: " << this->closings->bucket_count() << std::endl;
  if(this->_solved && !this->_saved && (this->timer.getRemaining() > 20000 || !this->timing)) {
    this->saveTables();
    std::cerr << "Tables Saved" << std::endl;
  }
  std::cerr << "--------------------------" << std::endl << std::endl;
  return todo;
}


int nodes = 0;
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
  double bestheur = -infinity;

  Entry* entry = nullptr;
  Move* winner = (*moves)[0];
  int search_depth = 1; 
  while(search_depth <= MAX_DEPTH)
    { 
      entry = this->trans->contains(board);
      if(entry) 
	{
	  Move* move = entry->getMove();
	  moves->insert(moves->begin(), move);
	}
      //only let new winner be the best move if we finish
      //searching the entire ply 
      Move* newWinner = winner;
      for(unsigned int i = 0; i < moves->size() && this->timer.canContinue(); i++)
	{
	  double heur;
	  Board* testboard = this->board->copy();
	  testboard->doMove((*moves)[i], this->color);
	  /*double guess;
	  Entry* testEntry = nullptr;
	  testEntry = this->trans->contains(board);
	  if (entry) {
	    guess = testEntry->getScore();
	  }
	  else{
	  guess = (this->color == BLACK)? uWashingtonHeuristic(testboard) : -uWashingtonHeuristic(testboard);
	  }*/
	  if (testboard->numValidMoves(this->oppcolor) > 0) {
	    heur = -negascout(testboard, this->oppcolor, search_depth-1, -infinity, infinity);
	    //heur = -MTDF(testboard, this->oppcolor, guess, search_depth-1);
	  }
	  else {
	    heur = negascout(testboard, this->color, search_depth-1, -infinity, infinity);
	    //heur = MTDF(testboard, this->color, guess, search_depth-1);
	  }
	  if (heur > bestheur) {
	    bestheur = heur;
	    newWinner = (*moves)[i];
	  }
	  delete testboard;
	}
      if(timer.canContinue() and newWinner) winner = newWinner;
      else break;
      search_depth++; 
    }
  if(search_depth-1 == MAX_DEPTH) this->_solved = true;
  std::cerr << "depth: " << search_depth-1 << std::endl;
  std::cerr << "Nodes: " << nodes << std::endl;
  std::cerr << "Evaluation: " << bestheur << std::endl;
  std::cerr << "Best move: " << " (" << winner->getX() << "," << winner->getY() << ")" << std::endl;
  nodes = 0;
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
  nodes++;
  Side opp = (s == WHITE)? BLACK : WHITE;
  if (board->isDone()) {
    double value =  heuristic(board) * 1000000;
    return (s == BLACK)? value : -value;
  }
  if (depth == 0) {
    double score =  (s == BLACK)? uWashingtonHeuristic(board) : -uWashingtonHeuristic(board);
    //std::cerr << "s: " << score << " Side: " << s << std::endl;
    return score;
  }

  double max = -infinity;
  
  Move* winner = nullptr;
  std::vector<Move*> *moves = board->getMoves(board, s);
  Entry* entry = this->trans->contains(board);
  if(entry)
  {
    Move* move = entry->getMove();
    moves->insert(moves->begin(), move);
  }
  for (unsigned int i = 0; i < moves->size(); i++) 
    {
      //Break if out of time.
      if(!this->timer.canContinue()) break;

      Move *m = (*moves)[i];
      Board * temp = board->copy();
      double score = 0;
      temp->doMove(m, s);
      if (temp->numValidMoves(opp) == 0)   {
	score = alphabeta(temp, s, depth-1, alpha, beta);
      }
      else  {
	score = -alphabeta(temp, opp, depth-1, -beta, -alpha);
      }
      if (score > max) {
	max = score;
	winner = m;
	if (score > alpha) alpha = score;
      }
      if (alpha > beta)	{
	delete temp;
	break;
      }
      delete temp;
    }
  if(!entry and winner) this->trans->add(board, winner, max, depth);
  else if(entry && winner && entry->depth < depth) this->trans->update(board, winner, max, depth, entry->depth);
  cleanMoves(moves);
  return max;
}
    
double Player::MTDF(Board *b, Side s, double f, int depth) {
  double g = f;
  double upper = infinity;
  double lower = -infinity;
  while (lower < upper-1) {
    //std::cerr << "g: " << g << " lower: " << lower << " upper: " << upper << std::endl;
    double beta = (g > lower+1)? g : lower+1;
    g = alphabeta(b, s, depth, beta-1, beta);
    if (g < beta) {
      upper = g;
    }
    else {
      lower = g;
    }
  }
  return g;
}

double Player::negascout(Board *board, Side s, int depth, double alpha, double beta) {
  nodes++;
  Side opp = (s == WHITE)? BLACK : WHITE;
  if (board->isDone()) {
    double value =  heuristic(board) * 1000000;
    return (s == BLACK)? value : -value;
  }
  if (depth == 0) {
    double score =  (s == BLACK)? uWashingtonHeuristic(board) : -uWashingtonHeuristic(board);
    return score;
  }
  double max = -infinity;
  
  Move* winner = nullptr;
  std::vector<Move*> *moves = board->getMoves(board, s);
  Entry* entry = this->trans->contains(board);
  if(entry)
  {
    Move* move = entry->getMove();
    moves->insert(moves->begin(), move);
  }
  for (unsigned int i = 0; i < moves->size(); i++) 
    {
      //Break if out of time.
      if(!this->timer.canContinue()) break;

      Move *m = (*moves)[i];
      Board * temp = board->copy();
      double score = 0;
      temp->doMove(m, s);
      if (entry && i != 1) {
	score = (temp->numValidMoves(opp) == 0)?
	  negascout(temp, s, depth-1, alpha, alpha+1) :
	  -negascout(temp, opp, depth-1, -alpha-1, -alpha);
	if (alpha < score) {
	  score = (temp->numValidMoves(opp) == 0)?
	    negascout(temp, s, depth-1, alpha, beta) :
	    -negascout(temp, opp, depth-1, -beta, -alpha);
	}
      }
      else{
	score = (temp->numValidMoves(opp) == 0)?
	  negascout(temp, s, depth-1, alpha, beta) :
	  -negascout(temp, opp, depth-1, -beta, -alpha);
      }
      if (score > max) {
	max = score;
	winner = m;
	if (score > alpha) alpha = score;
      }
      if (alpha > beta)	{
	delete temp;
	break;
      }
      delete temp;
    }
  if(!entry and winner) this->trans->add(board, winner, max, depth);
  else if(entry && winner && entry->depth < depth) this->trans->update(board, winner, max, depth, entry->depth);
  cleanMoves(moves);
  return max;
}
