#include "headers/Pilot.h"
#include <iostream>
using namespace std;

int main() {
  Pilot pilot;

  pilot.set_longitude(21.02);
  pilot.set_latitude(41.201);

  cout << "Longitude: " << to_string(pilot.get_longitude())
       << "\nLatitude: " << to_string(pilot.get_latitude());

  return 0;
}
