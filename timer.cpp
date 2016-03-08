#include "timer.h"
#include "common.h"


/**
 *@brief empty constructor
 **/
Timer::Timer()
{
  this->started = false;
}
/**
 *@brief Set the amount of time remaining
 *@param int count -- the new remaining amount of time
 **/
void Timer::setRemaining(int count)
{
  this->remaining = std::chrono::milliseconds(count);
  this->startTimer(count); 
}

/**
 *@brief Starts the start timer for timing durations passed.
 **/
void Timer::startTimer(int totalTime)
{
  if(!this->started)
    {
      this->started = true;
      this->turnsRemaining =  35;
      this-> timePerMove = totalTime / 60;
      this->start = std::chrono::steady_clock::now();
    }
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

bool Timer::canContinue()
{
  this->updateRemaining();
  if(this->getRemaining() / this->turnsRemaining < timePerMove)
    {
      return false;
    }
  else return true;
}
