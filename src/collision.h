#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class AABBHitbox {
    public:
    glm::vec2 Origin;
    glm::vec2 Scale;
    AABBHitbox(glm::vec2 origin = glm::vec2(0.0), glm::vec2 scale = glm::vec2(0.0)) : Origin(origin), Scale(scale) {}
};

bool Collision(AABBHitbox &a, AABBHitbox &b) {
    glm::vec2 aMin = a.Origin; glm::vec2 aMax= a.Origin + a.Scale;
    glm::vec2 bMin = b.Origin; glm::vec2 bMax= b.Origin + b.Scale;
    return (aMin.x <= bMax.x && aMax.x >= bMin.x) &&
           (aMin.y <= bMax.y && aMax.y >= bMin.y);
}