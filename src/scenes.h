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
    void Init() override;
    void Update() override;
    void Render() override;
    void Exit() override;
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

    void Init() override;

    void Update() override;

    void Render() override;

    void Exit() override;
};