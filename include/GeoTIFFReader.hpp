#pragma once
#include <string>
#include <vector>

#include "Point.hpp"
#include "PointSource.hpp"

class GeoTIFFReader : public PointSource {
   public:
    explicit GeoTIFFReader(const std::string &filepath);
    bool load();
    std::vector<Point> &getPoints() override;

    int getEPSG() const;

   private:
    std::string path;
    std::vector<Point> points;
    int epsg = 0;

    bool transformToWGS84(double x, double y, int epsgCode, double &lon, double &lat) const;
};
