#include "ray.h"
#include "constants.h"
#include "physics.h"
#include <cmath>
#include <iostream>
#include <algorithm>

Ray::Ray(float x, float y, float vx, float vy, RayScenario scenario, int startFrame)
    : scenario(scenario), startFrame(startFrame) {
    r = std::sqrt(x * x + y * y);
    phi = std::atan2(y, x);
    v_r = vx * std::cos(phi) + vy * std::sin(phi);
    v_phi = (-vx * std::sin(phi) + vy * std::cos(phi)) / r;
    L = r * r * v_phi;
    float f = 1.0f - BlackHole::rs / r;
    E = f * Physics::c;

    trail.emplace_back(x, y);
    initial_velocity_angle = std::atan2(vy, vx);
}

float Ray::get_f() const {
    return 1.0f - BlackHole::rs / r;
}

float Ray::get_dt_dlambda() const {
    return E / get_f();
}

void Ray::print_values() {
    std::cout << "Ray initialized:\n";
    std::cout << "  Position: r = " << r / BlackHole::rs << " rs\n";
    std::cout << "  Energy: E = " << E << "\n";
    std::cout << "  Angular momentum: L = " << L << "\n\n";

    std::cout << "Conserved quantities:\n";
    std::cout << "  E = " << E << " (energy)\n";
    std::cout << "  L = " << L << " (angular momentum)\n";

    float b_normalized = (L / E) / BlackHole::rs;
    std::cout << "  Impact parameter b = " << b_normalized << " rs\n";
}

bool Ray::isCaptured() {
    return r <= BlackHole::rs * 1.01f;
}

bool Ray::hasEscaped(float maxDistance) {
    return r > maxDistance;
}

void Ray::recordPosition() {
    float x = r * std::cos(phi);
    float y = r * std::sin(phi);
    trail.emplace_back(x, y);
}

void Ray::updateDeflection() {
    float v_x = v_r * std::cos(phi) - r * v_phi * std::sin(phi);
    float v_y = v_r * std::sin(phi) + r * v_phi * std::cos(phi);

    float new_velocity = std::atan2(v_y, v_x);

    current_deflection = std::abs(initial_velocity_angle - new_velocity);
    if (current_deflection > M_PI) {
        current_deflection = 2.0 * M_PI - current_deflection;
    }
}

bool Ray::isActive(int currentFrame) const {
    if (currentFrame >= startFrame) return true;
    return false;
}

void integrate(Ray& ray, float distance, float maxDistance) {
    if (ray.isCaptured() || ray.hasEscaped(maxDistance)) return;
    float remaining = distance;
    while (remaining > 0.f) {
        float step = Physics::adaptiveStep(ray.r);
        step = std::min(step, remaining);
        Physics::verletStep(ray, step);
        ray.recordPosition();
        if (ray.isCaptured() || ray.hasEscaped(maxDistance)) break;
        remaining -= step;
    }
    ray.updateDeflection();
}