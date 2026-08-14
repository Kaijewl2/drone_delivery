#include "../headers/Pilot.h"
#include <cstring>
#include <iostream>

// Getters
float Pilot::get_latitude() { return coords.get_latitiude(); }

float Pilot::get_longitude() { return coords.get_longitude(); }

// Setters
void Pilot::set_latitude(float lat) { coords.latitude = lat; }

void Pilot::set_longitude(float lng) { coords.longitude = lng; }

void Pilot::set_destination(float lat, float lng) {
  coords.set_destination(lat, lng);
}

// Mission caller
void Pilot::start_mission(char *mission_name) {
  if (strcmp(mission_name, "go_to_location")) {
    missions.go_to_location(coords.latitude, coords.longitude);
  } else {
    std::cout << "not a mission\n";
  }
}
