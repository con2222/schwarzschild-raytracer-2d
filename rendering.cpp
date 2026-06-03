#include "rendering.h"
#include "constants.h"
#include "physics.h"
#include <iostream>
#include <cmath>
#include <random>
#include <algorithm>
#include <execution>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

namespace Rendering {

    void drawTriangle(Point a, Point b, Point c) {
        glBegin(GL_TRIANGLES);
        glVertex2d(a.x, a.y);
        glVertex2d(b.x, b.y);
        glVertex2d(c.x, c.y);
        glEnd();
    }

    void drawQuads(Point a, Point b, Point c, Point d) {
        glBegin(GL_QUADS);
        glVertex2d(a.x, a.y);
        glVertex2d(b.x, b.y);
        glVertex2d(c.x, c.y);
        glVertex2d(d.x, d.y);
        glEnd();
    }

    void drawCircle(float x, float y, float radius, int segments) {
        float deltaAngle = static_cast<float>(360) / segments;
        glBegin(GL_TRIANGLE_FAN);
        glVertex2f(x, y);
        for (int i = 0; i <= segments; i++) {
            float rad = (i * deltaAngle) * M_PI / 180.f;
            glVertex2f(x + radius * std::cos(rad), y + radius * std::sin(rad));
        }
        glEnd();
    }

    void drawDashedCircle(float x, float y, float radius, int segments) {
        float deltaAngle = 360.f / segments;
        Point center = { x, y };
        glBegin(GL_LINES);
        for (int i = 0; i < segments; i += 2) {
            float rad1 = (i * deltaAngle) * M_PI / 180.f;
            float rad2 = ((i + 1) * deltaAngle) * M_PI / 180.f;

            glVertex2f(center.x + radius * std::cos(rad1), center.y + radius * std::sin(rad1));
            glVertex2f(center.x + radius * std::cos(rad2), center.y + radius * std::sin(rad2));
        }
        glEnd();
    }

    void drawCircleOutline(Point center, float radius, int segments) {
        float deltaAngle = static_cast<float>(360) / segments;
        glBegin(GL_LINE_LOOP);
        for (int i = 0; i < segments; i++) {
            float rad = (i * deltaAngle) * M_PI / 180.f;
            glVertex2f(center.x + radius * std::cos(rad), center.y + radius * std::sin(rad));
        }
        glEnd();
    }

    std::vector<glm::vec2> generateStars(int count) {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<float> distX(-8.0f, 8.0f);
        std::uniform_real_distribution<float> distY(-6.0f, 6.0f);

        std::vector<glm::vec2> stars;
        for (int i = 0; i < count; i++) {
            stars.emplace_back(distX(gen), distY(gen));
        }
        return stars;
    }

    void drawStars(const std::vector<glm::vec2>& stars) {
        glPointSize(2.f);
        glColor3f(1.0f, 1.0f, 1.0f);
        glBegin(GL_POINTS);
        for (const auto& star : stars) {
            glVertex2f(star.x, star.y);
        }
        glEnd();
    }

    void drawRays(const std::vector<Ray>& rays, int currentFrame) {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        for (const auto& ray : rays) {
            if (ray.trail.size() < 2 || !ray.isActive(currentFrame)) continue;

            glBegin(GL_LINE_STRIP);
            float factor = ray.current_deflection / M_PI;
            for (size_t i = 0; i < ray.trail.size(); i++) {
                float alpha = 0.2 + 0.8 * (static_cast<float>(i) / (ray.trail.size() - 1));

                if (ray.scenario == RayScenario::POINT_SOURCE) {
                    glColor4f(0.5f + factor * 0.5f, 1.0f, 0.f, alpha);
                }
                else if (ray.scenario == RayScenario::ORBITING) {
                    glColor4f(1.0f, 0.2f, 1.0f, alpha);
                }
                else {
                    glColor4f(factor, 0.5f * (1.f - factor), 1.0f - factor, alpha);
                }
                glVertex2f(ray.trail[i].x, ray.trail[i].y);
            }
            glEnd();

            glPointSize(5.f);
            glBegin(GL_POINTS);
            if (ray.scenario == RayScenario::POINT_SOURCE) {
                glColor3f(0.5f, 1.0f, 0.f);
            }
            else if (ray.scenario == RayScenario::ORBITING) {
                glColor3f(1.0f, 0.2f, 1.0f);
            }
            else {
                glColor3f(1.f, 1.f, 0.f);
            }
            glVertex2f(ray.trail.back().x, ray.trail.back().y);
            glEnd();
        }
        glDisable(GL_BLEND);
    }

    void framebufferSizeCallback(GLFWwindow* window, int width, int height) {
        glViewport(0, 0, width, height);
        float orthoWidth = Visual::VIEW_WIDTH;
        float orthoHeight = Visual::VIEW_HEIGHT;
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(-orthoWidth, orthoWidth, -orthoHeight, orthoHeight, -1.0, 1.0);
    }

    // --- RenderEngine Implementation ---

    RenderEngine::RenderEngine(std::vector<Ray>& simRays) : rays(simRays), frame(0) {
        if (!glfwInit()) {
            std::cerr << "Can't init glfw" << std::endl;
        }
        window = glfwCreateWindow(Visual::WIDTH, Visual::HEIGHT, "Blackhole-2D", nullptr, nullptr);
        glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
        glfwMakeContextCurrent(window);
        glfwSwapInterval(1);
        glewExperimental = GL_TRUE;

        if (glewInit() != GLEW_OK) {
            std::cerr << "Can't init glew" << std::endl;
        }
        framebufferSizeCallback(window, Visual::WIDTH, Visual::HEIGHT);

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        glClearColor(0.02f, 0.04f, 0.08f, 1.0f);

        glEnable(GL_LINE_SMOOTH);
        glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);

        stars = generateStars(Visual::NUM_STARS);
    }

    RenderEngine::~RenderEngine() {
        glfwDestroyWindow(window);
        glfwTerminate();
    }

    bool RenderEngine::shouldClose() {
        return glfwWindowShouldClose(window);
    }

    void RenderEngine::beginFrame() {
        glClear(GL_COLOR_BUFFER_BIT);
    }

    void RenderEngine::updatePhysics() {
        std::for_each(std::execution::par, rays.begin(), rays.end(), [this](auto& ray) {
            if (ray.isActive(frame)) {
                integrate(ray, Simulation::INTEGRATION_STEP, Simulation::MAX_DISTANCE);
            }
            });
    }

    void RenderEngine::drawFrame() {
        drawStars(stars);

        // Event horizon
        glColor3f(0.f, 0.f, 0.f);
        drawCircle(0.0f, 0.0f, BlackHole::rs, Visual::CIRCLE_SEGMENTS);

        // Photon sphere
        glColor3f(0.0f, 0.8f, 0.8f);
        glLineWidth(3.0f);
        drawDashedCircle(0.f, 0.f, 1.5f * BlackHole::rs, Visual::CIRCLE_SEGMENTS);

        // Point source marker
        glPointSize(8.0f);
        glColor3f(0.5f, 1.0f, 0.0f);
        glBegin(GL_POINTS);
        glVertex2f(Visual::POINT_SOURCE_X, Visual::POINT_SOURCE_Y);
        glEnd();

        drawRays(rays, frame);
    }

    void RenderEngine::endFrame() {
        glfwSwapBuffers(window);
        glfwPollEvents();
        frame++;
    }

} // namespace Rendering