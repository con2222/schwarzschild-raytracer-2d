[Читать на русском](README.ru.md) | [Read in English](README.md)

# 2D Schwarzschild Black Hole Ray-Tracer

An interactive simulation of relativistic physics and gravitational lensing effects in the vicinity of a static black hole. Written in **C++17** using **OpenGL**, this project visualizes how an intense gravitational field warps the trajectories of light rays (photons) according to Einstein's General Theory of Relativity within the Schwarzschild metric.

## Simulation Demo
https://github.com/user-attachments/assets/99a70baf-ab0a-44f2-90bb-47a4107d695f

---

## Simulation Scenarios

The engine sequentially triggers three distinct physical scenarios distributed across the timeline:

1. **Photon Orbiting (Starts at Frame 0):** Demonstrates critical light behavior on the edge of the photon sphere. The photon is launched tangentially and continuously loops around the event horizon.
2. **Point Source Rays (Starts at Frame 180):** A cone of light rays is emitted from a single localized source (e.g., a distant star or probe). This showcases the beam splitting effect: some rays fall into the singularity, some graze it, and others escape with severe deflections.
3. **Parallel Rays / Gravitational Lensing (Starts at Frame 360):** A flat wavefront of parallel rays emulates light arriving from an infinitely distant celestial object. As they pass the black hole, they focus and distort, demonstrating classical gravitational lensing.

---

## Customization & Experiments (`constants.h`)

You can thoroughly modify the physics, screen geometry, and performance of the simulation without touching the core engine code. Open `constants.h` and tweak the values inside the namespaces:

```cpp
namespace Physics {
    const float c{ 1.0f };       // Speed of light
    const float G{ 1.0f };       // Gravitational constant
}

namespace BlackHole {
    const float rs{ 1.0f };      // Schwarzschild radius
    const float MASS{ 0.5f };    // Black hole mass
}

namespace Visual {
    const int NUM_STARS = 200;            // Background star count
    const int WIDTH = 1920;               // Window width resolution
    const int HEIGHT = 1080;              // Window height resolution
    const float VIEW_WIDTH = 8.0f;        // Orthographic projection bound X
    const float VIEW_HEIGHT = 6.0f;       // Orthographic projection bound Y
    const float POINT_SOURCE_X = -7.6f;   // X coordinate of the point light source
    const float POINT_SOURCE_Y = 5.1f;    // Y coordinate of the point light source
    const int CIRCLE_SEGMENTS = 100;      // Quality of drawn circles (tessellation)

    // Scenario timeline triggers (in frames)
    const int ORBITING_START = 0;         
    const int POINT_SOURCE_START = 180;   
    const int PARALLEL_START = 360;       
}

namespace Simulation {
    const float minStep{ 0.01f };         // Minimum integrator step size (near horizon)
    const float maxStep{ 0.1f };          // Maximum integrator step size (at a distance)
    const float transitionStart = 1.0f;   // Radius where step size optimization finishes
    const float transitionEnd = 10.0f;    // Radius where step size optimization begins
    const float MAX_DISTANCE = 16.0f;     // Radius beyond which a ray is considered Escaped
    const float INTEGRATION_STEP = 0.05f; // Distance covered by a ray per frame execution
}

```

### Things to Try:

* **Kinematic Smoothness (`INTEGRATION_STEP`):** Reduce it (e.g., to `0.01f`) for an ultra-smooth, cinematic slower trace, or increase it for instant trajectory paths.
* **Overlapping Timelines:** By setting `POINT_SOURCE_START` and `PARALLEL_START` to `0`, you can force all scenarios to run simultaneously right from launch, rendering a massive grid of interacting light vectors.
* **Ray Density:** To adjust the total number of light rays generated, simply change the iteration counts in the `generatePointSourceRays` and `generateParallelRays` loops inside `main.cpp`.

---

## Project Architecture

The codebase relies on a modular architecture:

* `constants.h` — Centralized configuration file holding all physical, mathematical, and window presets.
* `ray.h` / `ray.cpp` — Declares the `Ray` data structure, tracking its positional history (trail), execution state, and high-level integration bounds.
* `physics.h` / `physics.cpp` — The core numerical computation engine (calculating relativistic acceleration coefficients, Verlet steps, and adaptive constraints).
* `rendering.h` / `rendering.cpp` — An OpenGL-powered `RenderEngine` abstraction wrapper handling GLFW window bounds, frame buffering, and raw vertex drawing operations.
* `main.cpp` — App entry point orchestrating simulation sequences and managing the global runtime loop.

---

## Building and Running

### Prerequisites

The project is cross-platform. System dependencies are cleanly managed via the **vcpkg** package manager:

```bash
vcpkg install glfw3 glew glm
```

### Building via Console (CMake)

1. Clone the repository:

```bash
git clone https://github.com/con2222/schwarzschild-raytracer-2d.git
cd opengl-blackhole
```

2. Configure the project using your local vcpkg toolchain file:

```bash
cmake -S . -B build -DCMAKE_TOOLCHAIN_FILE="D:/vcpkg/scripts/buildsystems/vcpkg.cmake"
```

3. Compile the executable build:

```bash
cmake --build build
```

## Credits

This project was built as part of the guided tracks on the [HelloC++](https://www.hellocpp.dev/) learning platform, focusing on modern C++ and interactive graphics simulations.
