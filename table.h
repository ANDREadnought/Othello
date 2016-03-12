#ifndef _TABLE
#define _TABLE

#include <cstdlib>
#include <iostream>
#include <fstream>
#include <unordered_map>
#include <string>
#include "common.h"
#include "board.h"

class Entry
{
 public:
  Entry(Board* board, Move* best, double score, int depth, int pop);
  Entry(Board* board, std::string best, double score, int depth, int pop);
  Entry(std::string board, Move* best, double score, int depth, int pop);
  Entry(std::string board, std::string best, double score, int depth, int pop);
  ~Entry();

  std::string board;
  Move* move;
  double score;
  int depth;
  int pop;
  Move* getMove();
};

class Table
{
 public:
  std::string filename;
  std::unordered_map<std::string, Entry*>* table;
  Table(std::string filename, int max_mem);
  ~Table();
  void load();
  void save();
  int size();
  int bucket_count();
  void add(Board* board, Move* best, double score, int depth);
  void update(Board* board, Move* best, double score, int depth, int pop);
  Entry* contains(Board* board);
 private:
  int _memory;
  int _max_memory;
  int _size;
  int _bucket_count;
};

#endif
