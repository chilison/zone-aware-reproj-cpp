#pragma once

#include <string>

class Point
{
public:
    double lat;
    double lon;

    double easting = 0.0;
    double northing = 0.0;
    int utm_zone = 0;
    char hemisphere = 'N';

    double x_tm, y_tm;
    double x_eqdc, y_eqdc;

    Point() = default;

    Point(double lat, double lon);

private:
    void calculateUTM();
};
