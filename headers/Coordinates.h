#pragma once

#include <vector>

class Coordinates {
public:
  float latitude;
  float longitude;
  std::vector<float> destination;

  float get_latitiude();
  float get_longitude();
  std::vector<float> get_destination();

  void set_latitude(float lat);
  void set_longitude(float lng);
  void set_destination(float lat, float lng);
};
