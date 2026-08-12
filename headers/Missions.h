#include <vector>
class Missions {

public:
  float longitude, latitude;

  Missions();
  Missions(float longitude, float latitude);

  std::vector<float> go_to_location(float longitude, float latitude);
};
