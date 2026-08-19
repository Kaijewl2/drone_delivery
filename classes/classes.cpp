#include <chrono>
#include <cstdint>
#include <future>
#include <iostream>
/*#include <mavsdk/mavsdk.hpp>
#include <mavsdk/plugins/action/action.hpp>
#include <mavsdk/plugins/telemetry/telemetry.hpp>
#include <memory>
#include <thread>

using namespace mavsdk;
using std::chrono::seconds;
using std::this_thread::sleep_for;
*/
class Coordinates {
public:
  Coordinates() { std::cout << "Coordinates obj"; }
  float latitude = 40.686;
  float longitude = -75.189;

  float get_latitude() { return latitude; };
  float get_longitude() { return longitude; };

  void set_latitude(float lat) { latitude = lat; };
  void set_longitude(float lng) { longitude = lng; };
  void set_destination(float lat, float lng) {
    latitude = lat;
    longitude = lng;
  };
};

class Missions {

public:
  Missions() { std::cout << "Missions obj" << std::endl; }

  void go_to_location(float latitude, float longitude) {
    std::cout << "traveling to coordinates Lat: " << latitude
              << " , Lng: " << longitude << std::endl;

  /*Mavsdk mavsdk{Mavsdk::Configuration{ComponentType::GroundStation}};
  ConnectionResult connection_result =
      mavsdk.add_any_connection("udpin://127.0.0.1:14552");

  if (connection_result != ConnectionResult::Success) {
    std::cerr << "Connection failed: " << connection_result << '\n';
    return 1;
  }

  auto system = mavsdk.first_autopilot(3.0);
  if (!system) {
    std::cerr << "Timed out waiting for system\n";
    return 1;
  }

  // Instantiate plugins.
  auto telemetry = Telemetry{system.value()};
  auto action = Action{system.value()};

  // We want to listen to the altitude of the drone at 1 Hz.
  const auto set_rate_result = telemetry.set_rate_position(1.0);
  if (set_rate_result != Telemetry::Result::Success) {
    std::cerr << "Setting rate failed: " << set_rate_result << '\n';
    return 1;
  }

  // Set up callback to monitor altitude while the vehicle is in flight
  telemetry.subscribe_position([](Telemetry::Position position) {
    std::cout << "Altitude: " << position.relative_altitude_m << " m\n";
  });

  // Check until vehicle is ready to arm
  while (telemetry.health_all_ok() != true) {
    std::cout << "Vehicle is getting ready to arm\n";
    sleep_for(seconds(1));
  }

  // Arm vehicle
  std::cout << "Arming...\n";
  const Action::Result arm_result = action.arm();

  if (arm_result != Action::Result::Success) {
    std::cerr << "Arming failed: " << arm_result << '\n';
    return 1;
  }

  // Take off
  std::cout << "Taking off...\n";
  const Action::Result takeoff_result = action.takeoff();
  if (takeoff_result != Action::Result::Success) {
    std::cerr << "Takeoff failed: " << takeoff_result << '\n';
    return 1;
  }

  // Let it hover for a bit before landing again.
  sleep_for(seconds(10));

  std::cout << "Landing...\n";
  const Action::Result land_result = action.land();
  if (land_result != Action::Result::Success) {
    std::cerr << "Land failed: " << land_result << '\n';
    return 1;
  }

  // Check if vehicle is still in air
  while (telemetry.in_air()) {
    std::cout << "Vehicle is landing...\n";
    sleep_for(seconds(1));
  }
  std::cout << "Landed!\n";

  // We are relying on auto-disarming but let's keep watching the telemetry
  // for a bit longer.
  sleep_for(seconds(3));
  std::cout << "Finished...\n";
*/ };
};

class Pilot {
public:
  Pilot() { std::cout << "Pilot obj"; }

  float get_latitude() { return coords.latitude; };
  float get_longitude() { return coords.longitude; };

  void set_latitude(float lat) { coords.set_latitude(lat); };
  void set_longitude(float lng) { coords.set_longitude(lng); };
  void set_destination(float lat, float lng) {
    coords.set_destination(lat, lng);
  };

  void go_to_location(float lat, float lng) {
    missions.go_to_location(lat, lng);
  }

private:
  Coordinates coords;
  Missions missions;
};

#include <emscripten/bind.h>

using namespace emscripten;

// Expose C++ functions to JS
EMSCRIPTEN_BINDINGS(jsgoodbye) {

  class_<Missions>("Missions")
      .constructor<>()
      .function("go_to_location", &Missions::go_to_location);

  class_<Coordinates>("Coordinates")
      .constructor<>()
      .function("get_latitude", &Coordinates::get_latitude)
      .function("get_longitude", &Coordinates::get_longitude)
      .function("set_latitude", &Coordinates::set_latitude)
      .function("set_longitude", &Coordinates::set_longitude)
      .function("set_destination", &Coordinates::set_destination);

  class_<Pilot>("Pilot")
      .constructor<>()
      .function("get_latitude", &Pilot::get_latitude)
      .function("get_longitude", &Pilot::get_longitude)
      .function("set_latitude", &Pilot::set_latitude)
      .function("set_longitude", &Pilot::set_longitude)
      .function("set_destination", &Pilot::set_destination)
      .function("go_to_location", &Pilot::go_to_location);
}
