
#include "table.h"

/**
 *@brief Constructs a table entry
 *@param Board* board -- a pointer to the board representation
 * to be converted to a string and used as the key in the hash table
 *@param Move* best -- a pointer to the best move
 *@param double score -- the score that that board evaluated to
 *@param int pop -- a popularity score for the board
 **/
Entry::Entry(Board* board, Move* best, double score, int depth, int pop)
{
  this->board = board->toString();
  this->move = new Move(best->getX(), best->getY());
  this->score = score;
  this->depth = depth;
  this->pop = pop;
}

/**
 *@brief Constructs a table entry
 *@param Board* board -- a pointer to the board representation
 * to be converted to a string and used as the key in the hash table
 *@param std::string best -- a string representing the best move
 *@param double score -- the score that that board evaluated to
 *@param int pop -- a popularity score for the board
 **/
Entry::Entry(Board* board, std::string best, double score, int depth, int pop)
{
  int movei = atoi(best.c_str());
  this->board = board->toString();
  this->move = new Move(movei / 10, movei % 10);
  this->score = score;
  this->depth = depth;
  this->pop = pop;
}

/**
 *@brief Constructs a table entry
 *@param std::string board -- the string representation of the board,
 * to be used as a key
 *@param Move* best -- a pointer to the best move
 *@param double score -- the score that that board evaluated to
 *@param int pop -- a popularity score for the board
 **/
Entry::Entry(std::string board, Move* best, double score, int depth, int pop)
{
  this->board = board;
  this->move = new Move(best->getX(), best->getY());
  this->score = score;
  this->depth = depth;
  this->pop = pop;

}


/**
 *@brief Constructs a table entry
 *@param std::string board -- the string representation of the board,
 * to be used as a key
 *@param std::string best -- a string representation of the best move
 *@param double score -- the score that that board evaluated to
 *@param int pop -- a popularity score for the board
 **/
Entry::Entry(std::string board, std::string best, double score, int depth, int pop)
{
  int movei = atoi(best.c_str());
  this->board = board;
  this->move = new Move(movei / 10, movei % 10);
  this->score = score;
  this->depth = depth;
  this->pop = pop;
}

/**
 *@brief Standard entry destructor
 **/
Entry::~Entry()
{
  delete move;
}

/**
 *@brief returns a pointer to a copy of the move we have stored as the best for this move
 **/
Move* Entry::getMove()
{
  Move* ret = new Move(this->move->getX(), this->move->getY());
  return ret;
}


/**
 *@brief Standard table constructor
 *@param std::string filename -- the name of the file to be 
 * read from and written to 
 *@param int max_mem -- the maximum memory this table is allowed to use
 **/
Table::Table(std::string filename, int max_mem)
{
  this->filename = filename;
  this->table = new std::unordered_map<std::string, Entry*>(buckets);
  this->_size = 0;
  this->_bucket_count = 0;
  this->_memory = sizeof(Table);
  this->_max_memory = max_mem;
}


/**
 *@brief Standard destructor
 **/
Table::~Table()
{
  for(auto i = this->table->begin(); i != this->table->end(); ++i)
    {
      if(i->second) delete i->second;
    }
  delete this->table;
}

/**
 *@brief loads the table from the file specified in constructor
 **/
void Table::load()
{
  char board[100]; std::string boards; 
  char score[100]; double scored;
  char depth[10]; int depthi;
  char pop[10]; int popi;
  char move[5]; int movei;

  std::pair<std::string, Entry*> entry;
  std::ifstream in(this->filename, std::fstream::in);
  if(in.is_open())
    {
      while(in and this->_memory < this->_max_memory)
	{
	  in >> board >> score >> move >> depth >> pop;
	  scored = atof(score);
	  boards = board; 
	  movei = atoi(move);
	  depthi = atoi(depth);
	  popi = atoi(pop);
	  
	  entry.first = boards;
	  Move moveM(movei / 10, movei % 10);
	  entry.second = new Entry(boards, &moveM, scored, depthi, popi);
	  this->table->insert(entry);
	  this->_memory += sizeof(Entry);
	  this->_size++;
	}
      in.close();
    }
  else
    {
      throw FileNotOpenError();
    }
  this->_bucket_count = this->table->bucket_count();
}

/**
 *@brief Resaves the table in the file specified in constructor 
 * -- complete overwrite
 **/
void Table::save()
{
  std::string boards, scores;
  //char* board; //char board[64]; 
  //char* score; //char score[10];
  std::ofstream out(filename, std::fstream::out);
  Entry* entry = nullptr;
  if(out.is_open())
    {
      for(auto i = this->table->begin(); i != this->table->end(); ++i)
	{
	  entry = i->second;
	  const char* score = (std::to_string(entry->score)).c_str();
	  const char* board = entry->board.c_str();
	  const char* move = (std::to_string(entry->move->getX() * 10 + entry->move->getY())).c_str();
	  const char* depth = (std::to_string(entry->depth)).c_str();
	  const char* pop = (std::to_string(entry->pop)).c_str();
	  out << board << ' ';
	  out << score << ' ';
	  out << move << ' ';
	  out << depth << ' ';
	  out << pop;
	  out << '\n';
	}
      out.close();
    }
  else
    {
      throw FileNotOpenError();
    }
}

/**
 *@brief Returns the number of buckets the table has
 **/
int Table::bucket_count()
{
  return this->_bucket_count;
}

/**
 *@brief Returns the number of entries in the Table
 **/
int Table::size()
{
  return this->_size;
}

/**
 *@brief Adds a new Entry to the Table
 *@param Board* board -- a pointer to the board representation
 * to be converted to a string and used as the key in the hash table
 *@param Move* best -- a pointer to the best move
 *@param double score -- the score that that board evaluated to
 *@param int depth -- the depth this position was searched to
 **/
void Table::add(Board* board, Move* best, double score, int depth)
{
  if(this->_memory < this->_max_memory)
    {
      Move* move = new Move(best->getX(), best->getY());
      Entry* entry = new Entry(board->toString(), move, score, depth, 0);
      std::pair<std::string, Entry*> inserting = {entry->board, entry};
      this->table->insert(inserting);
      this->_size++;
      this->_memory += sizeof(Entry);
    }
}


/**
 *@brief Updates a previous Entry
  *@param Board* board -- a pointer to the board representation
 * to be converted to a string and used as the key in the hash table
 *@param Move* best -- a pointer to the best move
 *@param double score -- the score that that board evaluated to
 *@param int depth -- the depth this position was searched to
 **/
void Table::update(Board* board, Move* best, double score, int depth, int pop)
{
  Entry* entry = this->contains(board);
  if(!entry) return;
  entry->board = board->toString();
  entry->move = new Move(best->getX(), best->getY());
  entry->score = score;
  entry->depth = depth;
  entry->pop = pop;
}

/**
 *@brief Checks to see if the Board is in the Table.
 *@param Board* board -- a pointer to the Board in the table
 *@return nullptr board is not a member, an Entry* if it is.
 **/
Entry* Table::contains(Board* board)
{
  std::string key = board->toString();
  if(table->count(key) > 0) return (*table)[key];
  return nullptr;
}
