#pragma once

#include <geodesic.h>
#include <proj.h>

#include <vector>

#include "Point.hpp"
#include "PointSource.hpp"

class RandomPointGenerator : public PointSource {
   public:
    explicit RandomPointGenerator(const Point &startPoint);
    ~RandomPointGenerator() = default;

    std::vector<Point> &getPoints() override;

   private:
    Point startPoint;
    std::vector<Point> points;

    struct geod_geodesic geod;

    Point generateNextPoint(const Point &from, double distance_m, double azimuth_deg);
    double maxDistance();
    double geodDistance(const Point &a, const Point &b);
};
