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

#include <cornjam/entity.h>
#include <cornjam/grid.h>
#include <cornjam/collision.h>
#include <cornjam/spritesheet.h>
#include <cornjam/camera2D.h>
#include <cornjam/particles.h>
#include <cornjam/inputmanager.h>
#include <cornjam/bitmaptext.h>
#include <misc/globals.h>
class Scene {
    public:
    virtual ~Scene() = default;

    virtual void Init() {}

    virtual void Update() = 0;

    virtual void Render() = 0;

    virtual void Exit() {}
};

//Custom scenes written below

class PlayingScene : public Scene {
    public:

    void Init() override;

    void Update() override;

    void Render() override;

    void Exit() override;
};