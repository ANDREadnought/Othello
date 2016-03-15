#include "timer.h"
#include "common.h"
#include <iostream>


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
      this->turnsRemaining = 35;
      this-> timePerMove = totalTime / this->turnsRemaining;
    }
  this->start = std::chrono::system_clock::now();
}

/**
 *@brief Finds time since last action and updates remaining time
 **/
void Timer::updateRemaining()
{
  this->end = std::chrono::system_clock::now();

  std::chrono::system_clock::duration elapsed = this->end - this->start;
  //std::cerr << "elapsed: " << (std::chrono::duration_cast<std::chrono::milliseconds>(elapsed)).count() << std::endl;
  this->remaining = this->remaining - elapsed;

  this->start = std::chrono::system_clock::now();
}

/**
 *@brief returns the remaining time on the clock
 **/
int Timer::getRemaining()
{
  return (std::chrono::duration_cast<std::chrono::milliseconds>(remaining)).count();
}

bool Timer::canContinue()
{
  if(this->getRemaining() < 0) return true;
  this->updateRemaining();
  if(this->getRemaining() / this->turnsRemaining < timePerMove)
    {
      return false;
    }
  return true;
}

void Timer::progressTurn()
{
  this->turnsRemaining--;
  this->turnNumber += 2;
}

int Timer::getTurnsRemaining()
{
  return this->turnsRemaining;
}
int Timer::getTurnNumber()
{
  return this->turnNumber;
}

void Timer::setTurnNumber(int num)
{
  this->turnNumber = num;
}
