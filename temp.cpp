#include <iostream>

class Coordinates {
public:
  Coordinates() { std::cout << "Coordinates obj"; }
  float latitude;
  float longitude;

  float get_latitiude();
  float get_longitude();

  void set_latitude(float lat);
  void set_longitude(float lng);
  void set_destination(float lat, float lng);
};

class Missions {

public:
  Missions() { std::cout << "Coordinates obj" << std::endl; }

  void go_to_location(/*float latitude, float longitude*/) {
    std::cout << "go_to_location" << std::endl;
  };
};

class Pilot {
public:
  Pilot() { std::cout << "Coordinates obj"; }

  float get_latitude();
  float get_longitude();

  void set_latitude(float lat);
  void set_longitude(float lng);
  void set_destination(float lat, float lng);

  void start_mission(char *mission_name);

private:
  Coordinates coords;
  Missions missions;
};

class Hello {

public:
  Hello() { std::cout << "Hello world!!!\n" << std::endl; }

  void saySomething() { std::cout << "something" << std::endl; }
};

#include <emscripten/bind.h>

using namespace emscripten;

EMSCRIPTEN_BINDINGS(jshello) {
  class_<Hello>("Hello").constructor<>().function("saySomething",
                                                  &Hello::saySomething);

  class_<Missions>("Missions")
      .constructor<>()
      .function("go_to_location", &Missions::go_to_location);
}
