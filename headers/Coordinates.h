#pragma once

class Coordinates {
public:
  float latitude;
  float longitude;

  float get_latitiude();
  float get_longitude();

  void set_latitude(float lat);
  void set_longitude(float lng);
  void set_destination(float lat, float lng);
};
