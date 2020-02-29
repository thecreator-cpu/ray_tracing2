#include "fresnel.h"

Color3f FresnelDielectric::Evaluate(float cosThetaI) const
{
    //TODO
    cosThetaI = std::clamp(cosThetaI, -1.f, 1.f);

    //Potentially swap indices of refraction
    bool entering = cosThetaI > 0.f;
    float metaI = etaI;
    float metaT = etaT;
    if (!entering) {
        metaI = etaT;
        metaT = etaI;
        cosThetaI = std::abs(cosThetaI);
    }

    //Compute cosThetaT using Snell’s law
    Float sinThetaI = std::sqrt(std::max((Float)0,
                                                1.f - cosThetaI * cosThetaI));
    Float sinThetaT = metaI / metaT * sinThetaI;

    //Handle total internal reflection>>
    Float cosThetaT = std::sqrt(std::max((Float)0,
                                            1.f - sinThetaT * sinThetaT));

    Float Rparl = ((metaT * cosThetaI) - (metaI * cosThetaT)) /
                  ((metaT * cosThetaI) + (metaI * cosThetaT));
    Float Rperp = ((metaI * cosThetaI) - (metaT * cosThetaT)) /
                  ((metaI * cosThetaI) + (metaT * cosThetaT));

    return Color3f((Rparl * Rparl + Rperp * Rperp) / 2.f);
}

