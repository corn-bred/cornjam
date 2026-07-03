#pragma once
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/exponential.hpp>
#include "collision.h"

class Entity {
    protected:
    glm::vec2 Direction;
    float DirectionRad;
    public:
    glm::vec3 Position;
    glm::vec2 Velocity;
    
    glm::vec2 ScaleLocal;
    glm::vec2 ScaleGlobal;

    void SetDirection(float directionRad) {
        DirectionRad = directionRad;
        Direction = glm::vec2(std::cos(directionRad), std::sin(directionRad));
    }

    void SetDirection(glm::vec2 direction) {
        Direction = direction;
        DirectionRad = glm::atan(direction.x, direction.y);
    }
};

class Player : public Entity {
    public:
    glm::vec2 Resistance;
    glm::vec2 Speed;
    AABBHitbox Hitbox;
    Player(glm::vec3 position = glm::vec3(0.0), float directionRad = 0.0, glm::vec2 speed = glm::vec2(0.0), glm::vec2 velocity = glm::vec2(0.0), glm::vec2 resistance = glm::vec2(1.0), glm::vec2 scaleLocal = glm::vec2(1.0), glm::vec2 scaleGlobal = glm::vec2(1.0), glm::vec2 hitboxPosition = glm::vec2(0.0), glm::vec2 hitboxSize = glm::vec2(1.0)) : Hitbox(hitboxPosition + glm::vec2(position.x, position.y), hitboxSize) {
        Position = position;
        Velocity = velocity;
        Resistance = resistance;
        Speed = speed;
        DirectionRad = directionRad;
        Direction = glm::vec2(std::cos(DirectionRad), std::sin(DirectionRad));
        ScaleLocal = scaleLocal;
        ScaleGlobal = scaleGlobal;
    }

    void VeloUpdate(double deltaTime, Player secondPlayer) {
        Position.x += Velocity.x * (float)(deltaTime);
        Hitbox.Origin.x = Position.x;
        if(Collision(Hitbox, secondPlayer.Hitbox)) {
            Position.x += snapToSurfaceX(Hitbox, secondPlayer.Hitbox, Velocity.x);
        }
        Hitbox.Origin.x = Position.x;
        Position.y += Velocity.y * (float)(deltaTime);
        Hitbox.Origin.y = Position.y;
        if(Collision(Hitbox, secondPlayer.Hitbox)) {
            Position.y += snapToSurfaceY(Hitbox, secondPlayer.Hitbox, Velocity.y);
        }
        Hitbox.Origin.y = Position.y;
        Velocity.x = Velocity.x * glm::pow(Resistance.x, deltaTime * 60);
        Velocity.y = Velocity.y * glm::pow(Resistance.y, deltaTime * 60);
        //Velocity.y = Velocity.y - (18000.0 * deltaTime);
    }

    glm::mat4 GetTransformMatrix() {
        glm::mat4 Matrix(1.0f);

        Matrix = glm::translate(Matrix, glm::vec3(Position.x, Position.y, 0.0));
        Matrix = glm::scale(Matrix, glm::vec3(ScaleGlobal.x, ScaleGlobal.y, 1.0));
        Matrix = glm::rotate(Matrix, DirectionRad, glm::vec3(0.0, 0.0,1.0));
        Matrix = glm::scale(Matrix, glm::vec3(ScaleLocal.x, ScaleLocal.y, 1.0));
        
        return Matrix;
    }
};