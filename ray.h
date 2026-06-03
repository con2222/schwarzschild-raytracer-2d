#pragma once

#include <vector>
#include <glm/glm.hpp>


enum class RayScenario {
    PARALLEL,
    POINT_SOURCE,
    ORBITING
};

struct Ray {
    float r;
    float phi;
    float v_r;
    float v_phi;
    float L;
    float E;
    float initial_velocity_angle;
    float current_deflection = 0;
    int startFrame;
    RayScenario scenario;
    std::vector<glm::vec2> trail;

    Ray(float x, float y, float vx, float vy, RayScenario scenario, int startFrame);

    float get_f() const;
    float get_dt_dlambda() const;
    void print_values();
    bool isCaptured();
    bool hasEscaped(float maxDistance);
    void recordPosition();
    void updateDeflection();
    bool isActive(int currentFrame) const;
};

void integrate(Ray& ray, float distance, float maxDistance);