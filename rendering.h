#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <glm/glm.hpp>
#include "ray.h"

namespace Rendering {

    struct Point {
        float x;
        float y;
    };

    void drawTriangle(Point a, Point b, Point c);
    void drawQuads(Point a, Point b, Point c, Point d);
    void drawCircle(float x, float y, float radius, int segments);
    void drawDashedCircle(float x, float y, float radius, int segments);
    void drawCircleOutline(Point center, float radius, int segments);
    std::vector<glm::vec2> generateStars(int count);
    void drawStars(const std::vector<glm::vec2>& stars);
    void drawRays(const std::vector<Ray>& rays, int currentFrame);
    void framebufferSizeCallback(GLFWwindow* window, int width, int height);

    struct RenderEngine {
        GLFWwindow* window;
        std::vector<glm::vec2> stars;
        std::vector<Ray>& rays;
        int frame;

        RenderEngine(std::vector<Ray>& simRays);
        ~RenderEngine();

        bool shouldClose();
        void beginFrame();
        void updatePhysics();
        void drawFrame();
        void endFrame();
    };

} // namespace Rendering