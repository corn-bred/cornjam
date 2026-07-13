#version 430 core

struct Particle {
    vec2 Position;
    vec2 Origin; 
    vec2 Velocity;
    float Duration;   
    float MaxDuration;
    float Size;       
    uint Flags;   
    float Speed;   
    float padding; 
};

layout(std430, binding = 0) buffer ParticleBuffer {
    Particle particles[];
} pb;

uniform mat4 view;
uniform mat4 projection;

out float vAlpha;
out vec2 vTexCoords;

void main() {
    vec2 quadVertices[6] = vec2[6](
        vec2(-0.5, -0.5),
        vec2(0.5, -0.5),
        vec2(0.5, 0.5),

        vec2(-0.5, -0.5),
        vec2(0.5, 0.5),
        vec2(-0.5, 0.5) 
    );

    vec2 quadTexCoords[6] = vec2[6](
        vec2(0.0, 0.0),
        vec2(1.0, 0.0),
        vec2(1.0, 1.0),
        vec2(0.0, 0.0),
        vec2(1.0, 1.0),
        vec2(0.0, 1.0) 
    );

    uint particleIndex = gl_VertexID / 6; // Which particle are we on
    uint vertexID = gl_VertexID % 6;  //While vertex of the particle are we on

    Particle particle = pb.particles[particleIndex];
    
    vec2 OffsetPos = quadVertices[vertexID] * particle.Size; //offsetted by the origin

    vec3 WorldPos = vec3(particle.Position + OffsetPos, 0.0);

    gl_Position = projection * view * vec4(WorldPos, 1.0);

    vAlpha = (particle.MaxDuration > 0.0) ? 1.0 - particle.Duration / particle.MaxDuration : 1.0;
    vTexCoords = quadTexCoords[vertexID];
}