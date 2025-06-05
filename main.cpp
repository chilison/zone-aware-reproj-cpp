#include <iostream>

#include "API.hpp"

int main() {
    PointAPI api;

    if (api.loadFromGeoTIFF("example.json")) {
        std::cout << "Loaded points from GeoTIFF:\n";
        for (const auto &pt : api.getPoints()) {
            std::cout << "TM: " << pt.x_tm << ", " << pt.y_tm << "\n";
            std::cout << "EQDC: " << pt.x_eqdc << ", " << pt.y_eqdc << "\n";
        }
    } else {
        std::cerr << "Failed to load from GeoTIFF\n";
    }

    Point start(50, 30);
    RandomPointGenerator generator(start);
    std::cout << "Generated random points:\n";
    for (const auto &pt : api.getPoints()) {
        std::cout << "TM: " << pt.x_tm << ", " << pt.y_tm << "\n";
        std::cout << "EQDC: " << pt.x_eqdc << ", " << pt.y_eqdc << "\n";
    }

    return 0;
}
