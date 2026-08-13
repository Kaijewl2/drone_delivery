#include "../headers/Missions.h"
#include <vector>

Missions::Missions() {}
Missions::Missions(const Coordinates &coords) : coords(coords) {};

std::vector<float> Missions::go_to_location() {
  std::vector<float> lng_lat;

  lng_lat[0] = coords.get_longitude();
  lng_lat[1] = coords.get_latitiude();

  return lng_lat;
}
