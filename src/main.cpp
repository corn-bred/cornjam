#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp> 
#include <cornbreadlib/primitives.h>
#include <cornbreadlib/vertexbuffer.h>
#include <cornbreadlib/shaders.h>
#include <cornbreadlib/texturebuffer.h>
#include "entity.h"
#include "grid.h"
#include "collision.h"
#include "spritesheet.h"
#include "camera2D.h"

using namespace std;

int WIDTH = 1200;
int HEIGHT = 800;
double DeltaTime, LastFrame;
unsigned int FPSCounter, ShownFPS;
int FrameIndex = 0;

Player mainPlayer(glm::vec3(WIDTH / 2.0f, HEIGHT / 2.0f, 1.0), 0.0f, glm::vec2(150.0f, 1700.0f), glm::vec2(0.0), glm::vec2(1000.0f, 30000.0f), glm::vec2(0.8f), glm::vec2(0.92f), glm::vec2(50.0f), glm::vec2(1.0), glm::vec2(0.0), glm::vec2(50.0), glm::vec2(1.0f, 0.5f), 0.1);

void framebufferSizeCallback(GLFWwindow *window, int width, int height) {
    WIDTH = width;
    HEIGHT = height;
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window) {
    if(glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
        if (mainPlayer.CollisionAxes[1] != 0)
        mainPlayer.Velocity.x += -mainPlayer.Speed.x * (DeltaTime * 60);
        else {
        mainPlayer.Velocity.x += -mainPlayer.Speed.x * (1 - (mainPlayer.AirResistance.x / 2)) * (DeltaTime * 60);
        }
    }

    if(glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
        if (mainPlayer.CollisionAxes[1] != 0)
        mainPlayer.Velocity.x += mainPlayer.Speed.x * (DeltaTime * 60);
        else
        mainPlayer.Velocity.x += mainPlayer.Speed.x * (1 - (mainPlayer.AirResistance.x / 2)) * (DeltaTime * 60);
    }

    if(glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
        if (mainPlayer.CollisionAxes[1] == -1)
        mainPlayer.Velocity.y = mainPlayer.Speed.y;
        else if (mainPlayer.CollisionAxes[0] != 0 && mainPlayer.CollisionAxes[1] == 0) { //To stop climb bugging
            mainPlayer.Velocity.y = mainPlayer.Speed.y * mainPlayer.MinijumpPushoff.y;
            mainPlayer.Velocity.x = mainPlayer.TerminalSpeed.x * -mainPlayer.CollisionAxes[0] * mainPlayer.MinijumpPushoff[0];
        }
    }

    if(glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
        mainPlayer.Velocity.y = -mainPlayer.Speed.y;
    }

    
}

float LastX = -1.0, LastY = -1.0;

void mouseCallback(GLFWwindow *window, double xpos, double ypos) {

}

void scrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
}

