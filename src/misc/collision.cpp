#include <cornjam/collision.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

AABBHitbox::AABBHitbox(glm::vec2 origin = glm::vec2(0.0), glm::vec2 scale = glm::vec2(0.0)) : Origin(origin), Scale(scale) {}

bool Collision(AABBHitbox &a, AABBHitbox &b) {
    glm::vec2 aMin = a.Origin; glm::vec2 aMax= a.Origin + a.Scale;
    glm::vec2 bMin = b.Origin; glm::vec2 bMax= b.Origin + b.Scale;
    return ((aMin.x < bMax.x && aMax.x > bMin.x) &&
           (aMin.y < bMax.y && aMax.y > bMin.y));
}

float snapToSurfaceX(AABBHitbox &a, AABBHitbox &b, float xVelocity) {
    glm::vec2 aMin = a.Origin; glm::vec2 aMax= a.Origin + a.Scale;
    glm::vec2 bMin = b.Origin; glm::vec2 bMax= b.Origin + b.Scale;
    if (xVelocity > 0) {
        return (bMin.x - aMax.x);
    } else if (xVelocity < 0) {
        return (bMax.x - aMin.x);
    } else return 0;
}

float snapToSurfaceY(AABBHitbox &a, AABBHitbox &b, float yVelocity) {
    glm::vec2 aMin = a.Origin; glm::vec2 aMax= a.Origin + a.Scale;
    glm::vec2 bMin = b.Origin; glm::vec2 bMax= b.Origin + b.Scale;
    if (yVelocity > 0) {
        return (bMin.y - aMax.y);
    } else if (yVelocity < 0){
        return (bMax.y - aMin.y);
    } else return 0;
}