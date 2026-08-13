#pragma once

#include "../headers/Coordinates.h"
#include <vector>

float Coordinates::get_latitiude() { return latitude; }

float Coordinates::get_longitude() { return longitude; }

std::vector<float> Coordinates::get_destination() { return destination; }

void Coordinates::set_latitude(float lat) { latitude = lat; }

void Coordinates::set_longitude(float lng) { longitude = lng; }

void Coordinates::set_destination(float lat, float lng) {
  destination[0] = lat;
  destination[1] = lng;
}
