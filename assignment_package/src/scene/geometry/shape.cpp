#include "shape.h"
#include <QDateTime>

pcg32 Shape::colorRNG = pcg32(QDateTime::currentMSecsSinceEpoch());


void Shape::InitializeIntersection(Intersection *isect, float t, Point3f pLocal) const
{
    isect->point = Point3f(transform.T() * glm::vec4(pLocal, 1));
    ComputeTBN(pLocal, &(isect->normalGeometric), &(isect->tangent), &(isect->bitangent));
    isect->uv = GetUVCoordinates(pLocal);
    isect->t = t;
}

Intersection Shape::Sample(const Intersection &ref, const Point2f &xi, float *pdf) const
{
    //TODO
    Intersection ans = Sample(xi,pdf);
    //correct points?
    float d = glm::distance(ref.point, ans.point);
    glm::vec3 wi = ref.normalGeometric - ans.normalGeometric;
    float dot = abs(glm::dot(wi,ans.normalGeometric));
    *pdf = *pdf * d * d / dot;
    if (dot == 0){
        pdf = 0;
    }
    return ans;
}
