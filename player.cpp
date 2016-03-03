#include "player.h"

/*
 * Constructor for the player; initialize everything here. The side your AI is
 * on (BLACK or WHITE) is passed in as "side". The constructor must finish 
 * within 30 seconds.
 */
Player::Player(Side side) {
    // Will be set to true in test_minimax.cpp.
    testingMinimax = false;
    this->trans = nullptr;
    this->openings = nullptr;
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
  for(int i = 0; i < moves->size(); i++)
    {
      double heur;
      Board* testboard = this->board->copy();
      testboard->doMove((*moves)[i], this->color);
      heur = heuristic(testboard);
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
