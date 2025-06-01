#pragma once

#include "Point.hpp"
#include "ProjectionParameters.hpp"
#include <vector>

class ProjectionEngine
{
public:
    void updateProjections(std::vector<Point> &points,
                           const TMParameters &tm,
                           const EQDCParameters &eqdc);
};
