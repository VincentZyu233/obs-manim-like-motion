#pragma once

#include <cmath>
#include <algorithm>

namespace manim {
namespace math {

inline float clamp01(float t) {
    return std::clamp(t, 0.0f, 1.0f);
}

inline float sigmoid(float x) {
    return 1.0f / (1.0f + std::exp(-x));
}

inline float linear(float t) {
    return clamp01(t);
}

inline float smooth(float t, float inflection = 10.0f) {
    t = clamp01(t);
    float error = sigmoid(-inflection / 2.0f);
    float val = (sigmoid(inflection * (t - 0.5f)) - error) / (1.0f - 2.0f * error);
    return clamp01(val);
}

inline float smoothstep(float t) {
    t = clamp01(t);
    return 3.0f * t * t - 2.0f * t * t * t;
}

inline float smootherstep(float t) {
    t = clamp01(t);
    return t * t * t * (t * (t * 6.0f - 15.0f) + 10.0f);
}

inline float rush_into(float t, float inflection = 10.0f) {
    t = clamp01(t);
    return 2.0f * smooth(t / 2.0f, inflection);
}

inline float rush_from(float t, float inflection = 10.0f) {
    t = clamp01(t);
    return 2.0f * smooth(t / 2.0f + 0.5f, inflection) - 1.0f;
}

inline float double_smooth(float t) {
    t = clamp01(t);
    if (t < 0.5f) {
        return 0.5f * smooth(2.0f * t);
    } else {
        return 0.5f * (1.0f + smooth(2.0f * t - 1.0f));
    }
}

inline float there_and_back(float t, float inflection = 10.0f) {
    if (t < 0.0f || t > 1.0f) return 0.0f;
    float new_t = (t < 0.5f) ? (2.0f * t) : (2.0f * (1.0f - t));
    return smooth(new_t, inflection);
}

inline float exponential_decay(float t, float half_life = 0.1f) {
    t = clamp01(t);
    return 1.0f - std::exp(-t * 0.693147f / half_life);
}

} // namespace math
} // namespace manim
