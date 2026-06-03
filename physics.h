#pragma once
#include "ray.h"

namespace Physics {
    void computeAccelerations(const Ray& ray, float& a_r, float& a_phi);
    void verletStep(Ray& ray, float dt);
    float adaptiveStep(float r);
    void testConservation(Ray ray, int steps, float dt);
}