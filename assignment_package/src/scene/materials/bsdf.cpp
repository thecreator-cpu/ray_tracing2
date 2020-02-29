#include "bsdf.h"
#include <warpfunctions.h>

BSDF::BSDF(const Intersection& isect, float eta /*= 1*/)
//TODO: Properly set worldToTangent and tangentToWorld
    : worldToTangent(),
      tangentToWorld(),
      normal(isect.normalGeometric),
      eta(eta),
      numBxDFs(0),
      bxdfs{nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr}
{

    Vector3f tan = Vector3f(0);
    Vector3f bit = Vector3f(0);
    CoordinateSystem(normal,&tan,&bit);
    UpdateTangentSpaceMatrices(normal,tan,bit);
}


void BSDF::UpdateTangentSpaceMatrices(const Normal3f& n, const Vector3f& t, const Vector3f b)
{
    //TODO: Update worldToTangent and tangentToWorld based on the normal, tangent, and bitangent
    tangentToWorld = glm::mat3(t,b,n);
    worldToTangent = glm::transpose(tangentToWorld );
}



//
Color3f BSDF::f(const Vector3f &woW, const Vector3f &wiW, BxDFType flags /*= BSDF_ALL*/) const
{
    //TODO
    Color3f col = Color3f(0,0,0);
    for(BxDF* b : bxdfs){
        if(b == nullptr){
            continue;
        }
        if(b->MatchesFlags(flags)){
           Vector3f woT = worldToTangent * woW;
           Vector3f wiT = worldToTangent * wiW;
           col += b->f(woT,wiT);
        }
    }

    return col;
}

// Use the input random number _xi_ to select
// one of our BxDFs that matches the _type_ flags.

// After selecting our random BxDF, rewrite the first uniform
// random number contained within _xi_ to another number within
// [0, 1) so that we don't bias the _wi_ sample generated from
// BxDF::Sample_f.

// Convert woW and wiW into tangent space and pass them to
// the chosen BxDF's Sample_f (along with pdf).
// Store the color returned by BxDF::Sample_f and convert
// the _wi_ obtained from this function back into world space.

// Iterate over all BxDFs that we DID NOT select above (so, all
// but the one sampled BxDF) and add their PDFs to the PDF we obtained
// from BxDF::Sample_f, then average them all together.

// Finally, iterate over all BxDFs and sum together the results of their
// f() for the chosen wo and wi, then return that sum.

Color3f BSDF::Sample_f(const Vector3f &woW, Vector3f *wiW, const Point2f &xi,
                       float *pdf, BxDFType type, BxDFType *sampledType) const
{
    // Use the input random number _xi_ to select
    // one of our BxDFs that matches the _type_ flags.
    //can it be out of bound?
    int index = xi[0]*BxDFsMatchingFlags(type);
    int i = 0;
    BxDF* bxdf;
    for(BxDF* b : bxdfs){
        if(b==nullptr){
            continue;
        }

        if (b->MatchesFlags(type)){
            if(i == index){
                bxdf = b;
                if(sampledType != nullptr){
                    *sampledType = b->type;
                }

            }
            i++;
        }

    }


    // rewrite the first uniform random number contained within _xi_
    Point2f x = Point2f((double) rand() / (RAND_MAX),xi[1]);

    // Convert woW and wiW into tangent space and pass them to
    // the chosen BxDF's Sample_f (along with pdf).
    // Store the color returned by BxDF::Sample_f and convert
    // the _wi_ obtained from this function back into world space.
    Vector3f woT = worldToTangent * woW;
    Vector3f wiT;
    Color3f col = Color3f(0);

    if(bxdf != nullptr) {
        col += bxdf->Sample_f(woT, &wiT, xi, pdf, sampledType);
    }
    *wiW = tangentToWorld * wiT;


    //check if specular
    if (bxdf->type & BSDF_SPECULAR){
        return col;
    }

    // Iterate over all BxDFs that we DID NOT select above (so, all
    // but the one sampled BxDF) and add their PDFs to the PDF we obtained
    // from BxDF::Sample_f, then average them all together.
    for(int i = 0; i < numBxDFs; i++) {
        if(bxdfs[i]==nullptr){
            continue;
        }
        if(bxdfs[i] ->MatchesFlags(type)){
            //has to match the type
            if(bxdfs[i]->type != bxdf->type){

                *pdf += bxdfs[i]->Pdf(woT,wiT);
            }
        }
    }

    *pdf = *pdf / float(BxDFsMatchingFlags(type));


    // Finally, iterate over all BxDFs and sum together the results of their
    // f() for the chosen wo and wi, then return that sum.
   for(const BxDF* b : bxdfs){
       if(b==nullptr){
           continue;
       }
       if(b -> MatchesFlags(type)){
           //has to match the type
           if(b->type != bxdf->type){
               col += b->f(woT, wiT);
           }
       }
   }
    return col;
}


float BSDF::Pdf(const Vector3f &woW, const Vector3f &wiW, BxDFType flags) const
{
    float ans = 0;
    for(int i = 0; i < numBxDFs; i++){
        if(bxdfs[i]->MatchesFlags(flags)){
           Vector3f woT = worldToTangent * woW;
           Vector3f wiT = worldToTangent * wiW;
           ans += bxdfs[i]->Pdf(woT,wiT);
        }
    }

    ans /= BxDFsMatchingFlags(flags);
    return ans;
}

Color3f BxDF::Sample_f(const Vector3f &wo, Vector3f *wi, const Point2f &xi,
                       Float *pdf, BxDFType *sampledType) const
{
    *wi = WarpFunctions::squareToHemisphereUniform(xi);
    if (wo.z < 0) {
        wi->z *= -1;
    }
    *pdf = Pdf(wo, *wi);
    return f(wo, *wi);
}

// The PDF for uniform hemisphere sampling
float BxDF::Pdf(const Vector3f &wo, const Vector3f &wi) const
{
    return SameHemisphere(wo, wi) ? Inv2Pi : 0;
}

BSDF::~BSDF()
{
    for(int i = 0; i < numBxDFs; i++)
    {
        delete bxdfs[i];
    }
}
