#ifndef INTERSECTION_H
#define INTERSECTION_H

#include <vector>
#include <future>
#include <mutex>
#include <memory>
#include "TrafficObject.h"
#include "TrafficLight.h"

class Street;
class Vehicle;

class WaitingVehicles {
 public:
  int getSize();

  void pushBack(const std::shared_ptr<Vehicle>& vehicle, std::promise<void> &&promise);
  void permitEntryToFirstInQueue();

 private:
  std::vector<std::shared_ptr<Vehicle>> _vehicles;
  std::vector<std::promise<void>> _promises;
  std::mutex _mutex;
};

class Intersection : public TrafficObject {
 public:
  Intersection();

  void setIsBlocked(bool isBlocked);

  void addVehicleToQueue(const std::shared_ptr<Vehicle>& vehicle);
  void addStreet(const std::shared_ptr<Street>& street);
  std::vector<std::shared_ptr<Street>> queryStreets(const std::shared_ptr<Street>& incoming);
  void simulate();
  void vehicleHasLeft(const std::shared_ptr<Vehicle>& vehicle);
  bool trafficLightIsGreen();

 private:

  // typical behaviour methods
  void processVehicleQueue();

  // private members
  std::vector<std::shared_ptr<Street>> _streets;
  WaitingVehicles _waitingVehicles;
  bool _isBlocked;

  TrafficLight _trafficLight;
};

#endif
