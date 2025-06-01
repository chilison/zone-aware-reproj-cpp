#include "ProjectionManager.hpp"
#include <cmath>
#include <iostream>

ProjectionManager::ProjectionManager() {}

void ProjectionManager::update(const Point &p)
{
    if (isFirst)
    {
        baseLat = p.lat;
        baseLon = p.lon;
        isFirst = false;
        return;
    }

    accumulatedLat += std::abs(p.lat - baseLat);
    accumulatedLon += std::abs(p.lon - baseLon);
    baseLat = p.lat;
    baseLon = p.lon;

    history.push_back(p);

    checkAndReset();
}

void ProjectionManager::registerCallback(std::function<void(const TMParameters &, const EQDCParameters &)> cb)
{
    callback = std::move(cb);
}

void ProjectionManager::checkAndReset()
{
    const double threshold = 0.1;

    if (accumulatedLon >= threshold || accumulatedLat >= threshold)
    {
        double sumLat = 0.0;
        double sumLon = 0.0;
        double minLat = 90.0;
        double maxLat = -90.0;

        for (const auto &p : history)
        {
            sumLat += p.lat;
            sumLon += p.lon;
            if (p.lat < minLat)
                minLat = p.lat;
            if (p.lat > maxLat)
                maxLat = p.lat;
        }

        double avgLat = sumLat / history.size();
        double avgLon = sumLon / history.size();

        TMParameters tm{avgLat, avgLon};
        EQDCParameters eqdc{minLat, maxLat, avgLon};

        if (callback)
            callback(tm, eqdc);

        accumulatedLat = 0.0;
        accumulatedLon = 0.0;
    }
}
