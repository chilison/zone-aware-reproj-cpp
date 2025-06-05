#pragma once
#include <vector>

#include "Point.hpp"

class PointSource {
   public:
    virtual std::vector<Point> &getPoints() = 0;
    virtual ~PointSource() = default;
};
