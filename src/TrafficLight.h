#pragma once

#include <mutex>
#include <iostream>
#include <deque>
#include <condition_variable>
#include "TrafficObject.h"

class Vehicle;
enum TrafficLightPhase { red, green };

template<class T>
class MessageQueue {
 public:
  T receive();
  void send(T &&msg);

 private:
  std::condition_variable _condition;
  std::mutex _mutex;
  std::deque<T> _queue;
};

class TrafficLight : public TrafficObject {
 public:
  TrafficLight();

  TrafficLightPhase getCurrentPhase();

  void waitForGreen();
  void simulate() override;

 private:
  void cycleThroughPhases();
  TrafficLightPhase _currentPhase;

  std::shared_ptr<MessageQueue<TrafficLightPhase>> _queue{new MessageQueue<TrafficLightPhase>};
  std::condition_variable _condition;
  std::mutex _mutex;
};

