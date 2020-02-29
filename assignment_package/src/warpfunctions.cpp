#define _USE_MATH_DEFINES
#include "warpfunctions.h"
#include <math.h>

glm::vec3 WarpFunctions::squareToDiskUniform(const glm::vec2 &sample)
{
    float r = sqrt(sample[0]);
    float theta = 2*M_PI*sample[1];
    float x = r * cos(theta);
    float y = r * sin(theta);
    return glm::vec3(x,y,0);
}

glm::vec3 WarpFunctions::squareToDiskConcentric(const glm::vec2 &sample)
{
    glm::vec2 uOffset = 2.f * sample - glm::vec2(1, 1);

    if (uOffset.x == 0 && uOffset.y == 0)
        return glm::vec3(0, 0, 0);

    float theta, r;
    if (std::abs(uOffset.x) > std::abs(uOffset.y)) {
       r = uOffset.x;
       theta = M_PI/4.f * (uOffset.y / uOffset.x);
    } else {
        r = uOffset.y;
        theta = M_PI/2.f - M_PI/4.f * (uOffset.x / uOffset.y);
    }
    return r * glm::vec3(std::cos(theta), std::sin(theta), 0);
}

float WarpFunctions::squareToDiskPDF(const glm::vec3 &sample)
{
    return 1.f/M_PI;
}

glm::vec3 WarpFunctions::squareToSphereUniform(const glm::vec2 &sample)
{
    float z = 1 - 2 * sample[0];
    float x = cos(2*M_PI*sample[1])*sqrt(1 - z*z);
    float y = sin(2*M_PI*sample[1])*sqrt(1 - z*z);
    return glm::vec3(x,y,z);
}

float WarpFunctions::squareToSphereUniformPDF(const glm::vec3 &sample)
{
    return 1.f/M_PI/4.f;
}

glm::vec3 WarpFunctions::squareToSphereCapUniform(const glm::vec2 &sample, float thetaMin)
{

    float x = sample[0] * (180.f-thetaMin)/180.f;
    return squareToSphereUniform(glm::vec2(x,sample[1]));

}

float WarpFunctions::squareToSphereCapUniformPDF(const glm::vec3 &sample, float thetaMin)
{
    float h = glm::cos(glm::radians(180.f-thetaMin));
    return 1.f/(2.f*M_PI*(1-h));
}

glm::vec3 WarpFunctions::squareToHemisphereUniform(const glm::vec2 &sample)
{
    float z = sample[0];
    float x = cos(2*M_PI*sample[1])*sqrt(1 - z*z);
    float y = sin(2*M_PI*sample[1])*sqrt(1 - z*z);
    return glm::vec3(x,y,z);
}

float WarpFunctions::squareToHemisphereUniformPDF(const glm::vec3 &sample)
{
    return 1.f/M_PI / 2.f;
}

glm::vec3 WarpFunctions::squareToHemisphereCosine(const glm::vec2 &sample)
{
    glm::vec3 ans = squareToDiskUniform(sample);
    float x = ans[0];
    float y = ans[1];
    ans[2] = sqrt(1-x*x-y*y);
    return ans;
}

float WarpFunctions::squareToHemisphereCosinePDF(const glm::vec3 &sample)
{
    return sample[2] * 1.f/M_PI;
}
