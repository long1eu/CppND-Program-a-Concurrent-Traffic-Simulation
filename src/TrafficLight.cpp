#include <iostream>
#include <random>
#include "TrafficLight.h"

/* Implementation of class "MessageQueue" */

std::shared_ptr<MessageQueue<int>> msgQueue(new MessageQueue<int>);

template<typename T>
T MessageQueue<T>::receive() {
  std::cout << " MessageQueue receive: thread id = " << std::this_thread::get_id() << std::endl;
  std::unique_lock<std::mutex> uLock(_mutex);

  _condition.wait(uLock, [this] {
    return !_queue.empty();
  });

  T msg = std::move(_queue.back());
  _queue.clear();
  return msg;
}

template<typename T>
void MessageQueue<T>::send(T &&msg) {

  std::lock_guard<std::mutex> uLock(_mutex);

  _queue.push_back(std::move(msg));
  _condition.notify_one();
}

/* Implementation of class "TrafficLight" */

TrafficLight::TrafficLight() {
  _currentPhase = TrafficLightPhase::red;
}

void TrafficLight::waitForGreen() {
  while (true) {
    TrafficLightPhase color;
    if (msgQueue->receive() == TrafficLightPhase::green) {
      return;
    }
  }
}

TrafficLightPhase TrafficLight::getCurrentPhase() {
  return TrafficLight::_currentPhase;
}

void TrafficLight::simulate() {
  threads.emplace_back(std::thread(&TrafficLight::cycleThroughPhases, this));
}

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"
void TrafficLight::cycleThroughPhases() {
  std::shared_ptr<MessageQueue<int>> messQueue(new MessageQueue<int>);
  std::chrono::time_point<std::chrono::system_clock> lastUpdate = std::chrono::system_clock::now();
  std::random_device rd;
  std::mt19937 eng(rd());
  std::uniform_int_distribution<> distr(4000, 6000);

  while (true) {
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
    int timeSinceLastUpdate =
        std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - lastUpdate).count();

    int cycleDuration = distr(eng);
    if (timeSinceLastUpdate >= cycleDuration) {
      lastUpdate = std::chrono::system_clock::now();
      if (_currentPhase == TrafficLightPhase::red) {
        _currentPhase = TrafficLightPhase::green;
      } else {
        _currentPhase = TrafficLightPhase::red;
      }
      msgQueue->send(_currentPhase);
    }
  }
}
#pragma clang diagnostic pop

