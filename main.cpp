#include "headers/Missions.h"
#include <iostream>
#include <memory>
#include <string>
#include <vector>
using namespace std;

int main() {

  std::shared_ptr<Missions> mission;

  // std::vector<float> destination = mission->go_to_location(21.03f, 48.10f);

  cout << "Longitude: " << to_string(mission->longitude)
       << "\n Latitude: " << to_string(mission->latitude);

  return 0;
}
