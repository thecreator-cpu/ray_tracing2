#include "directlightingintegrator.h"

Color3f DirectLightingIntegrator::Li(const Ray &ray, const Scene &scene, std::shared_ptr<Sampler> sampler, int depth) const
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

    int index = sampler->Get2D()[0] * scene.lights.length();

    std::shared_ptr<Light> l = scene.lights[index];



    Point2f xi = sampler->Get2D();
    Vector3f wi;
    Float pdf;
    Color3f f = isect.bsdf->f(wo,wi);

    Color3f sample_li = l->Sample_Li(isect,xi,&wi,&pdf);


    Ray ray2 = isect.SpawnRay(wi);

    float visibility = 1.f;
    Intersection test = Intersection();
    if (scene.Intersect(ray2,&test)){
        if(QString::compare(test.objectHit->name,l->name) != 0){
            visibility = 0;
        }
    }


    return isect.Le(wo) + f * sample_li *glm::abs(glm::dot(wi,isect.normalGeometric)) * visibility/pdf;
}
