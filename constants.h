#pragma once

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

namespace Physics {
    const float c{ 1.0f };
    const float G{ 1.0f };
}

namespace BlackHole {
    const float rs{ 1.0f };
    const float MASS{ 0.5f };
}

namespace Visual {
    const int NUM_STARS = 200;
    const int WIDTH = 1920;
    const int HEIGHT = 1080;

    const float VIEW_WIDTH = 8.0f;
    const float VIEW_HEIGHT = 6.0f;
    const float POINT_SOURCE_X = -0.95f * VIEW_WIDTH;
    const float POINT_SOURCE_Y = 0.85f * VIEW_HEIGHT;
    const int CIRCLE_SEGMENTS = 100;

    const int ORBITING_START = 0;
    const int POINT_SOURCE_START = 180;
    const int PARALLEL_START = 360;
}

namespace Simulation {
    const float minStep{ 0.01f };
    const float maxStep{ 0.1f };
    const float transitionStart = 1.0f;
    const float transitionEnd = 10.0f;

    const float MAX_DISTANCE = 2.0f * Visual::VIEW_WIDTH; // 16.0f
    const float INTEGRATION_STEP = 0.05f;
}