#include "naiveintegrator.h"

Color3f NaiveIntegrator::Li(const Ray &ray, const Scene &scene, std::shared_ptr<Sampler> sampler, int depth) const
{


    Vector3f wo = -ray.direction;

    Intersection isect = Intersection();
    Ray r = Ray(ray);

    if (!scene.Intersect(r, &isect)) {
        return Color3f(0.f);
    }

    if (depth == 0) {

        return isect.Le(wo);
    }

    //no material
    if (!isect.ProduceBSDF()){
        Color3f ans = isect.Le(wo);
        return ans;
    }

    Point2f xi = sampler->Get2D();
    Vector3f wi;
    Float pdf;
    Color3f f = isect.bsdf->Sample_f(wo,&wi,xi,&pdf);

    Ray ray2 = isect.SpawnRay(wi);

    Color3f ans = isect.Le(wo) + f * Li(ray2,scene,sampler,depth-1)/pdf;


    return isect.Le(wo) + f * Li(ray2,scene,sampler,depth-1)
            * glm::abs(glm::dot(wi,isect.normalGeometric))/pdf;
}
