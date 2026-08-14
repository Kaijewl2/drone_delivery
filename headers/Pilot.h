#include "../headers/Coordinates.h"
#include "../headers/Missions.h"

class Pilot {
public:
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
