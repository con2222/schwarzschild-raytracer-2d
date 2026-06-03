#include "constants.h"
#include "ray.h"
#include "physics.h"
#include "rendering.h"

#include <vector>
#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif


void generateOrbitingRay(std::vector<Ray>& rays) {
    float startX = -0.9f * Visual::VIEW_WIDTH;
    float startY = 2.577934f * BlackHole::rs;
    rays.emplace_back(startX, startY, Physics::c, 0.f, RayScenario::ORBITING, Visual::ORBITING_START);
}

void generatePointSourceRays(std::vector<Ray>& rays) {
    int numRays = 25;
    float toBlackHoleX = 0.0f - Visual::POINT_SOURCE_X;
    float toBlackHoleY = 0.0f - Visual::POINT_SOURCE_Y;
    float baseAngle = std::atan2(toBlackHoleY, toBlackHoleX);
    float coneSpread = M_PI / 3.0f;

    for (int i = 0; i < numRays; i++) {
        float angleOffset = -coneSpread / 2.0f + coneSpread * i / (numRays - 1.0f);
        float currentAngle = baseAngle + angleOffset;
        float vx = Physics::c * std::cos(currentAngle);
        float vy = Physics::c * std::sin(currentAngle);
        rays.emplace_back(Visual::POINT_SOURCE_X, Visual::POINT_SOURCE_Y, vx, vy, RayScenario::POINT_SOURCE, Visual::POINT_SOURCE_START);
    }
}

void generateParallelRays(std::vector<Ray>& rays) {
    float parallelStartX = -Visual::VIEW_WIDTH;
    int numParallelRays = 70;
    float parallelVx = Physics::c;
    float parallelVy = 0.0f;

    for (int i = 0; i < numParallelRays; i++) {
        float t = i / (numParallelRays - 1.0f);
        float parallelStartY = -Visual::VIEW_HEIGHT + (t * Visual::VIEW_HEIGHT) * 2.f;
        rays.emplace_back(parallelStartX, parallelStartY, parallelVx, parallelVy, RayScenario::PARALLEL, Visual::PARALLEL_START);
    }
}

std::vector<Ray> generateRays() {
    std::vector<Ray> rays;
    generateOrbitingRay(rays);
    generatePointSourceRays(rays);
    generateParallelRays(rays);
    return rays;
}


int main() {
    std::vector<Ray> rays = generateRays();

    Rendering::RenderEngine engine(rays);

    while (!engine.shouldClose()) {
        engine.beginFrame();
        engine.updatePhysics();
        engine.drawFrame();
        engine.endFrame();
    }

    return 0;
}