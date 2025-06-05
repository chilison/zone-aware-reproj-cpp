#include "ProjectionEngine.hpp"

#include <proj.h>

#include <iostream>
#include <vector>

#include "Point.hpp"

void ProjectionEngine::updateProjections(std::vector<Point> &points, const TMParameters &tm,
                                         const EQDCParameters &eqdc) {
    PJ_CONTEXT *ctx = proj_context_create();

    // Transverse Mercator
    std::string tm_proj = "+proj=tmerc +lat_0=" + std::to_string(tm.lat_0) +
                          " +lon_0=" + std::to_string(tm.lon_0) + " +ellps=WGS84 +units=m +no_defs";

    PJ *tm_transform = proj_create_crs_to_crs(ctx,
                                              "EPSG:4326",  // WGS84
                                              tm_proj.c_str(), nullptr);

    // Equidistant Conic
    std::string eqdc_proj = "+proj=eqdc +lat_1=" + std::to_string(eqdc.lat_1) +
                            " +lat_2=" + std::to_string(eqdc.lat_2) +
                            " +lat_0=" + std::to_string((eqdc.lat_1 + eqdc.lat_2) / 2.0) +
                            " +lon_0=" + std::to_string(eqdc.lon_0) +
                            " +ellps=WGS84 +units=m +no_defs";

    PJ *eqdc_transform = proj_create_crs_to_crs(ctx, "EPSG:4326", eqdc_proj.c_str(), nullptr);

    if (!tm_transform || !eqdc_transform) {
        std::cerr << "Initializing transformer PROJ error.\n";
        return;
    }

    PJ_COORD input, output_tm, output_eqdc;

    for (auto &p : points) {
        input = proj_coord(p.lon, p.lat, 0, 0);

        output_tm = proj_trans(tm_transform, PJ_FWD, input);
        p.x_tm = output_tm.xy.x;
        p.y_tm = output_tm.xy.y;

        output_eqdc = proj_trans(eqdc_transform, PJ_FWD, input);
        p.x_eqdc = output_eqdc.xy.x;
        p.y_eqdc = output_eqdc.xy.y;
    }

    proj_destroy(tm_transform);
    proj_destroy(eqdc_transform);
    proj_context_destroy(ctx);
}
