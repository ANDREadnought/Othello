#include "table.h"


Table::Table(std::string filename)
{
  this->filename = filename;
  this->table = new std::unordered_map<std::string, double>;
}

Table::~Table()
{
  delete this->table;
}

void Table::load()
{
  char board[100];
  char score[100];
  double scored;
  std::string boards;
  std::pair<std::string, double> entry;
  std::ifstream in(this->filename, std::fstream::in);
  if(in.is_open())
    {
      while(in)
	{
	  in >> board >> score;
	  scored = atof(score);
	  boards = board; 
	  entry.first = boards;
	  entry.second = scored;
	  this->table->insert(entry);
	}
      in.close();
    }
  else
    {
      throw FileNotOpenError();
    }
  }

void Table::save()
{
  std::string boards, scores;
  //char* board; //char board[64]; 
  //char* score; //char score[10];
  std::ofstream out(filename, std::fstream::out);
  if(out.is_open())
    {
      for(auto i = this->table->begin(); i != this->table->end(); ++i)
	{
	  boards = i->first; //probably conversion error
	  scores = std::to_string(i->second); //probably conversion error
	  const char* score = scores.c_str();
	  const char* board = boards.c_str();
	  out << board;
	  out << ' ';
	  out << score;
	  out << '\n';
	}
      out.close();
    }
  else
    {
      throw FileNotOpenError();
    }
}
