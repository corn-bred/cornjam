#include <cornbreadlib/utility.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

float quadData[24] = 
{   
    0, 0,    0.0f, 0.0f,
    1, 0,    1.0f, 0.0f,
    1, 1,    1.0f, 1.0f,
    0, 0,    0.0f, 0.0f,
    1, 1,    1.0f, 1.0f,
    0, 1,    0.0f, 1.0f
};


float lerpToTime(float lerp, float DeltaTime) {
    float Smoothness = -60.0 * log(1.0 - lerp);
    return (1.0 - glm::exp(-Smoothness * DeltaTime));
}