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
  void startTimer();
  void updateRemaining();
  void setRemaining(int count);
  int getRemaining();
 private:
  std::chrono::milliseconds remaining;
  std::chrono::steady_clock::time_point start;
  std::chrono::steady_clock::time_point end;
};

#endif
