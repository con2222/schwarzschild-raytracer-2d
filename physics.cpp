#include "physics.h"
#include "constants.h"
#include <cmath>
#include <iostream>
#include <algorithm>

namespace Physics {

    void computeAccelerations(const Ray& ray, float& a_r, float& a_phi) {
        float f = ray.get_f();
        float dt_dlambda = ray.get_dt_dlambda();

        a_r = -(BlackHole::rs / (2 * (ray.r * ray.r))) * f * (dt_dlambda * dt_dlambda) +
            (BlackHole::rs / (2 * (ray.r * ray.r) * f)) * (ray.v_r * ray.v_r) +
            (ray.r - BlackHole::rs) * (ray.v_phi * ray.v_phi);
        a_phi = -2.0f * (ray.v_r * ray.v_phi) / ray.r;
    }

    void verletStep(Ray& ray, float dt) {
        float a_r, a_phi;
        float halfStep = dt * 0.5f;
        computeAccelerations(ray, a_r, a_phi);

        ray.v_r = ray.v_r + a_r * halfStep;
        ray.v_phi = ray.v_phi + a_phi * halfStep;

        ray.r = ray.r + ray.v_r * dt;
        ray.phi = ray.phi + ray.v_phi * dt;

        computeAccelerations(ray, a_r, a_phi);
        ray.v_r = ray.v_r + a_r * halfStep;
        ray.v_phi = ray.v_phi + a_phi * halfStep;
    }

    float adaptiveStep(float r) {
        float factor = (r - Simulation::transitionStart) / (Simulation::transitionEnd - Simulation::transitionStart);
        factor = std::clamp(factor, 0.f, 1.f);
        return Simulation::minStep + factor * (Simulation::maxStep - Simulation::minStep);
    }

    void testConservation(Ray ray, int steps, float dt) {
        float initial_L = ray.L;
        float initial_E = ray.E;

        for (int i = 0; i < steps; ++i) {
            verletStep(ray, dt);
        }

        float current_L = ray.r * ray.r * ray.v_phi;

        std::cout << "After " << steps << " steps:\n";
        std::cout << "  L drift: " << std::abs(current_L - initial_L) / initial_L * 100.0f << "%\n";
    }

} // namespace Physics