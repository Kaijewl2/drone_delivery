

#include "Coordinates.h"
#include <vector>

class Missions {

public:
  Missions();
  Missions(const Coordinates &coords);

  std::vector<float> go_to_location();

private:
  Coordinates coords;
};
