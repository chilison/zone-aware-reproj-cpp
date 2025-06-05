#include "GeoTIFFReader.hpp"

#include <gdal_priv.h>
#include <ogr_spatialref.h>

#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

GeoTIFFReader::GeoTIFFReader(const std::string &jsonPath) : path(jsonPath) {}

bool GeoTIFFReader::load() {
    std::ifstream file(path);
    if (!file) {
        std::cerr << "Failed to open JSON file: " << path << "\n";
        return false;
    }

    json j;
    try {
        file >> j;
    } catch (const std::exception &e) {
        std::cerr << "Failed to parse JSON: " << e.what() << "\n";
        return false;
    }

    std::string tiffPath = j["geotiff_path"];
    const auto &pixelCenters = j["pixel_centers"];

    GDALAllRegister();
    GDALDataset *dataset = static_cast<GDALDataset *>(GDALOpen(tiffPath.c_str(), GA_ReadOnly));
    if (!dataset) {
        std::cerr << "Failed to open GeoTIFF: " << tiffPath << "\n";
        return false;
    }

    double gt[6];
    if (dataset->GetGeoTransform(gt) != CE_None) {
        std::cerr << "Failed to get geotransform from TIFF\n";
        GDALClose(dataset);
        return false;
    }

    const char *projRef = dataset->GetProjectionRef();
    epsg = 0;
    if (projRef && strlen(projRef) > 0) {
        OGRSpatialReference sr;
        if (sr.importFromWkt(projRef) == OGRERR_NONE) {
            sr.AutoIdentifyEPSG();
            const char *authName = sr.GetAuthorityName(nullptr);
            const char *code = sr.GetAuthorityCode(nullptr);
            if (authName && std::string(authName) == "EPSG" && code) {
                epsg = std::atoi(code);
            }
        }
    }

    for (const auto &p : pixelCenters) {
        int px = p["x"];
        int py = p["y"];

        double xGeo = gt[0] + px * gt[1] + py * gt[2];
        double yGeo = gt[3] + px * gt[4] + py * gt[5];

        double lat = 0.0, lon = 0.0;
        bool ok = transformToWGS84(xGeo, yGeo, epsg, lon, lat);

        if (ok) {
            points.emplace_back(lat, lon);
        } else {
            std::cerr << "Failed to transform point (" << xGeo << ", " << yGeo << ") to WGS84\n";
        }
    }

    GDALClose(dataset);
    return true;
}

std::vector<Point> &GeoTIFFReader::getPoints() { return points; }

int GeoTIFFReader::getEPSG() const { return epsg; }

bool GeoTIFFReader::transformToWGS84(double x, double y, int epsgCode, double &lon,
                                     double &lat) const {
    OGRSpatialReference srcSRS;
    if (srcSRS.importFromEPSG(epsgCode) != OGRERR_NONE) return false;

    OGRSpatialReference dstSRS;
    dstSRS.SetWellKnownGeogCS("WGS84");

    OGRCoordinateTransformation *transform = OGRCreateCoordinateTransformation(&srcSRS, &dstSRS);
    if (!transform) return false;

    lon = x;
    lat = y;

    int success = transform->Transform(1, &lon, &lat);
    OCTDestroyCoordinateTransformation(transform);
    return success != 0;
}