#include "../headers/Missions.h"
#include <vector>

Missions::Missions() : longitude(71.02f), latitude(41.38f) {}
Missions::Missions(float lng, float lat) : longitude(lng), latitude(lat) {};

std::vector<float> Missions::go_to_location(float lng, float lat) {
  std::vector<float> lng_lat;

  lng_lat[0] = lng;
  lng_lat[1] = lat;

  return lng_lat;
}
