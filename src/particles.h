#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glad/glad.h>
#include <cornbreadlib/SSBO.h>
#include <cornbreadlib/computeshader.h>

struct Particle {
    glm::vec2 Position; //8 bytes = 8 =/8
    glm::vec2 Origin;   //8 bytes = 16 =/8
    glm::vec2 Velocity; //8 bytes = 24 =/8
    float Duration;     //4 bytes = 28 =/4
    float MaxDuration;  //4 bytes = 32 =/4
    float Size;         //4 bytes = 36 =/4
    uint32_t Flags;     //4 bytes = 40 =/4
};

#define FLAG_IS_ACTIVE (1u << 0) // 1
#define FLAG_IS_LOOPING   (1u << 1) // 2
#define FLAG_HAS_GRAVITY (1u << 2) // 4

class Particles {
    ComputeShader mainCompute;
    Particles() : mainCompute("src/shaders/particles.comp") {}
};