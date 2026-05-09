#pragma once

#include <utility>
#include <vector>

struct resolution {
  unsigned int width;
  unsigned int height;
};

static const resolution APP_RESOLUTION{1200, 800};
static const int FRAME_LIMIT{144};

using Coordinate = std::pair<float, float>;
using Coordinates = std::vector<Coordinate>;

enum Direction { UP, DOWN, LEFT, RIGHT };
