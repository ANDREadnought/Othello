#ifndef _OURTIMER
#define _OURTIMER

#include <chrono>

/**
 *@brief Timer for timing durations 
 *  Use to maximize time usage 
 **/
class Timer
{
 public:
  Timer();
  void startTimer(int totalTime);
  void updateRemaining();
  void setRemaining(int count);
  int getRemaining();
  bool canContinue();
  void progressTurn();
  int getTurnsRemaining();
 private:
  int turnsRemaining;
  int timePerMove;
  bool started;
  std::chrono::system_clock::duration remaining;
  std::chrono::system_clock::time_point start;
  std::chrono::system_clock::time_point end;
};

#endif
