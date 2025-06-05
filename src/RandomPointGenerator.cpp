#include "RandomPointGenerator.hpp"

#include <algorithm>
#include <iostream>
#include <random>

constexpr double MAX_DISTANCE_KM = 100.0;

RandomPointGenerator::RandomPointGenerator(const Point &startPoint) : startPoint(startPoint) {
    points.push_back(startPoint);
    geod_init(&geod, 6378137.0, 1.0 / 298.257223563);
}

Point RandomPointGenerator::generateNextPoint(const Point &from, double distance_m,
                                              double azimuth_deg) {
    double lat2, lon2, az2;
    geod_direct(&geod, from.lat, from.lon, azimuth_deg, distance_m, &lat2, &lon2, &az2);

    return Point(lat2, lon2);
}

double RandomPointGenerator::geodDistance(const Point &a, const Point &b) {
    double s12;
    geod_inverse(&geod, a.lat, a.lon, b.lat, b.lon, &s12, nullptr, nullptr);
    return s12 / 1000.0;
}

double RandomPointGenerator::maxDistance() {
    double maxDist = 0.0;
    for (size_t i = 0; i < points.size(); ++i) {
        for (size_t j = i + 1; j < points.size(); ++j) {
            double dist = geodDistance(points[i], points[j]);
            maxDist = std::max(maxDist, dist);
        }
    }
    return maxDist;
}

std::vector<Point> &RandomPointGenerator::getPoints() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> azimuthDist(0.0, 180.0);
    std::uniform_real_distribution<> distanceDist(100.0, 1000.0);

    while (maxDistance() < MAX_DISTANCE_KM) {
        const Point &last = points.back();
        double azimuth = azimuthDist(gen);
        double distance = distanceDist(gen);

        Point next = generateNextPoint(last, distance, azimuth);
        points.push_back(next);
    }

    return points;
}
