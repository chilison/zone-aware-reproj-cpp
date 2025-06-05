#pragma once
#include <string>
#include <vector>

#include "GeoTIFFReader.hpp"
#include "RandomPointGenerator.hpp"

class PointAPI {
   public:
    PointAPI() = default;

    bool loadFromGeoTIFF(const std::string &jsonPath) {
        try {
            GeoTIFFReader reader(jsonPath);
            reader.load();
            points = reader.getPoints();
            return true;
        } catch (...) {
            return false;
        }
    }

    void generateRandomPoints(double x, double y) {
        Point start(50, 30);
        RandomPointGenerator generator(start);
        points = generator.getPoints();
    }

    const std::vector<Point> &getPoints() const { return points; }

    void clear() { points.clear(); }

   private:
    std::vector<Point> points;
};
