#pragma once

#include <functional>
#include <vector>

#include "Point.hpp"
#include "ProjectionEngine.hpp"

class ProjectionManager {
   public:
    ProjectionManager();

    void checkAndReset();
    void update(const Point &p);
    void registerCallback(std::function<void(const TMParameters &, const EQDCParameters &)> cb);
    const std::vector<Point> &getHistory() const { return history; }

   private:
    double accumulatedLat = 0.0;
    double accumulatedLon = 0.0;
    double baseLat = 0.0;
    double baseLon = 0.0;
    bool isFirst = true;

    std::vector<Point> history;

    std::function<void(const TMParameters &, const EQDCParameters &)> callback;
};
