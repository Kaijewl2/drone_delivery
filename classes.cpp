#include <iostream>

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
  };
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
