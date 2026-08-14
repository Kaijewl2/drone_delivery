#include "../headers/Missions.h"
#include <iostream>

void Missions::go_to_location(float lat, float lng) {
  std::cout << "going to " << std::to_string(lat) << ", "
            << std::to_string(lng);
}
