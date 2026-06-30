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

using namespace std;

int WIDTH = 1200;
int HEIGHT = 800;
double DeltaTime, LastFrame;
unsigned int FPSCounter, ShownFPS;
int FrameIndex = 0;

Player mainPlayer(glm::vec3(WIDTH / 2.0f, HEIGHT / 2.0f, 1.0), 0.0f, glm::vec2(500.0f, 500.0f), glm::vec2(0.0f), glm::vec2(0.8f), glm::vec2(50.0f));

void framebufferSizeCallback(GLFWwindow *window, int width, int height) {
    WIDTH = width;
    HEIGHT = height;
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window) {
    if(glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
        mainPlayer.Velocity.y = mainPlayer.Speed.y;
    if(glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
        mainPlayer.Velocity.y = -mainPlayer.Speed.y;
    if(glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
        mainPlayer.Velocity.x = -mainPlayer.Speed.x;
    if(glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
        mainPlayer.Velocity.x = mainPlayer.Speed.x;
}

float LastX = -1.0, LastY = -1.0;

void mouseCallback(GLFWwindow *window, double xpos, double ypos) {

}

void scrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
}

int main() {
    cout << "hi";
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

    GridSpace gridTest(glm::vec2(30.0), glm::vec3(0.0));
    gridTest.AddSquare(0, 0);
    gridTest.AddSquare(0, 1);
    gridTest.AddSquare(1, 0);
    gridTest.AddSquare(1, 1);

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

        mainPlayer.VeloUpdate(DeltaTime);

        mainShader.use();
        glm::mat4 Model = mainPlayer.GetTransformMatrix();
        mainShader.setMat4("model", Model);
        //std::cout << "Model matrix:\n" << glm::to_string(Model) << std::endl;

        glm::mat4 View = glm::mat4(1.0f);
        mainShader.setMat4("view", View);

        glm::mat4 Projection = glm::ortho(0.0f, (float)WIDTH, 0.0f, (float)HEIGHT, -100.0f, 100.0f);
        mainShader.setMat4("projection", Projection);
        //cout << "Projection matrix:\n" << glm::to_string(Projection) << endl;

        playerTexture.bindTexture(0);

        mainVBO.bind();

        glDrawArrays(GL_TRIANGLES, 0, 6);

        gridTest.RenderAll(mainShader, mainVBO, View, Projection);

        glfwSwapBuffers(window);

        FPSCounter++;
    }
    glfwTerminate();
    return 0;
}