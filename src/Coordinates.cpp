#pragma once

#include "../headers/Coordinates.h"

// Getters
float Coordinates::get_latitiude() { return latitude; }

float Coordinates::get_longitude() { return longitude; }

// Setters
void Coordinates::set_latitude(float lat) { latitude = lat; }

void Coordinates::set_longitude(float lng) { longitude = lng; }

void Coordinates::set_destination(float lat, float lng) {
  latitude = lat;
  longitude = lng;
}
