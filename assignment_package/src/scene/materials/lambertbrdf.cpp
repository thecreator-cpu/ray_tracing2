#include "lambertbrdf.h"
#include <warpfunctions.h>



//f must return the proportion of energy from wi that is reflected along wo for a Lambertian surface
Color3f LambertBRDF::f(const Vector3f &wo, const Vector3f &wi) const
{

    return R/(float)M_PI;
}


Color3f LambertBRDF::Sample_f(const Vector3f &wo, Vector3f *wi, const Point2f &u,
                        Float *pdf, BxDFType *sampledType) const
{

    return BxDF::Sample_f(wo, wi, u, pdf, sampledType);
}

float LambertBRDF::Pdf(const Vector3f &wo, const Vector3f &wi) const
{

    return SameHemisphere(wo, wi) ? WarpFunctions::squareToHemisphereUniformPDF(wi) : 0;
}
