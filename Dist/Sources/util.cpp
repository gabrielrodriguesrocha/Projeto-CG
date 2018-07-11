// Local Headers
#include "util.hpp"

/*
De Casteljau's Algorithm for calculating Bézier curves
Thanks to Nils Pipenbrinck @ http://www.cubic.org/docs/bezier.htm
*/

void lerp(glm::vec3& dest, const glm::vec3 a, const glm::vec3 b, const float t)
{
    dest.x = a.x + (b.x-a.x)*t;
    dest.y = a.y + (b.y-a.y)*t;
    dest.z = a.z + (b.z-a.z)*t;
}

// evaluate a point on a bezier-curve. t goes from 0 to 1.0
void bezier(glm::vec3 &dest, const glm::vec3& a, const glm::vec3& b, const glm::vec3& c, const glm::vec3& d, const float t)
{
    glm::vec3 ab,bc,cd,abbc,bccd;
    lerp(ab, a,b,t);           // point between a and b (green)
    lerp(bc, b,c,t);           // point between b and c (green)
    lerp(cd, c,d,t);           // point between c and d (green)
    lerp(abbc, ab,bc,t);       // point between ab and bc (blue)
    lerp(bccd, bc,cd,t);       // point between bc and cd (blue)
    lerp(dest, abbc,bccd,t);   // point on the bezier-curve (black)
}