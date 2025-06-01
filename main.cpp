#include <iostream>

#include "ProjectionEngine.hpp"
#include "ProjectionManager.hpp"
#include "RandomPointGenerator.hpp"

int main() {
    Point start(50, 30);
    RandomPointGenerator gen(start);
    ProjectionManager pm;
    ProjectionEngine engine;

    pm.registerCallback([&](const TMParameters &tm, const EQDCParameters &eqdc) {
        auto historyCopy = pm.getHistory();
        engine.updateProjections(historyCopy, tm, eqdc);
    });

    for (const auto &p : gen.getPoints()) {
        pm.update(p);
    }

    return 0;
}
