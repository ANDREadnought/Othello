#include "timer.h"
#include "common.h"


/**
 *@brief empty constructor
 **/
Timer::Timer(){}
/**
 *@brief Set the amount of time remaining
 *@param int count -- the new remaining amount of time
 **/
void Timer::setRemaining(int count)
{
  this->remaining = std::chrono::milliseconds(count);
  this->startTimer();
}

/**
 *@brief Starts the start timer for timing durations passed.
 **/
void Timer::startTimer()
{
  this->start = std::chrono::steady_clock::now();
}

/**
 *@brief Finds time since last action and updates remaining time
 **/
void Timer::updateRemaining()
{
  this->end = std::chrono::steady_clock::now();
  try
    {
      this->remaining = this->remaining - std::chrono::duration_cast<std::chrono::milliseconds>(this->end - this->start);
    }
  catch(std::exception)
    {
    }
  this->start = std::chrono::steady_clock::now();
}

/**
 *@brief returns the remaining time on the clock
 **/
int Timer::getRemaining()
{
  return remaining.count();
}
