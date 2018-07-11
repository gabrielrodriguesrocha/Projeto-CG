#pragma once

#include <glm/glm.hpp>

/*
De Casteljau's Algorithm for calculating Bézier curves
Thanks to Nils Pipenbrinck @ http://www.cubic.org/docs/bezier.htm
*/

void lerp(glm::vec3& dest, const glm::vec3 a, const glm::vec3 b, const float t);

// evaluate a point on a bezier-curve. t goes from 0 to 1.0
void bezier(glm::vec3 &dest, const glm::vec3& a, const glm::vec3& b, const glm::vec3& c, const glm::vec3& d, const float t);