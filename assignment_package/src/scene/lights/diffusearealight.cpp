#include "diffusearealight.h"

Color3f DiffuseAreaLight::L(const Intersection &isect, const Vector3f &w) const
{

    if(!twoSided){
        if(glm::dot(isect.normalGeometric,w) > 0){
            return emittedLight;
        }
        return Color3f(0.f);
    }
    return emittedLight;
}


Color3f DiffuseAreaLight::Sample_Li(const Intersection &ref, const Point2f &xi,
                                    Vector3f *wi, Float *pdf) const{

    Intersection inter = shape->Sample(ref, xi, pdf);
    if(pdf == 0 || ref.point == inter.point){
        return Color3f(0.f);
    }

    *wi = glm::normalize(inter.point - ref.point);

    return emittedLight;
}
