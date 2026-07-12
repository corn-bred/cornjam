#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glad/glad.h>

struct Particle {
    glm::vec2 Position;
    glm::vec2 Velocity;
    float Duration;
    float MaxDuration;
    float Size;
    float padding1;
};

class Particles {
    GLuint SSBO;
    //unfinished
};