int main() {
    if (!glfwInit()) {
        cerr << "GLFW initialization failure\n";
        return 1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "Game", NULL, NULL);

    if (!window) {
        cerr << "GLFW window creation failure\n";
        glfwTerminate();
        return 1;
    }

    glfwMakeContextCurrent(window);

    glfwSwapInterval(0);

    GLFWmonitor *monitor = glfwGetPrimaryMonitor();
    GLFWvidmode *mode = const_cast<GLFWvidmode*>(glfwGetVideoMode(monitor));
    glfwSetWindowPos(window, (mode->width - WIDTH)/2, (mode->height - HEIGHT)/2);

    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);  
    glfwSetCursorPosCallback(window, mouseCallback);

    glfwSetScrollCallback(window, scrollCallback);

    if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) {
        cerr << "GLAD initialization failure\n";
        glfwTerminate();
        return 1;
    }

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    VertexBuffer mainVBO(quadData, sizeof(quadData), GL_STATIC_DRAW);
    mainVBO.addAttribute(0, 4, 2, GL_FLOAT, sizeof(float), 0);
    mainVBO.addAttribute(1, 4, 2, GL_FLOAT, sizeof(float), 2);

    Shader mainShader("src/shaders/main.vert", "src/shaders/main.frag");

    TextureBuffer playerTexture("res/goofy ahh thing.png");

    GridSpace gridTest(glm::vec2(WIDTH/10, HEIGHT/10), glm::vec3(0.0));

    gridTest.Data = {
    0,0,1,0,2,0,3,0,4,0,5,0,6,0,7,0,8,0,9,0,
    -1,0,-1,1,-1,2,-1,3,-1,4,-1,5,-1,6,-1,7,-1,8,-1,9,-1,10,
    10,0,10,1,10,2,10,3,10,4,10,5,10,6,10,7,10,8,10,9,10,10
    };

    Animation CubeRotating(0, 49, 0.04167, true, "res/rotatingcube.png", 10, 5, GL_CLAMP_TO_BORDER,GL_CLAMP_TO_BORDER, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR, false); // 1/24 is irrational
    
    Camera2D cameraTest(glm::vec3(0.0, 0.0, 0.0), glm::vec2(1.0), 0.0);

    glm::vec2 OldDifference = glm::vec2(0.0);
    while(!glfwWindowShouldClose(window)) { 
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        double currentframe = glfwGetTime();
        DeltaTime = currentframe - LastFrame;
        if (floor(currentframe) != floor(LastFrame)) {
            stringstream titlestring;
            titlestring << "Cornbread Program (FPS: " << FPSCounter << ")";
            glfwSetWindowTitle(window, titlestring.str().c_str()); 
            FPSCounter = 0;
        }
        LastFrame = currentframe;

        glfwPollEvents();
        processInput(window);

        //Camera updates
        float Smoothness = -60.0 * log(1.0 - 0.1);
        glm::vec3 NewPos = cameraTest.CameraToEntity(mainPlayer, WIDTH, HEIGHT, 1.0 - glm::exp(-Smoothness * DeltaTime));
        glm::vec2 Difference = glm::vec2(NewPos.x - cameraTest.Position.x, NewPos.y - cameraTest.Position.y);
        //cameraTest.SetDirection(cameraTest.GetDirectionRad() + 0.5 * DeltaTime);
        cameraTest.Scale = glm::vec2(1.0 + abs( mainPlayer.Velocity.x * 0.0001) );
        cameraTest.Position = NewPos;

        OldDifference = Difference;

        //Updates

        mainPlayer.VeloUpdate(DeltaTime, gridTest, 300, 6000.0f);

        mainShader.use();
        glm::mat4 Model = mainPlayer.GetTransformMatrix();
        mainShader.setMat4("model", Model);
        //std::cout << "Model matrix:\n" << glm::to_string(Model) << std::endl;

        glm::mat4 View = cameraTest.GetViewMatrix(WIDTH, HEIGHT);
        mainShader.setMat4("view", View);

        glm::mat4 Projection = glm::ortho(0.0f, (float)WIDTH, 0.0f, (float)HEIGHT, -100.0f, 100.0f);
        mainShader.setMat4("projection", Projection);
        //cout << "Projection matrix:\n" << glm::to_string(Projection) << endl;
        mainShader.setBool("isSolidColour", false);
        mainShader.setBool("isAnimation", false);

        playerTexture.bindTexture(0);

        mainVBO.bind();

        glDrawArrays(GL_TRIANGLES, 0, 6);

        gridTest.RenderAll(mainShader, mainVBO, View, Projection);

        CubeRotating.Update(currentframe, DeltaTime);

        glm::mat4 animModel = glm::mat4(1.0);
        animModel = glm::scale(animModel, glm::vec3(100, 100, 0));
        CubeRotating.RenderSprite(mainShader, mainVBO, animModel, View, Projection);

        glfwSwapBuffers(window);

        FPSCounter++;
    }
    glfwTerminate();
    return 0;
}