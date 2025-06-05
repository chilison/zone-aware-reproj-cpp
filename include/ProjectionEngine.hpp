#pragma once

#include <vector>

#include "Point.hpp"
#include "ProjectionParameters.hpp"

class ProjectionEngine {
   public:
    void updateProjections(std::vector<Point> &points, const TMParameters &tm,
                           const EQDCParameters &eqdc);
};
