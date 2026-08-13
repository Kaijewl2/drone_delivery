#include "../headers/Coordinates.h"
#include "../headers/Missions.h"
#include <vector>

class Pilot {
public:
  float get_latitude();
  float get_longitude();
  std::vector<float> get_destination();

  void set_latitude(float lat);
  void set_longitude(float lng);
  void set_destination(float lat, float lng);

private:
  Coordinates coords;
};
