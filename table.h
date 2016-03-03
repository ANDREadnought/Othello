#ifndef _TABLE
#define _TABLE

#include <cstdlib>
#include <iostream>
#include <fstream>
#include <unordered_map>
#include <string>
#include "common.h"

class Table
{
 public:
  std::string filename;
  std::unordered_map<std::string, double>* table;
  Table(std::string filename);
  ~Table();
  void load();
  void save();
};

#endif
