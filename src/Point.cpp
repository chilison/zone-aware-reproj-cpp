#include "Point.hpp"
#include <proj.h>
#include <cmath>
#include <stdexcept>

Point::Point(double lat, double lon)
    : lat(lat), lon(lon)
{
    calculateUTM();
}

void Point::calculateUTM()
{
    PJ_CONTEXT *context = proj_context_create();

    int zone = static_cast<int>((lon + 180) / 6) + 1;
    utm_zone = zone;
    hemisphere = (lat >= 0) ? 'N' : 'S';

    std::string utm_def = "+proj=utm +zone=" + std::to_string(zone) +
                          (lat >= 0 ? " +north" : " +south") + " +datum=WGS84 +units=m +no_defs";

    PJ *transformation = proj_create_crs_to_crs(
        context,
        "EPSG:4326",
        utm_def.c_str(),
        nullptr);

    if (!transformation)
    {
        throw std::runtime_error("Failed to create PROJ transformation.");
    }

    transformation = proj_normalize_for_visualization(context, transformation);
    PJ_COORD coord = proj_coord(lon, lat, 0, 0);
    PJ_COORD result = proj_trans(transformation, PJ_FWD, coord);

    easting = result.xy.x;
    northing = result.xy.y;

    proj_destroy(transformation);
    proj_context_destroy(context);
}
