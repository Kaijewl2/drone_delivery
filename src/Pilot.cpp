#include "../headers/Pilot.h"
#include <vector>

float Pilot::get_latitude() { return coords.get_latitiude(); }

float Pilot::get_longitude() { return coords.get_longitude(); }

std::vector<float> Pilot::get_destination() { return coords.get_destination(); }

void Pilot::set_latitude(float lat) { coords.latitude = lat; }

void Pilot::set_longitude(float lng) { coords.longitude = lng; }

void Pilot::set_destination(float lat, float lng) {
  coords.set_destination(lat, lng);
}
