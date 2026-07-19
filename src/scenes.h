#pragma once
#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <stb_image/stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <AL/al.h>
#include <AL/alc.h>

#include <cornbreadlib/utility.h>
#include <cornbreadlib/vertexbuffer.h>
#include <cornbreadlib/shaders.h>
#include <cornbreadlib/texturebuffer.h>

#include <dr_libs/dr_wav.h>

#include "entity.h"
#include "grid.h"
#include "collision.h"
#include "spritesheet.h"
#include "camera2D.h"
#include "particles.h"
#include "inputmanager.h"
#include "bitmaptext.h"
#include "globals.h"

class Scene;
class PausedScene;
class PlayingScene;


class Scene {
    public:
    virtual ~Scene() = default;

    virtual void Init() {}

    virtual void Update() = 0;

    virtual void Render() = 0;

    virtual void Exit() {}
};

//Custom scenes written below

class PausedScene : public Scene {
    TextRenderer *text;
    public:
    void Init() override {
        text = new TextRenderer("res/img/sans-serif.png", 15, 15, 72, 90, true);
    }
    void Update() override {
        if (input.isActionPressed(Action::Pause)) {
            game.ChangeScene(new PlayingScene());
        }
    }
    void Render() override {
        glm::mat4 TextView = glm::mat4(1.0);
        text->RenderText("Paused", glm::vec2(0, HEIGHT - 90), 1.0, TextView, Projection);
    }
    void Exit() override {
        delete text;
        text = nullptr;
    }
};

class PlayingScene : public Scene {
    Player* mainPlayer;
    VertexBuffer* mainVBO;
    Shader* mainShader;
    TextureBuffer* playerTexture;
    GridSpace* WorldGrid;
    Animation* CubeRotating;
    Camera2D* mainCamera;
    Particles* particleTest;
    TextRenderer* text;

    glm::vec2 OldDifference;

    public:

    void Init() override {
        mainPlayer = new Player(glm::vec3(WIDTH / 2.0f, HEIGHT / 2.0f, 1.0), 0.0f, glm::vec2(150.0f, 1700.0f), glm::vec2(0.0), glm::vec2(1000.0f, 30000.0f), glm::vec2(0.8f), glm::vec2(0.92f), glm::vec2(50.0f), glm::vec2(1.0), glm::vec2(0.0), glm::vec2(50.0), glm::vec2(1.0f, 0.5f), 0.1);

        mainVBO = new VertexBuffer(quadData, sizeof(quadData), GL_STATIC_DRAW);
        mainVBO->addAttribute(0, 2, GL_FLOAT, 4, 0);
        mainVBO->addAttribute(1, 2, GL_FLOAT, 4, 2);

        mainShader = new Shader("src/shaders/main.vert", "src/shaders/main.frag");

        playerTexture = new TextureBuffer("res/img/goofy ahh thing.png");

        WorldGrid = new GridSpace(glm::vec2(1200/10, 800/10), glm::vec3(0.0));

        WorldGrid->Data = {
        0,0,1,0,2,0,3,0,4,0,5,0,6,0,7,0,8,0,9,0,
        -1,0,-1,1,-1,2,-1,3,-1,4,-1,5,-1,6,-1,7,-1,8,-1,9,-1,10,
        10,0,10,1,10,2,10,3,10,4,10,5,10,6,10,7,10,8,10,9,10,10
        };

        CubeRotating = new Animation(0, 49, 0.04167, true, "res/img/rotatingcube.png", 10, 5, GL_CLAMP_TO_BORDER,GL_CLAMP_TO_BORDER, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR, false); // 1/24 is irrational

        mainCamera = new Camera2D(glm::vec3(0.0, 0.0, 0.0), glm::vec2(1.0), 0.0);

        OldDifference = glm::vec2(0.0);

        particleTest = new Particles("src/shaders/particles.comp", 50, glm::vec2(1200 / 2, 800 / 2), 2.0, 10.0, 100.0, true, true);

        particleTest->RenderSolidColourState(glm::vec3(1.0, 0.8, 0.6));

        text = new TextRenderer("res/img/sans-serif.png", 15, 15, 72, 90, true);
    }

    void Update() override {
        //Scene update!!11!
        if (input.isActionPressed(Action::Pause)) {
            game.ChangeScene(new PausedScene());
        }

        mainPlayer->KeyboardUpdate(input);

        //Camera updates
        glm::vec3 NewPos = mainCamera->CameraToEntity(*mainPlayer, WIDTH, HEIGHT, lerpToTime(0.1, DeltaTime));
        glm::vec2 Difference = glm::vec2(NewPos.x - mainCamera->Position.x, NewPos.y - mainCamera->Position.y);
        
        mainCamera->Scale = glm::vec2(1.0 + abs(mainPlayer->Velocity.x * 0.0001) );
        mainCamera->Position = NewPos;

        OldDifference = Difference;

        //Updates

        mainPlayer->VeloUpdate(*WorldGrid, 300, 6000.0f);

        CubeRotating->Update();

        particleTest->Update(glm::vec2(0.0, -100.0));
    }

    void Render() override {
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        mainShader->use();
        glm::mat4 Model = mainPlayer->GetTransformMatrix();
        mainShader->setMat4("model", Model);

        glm::mat4 View = mainCamera->GetViewMatrix(WIDTH, HEIGHT);
        mainShader->setMat4("view", View);

        mainShader->setMat4("projection", Projection);
        
        mainShader->setBool("isSolidColour", false);
        mainShader->setBool("isAnimation", false);

        playerTexture->bindTexture(0);

        mainVBO->bind();

        glDrawArrays(GL_TRIANGLES, 0, 6);

        WorldGrid->RenderAll(*mainShader, *mainVBO, View, Projection);

        glm::mat4 animModel = glm::mat4(1.0);
        animModel = glm::scale(animModel, glm::vec3(100, 100, 0));
        CubeRotating->RenderSprite(*mainShader, *mainVBO, animModel, View, Projection);

        particleTest->Render(View, Projection);
    }

    void Exit() {
        delete mainPlayer;
        delete mainVBO;
        delete mainShader;
        delete mainPlayer;
        delete WorldGrid;
        delete CubeRotating;
        delete mainCamera;
        delete particleTest;
        delete text;
        mainPlayer = nullptr;
        mainVBO = nullptr;
        mainShader = nullptr;
        mainPlayer = nullptr;
        WorldGrid = nullptr;
        CubeRotating = nullptr;
        mainCamera = nullptr;
        particleTest = nullptr;
        text = nullptr;
    }
};