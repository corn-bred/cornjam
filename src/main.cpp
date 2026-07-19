#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image/stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp> 

#include <AL/al.h>
#include <AL/alc.h>

#include <cornbreadlib/utility.h>
#include <cornbreadlib/vertexbuffer.h>
#include <cornbreadlib/shaders.h>
#include <cornbreadlib/texturebuffer.h>

#define DR_WAV_IMPLEMENTATION
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

using namespace std;

int WIDTH = 1200;
int HEIGHT = 800;
double DeltaTime, LastTime;
unsigned int FPSCounter, ShownFPS;
int FrameIndex = 0;

Player mainPlayer(glm::vec3(WIDTH / 2.0f, HEIGHT / 2.0f, 1.0), 0.0f, glm::vec2(150.0f, 1700.0f), glm::vec2(0.0), glm::vec2(1000.0f, 30000.0f), glm::vec2(0.8f), glm::vec2(0.92f), glm::vec2(50.0f), glm::vec2(1.0), glm::vec2(0.0), glm::vec2(50.0), glm::vec2(1.0f, 0.5f), 0.1);

void framebufferSizeCallback(GLFWwindow *window, int width, int height) {
    WIDTH = width;
    HEIGHT = height;
    glViewport(0, 0, width, height);
}

float LastX = -1.0, LastY = -1.0;

void mouseCallback(GLFWwindow *window, double xpos, double ypos) {

}

void scrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
}

int main() {
    //Initialization

    //GLFW & GLAD
    if (!glfwInit()) {
        cerr << "GLFW initialization failure\n";
        return 1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
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

    //OpenAL

    ALCdevice *AudioDevice = alcOpenDevice(NULL);
    if (!AudioDevice) {
        cerr << "Audio device creation failure\n";
        glfwTerminate();
        return 1;
    }

    ALCcontext *AudioContext = alcCreateContext(AudioDevice, NULL);
    if (!AudioContext) {
        cerr << "Audio context creation failure\n";
        glfwTerminate();
        return 1;
    }

    alcMakeContextCurrent(AudioContext);

    //OpenAL Listener position
    alListener3f(AL_POSITION, 0.0, 0.0, 0.0);
    alListener3f(AL_VELOCITY, 0.0f, 0.0f, 0.0f);
    alListener3f(AL_ORIENTATION, 0.0f, 0.0f, -1.0f);

    ALuint SourceMain, AudioBuffer; //Source is where the sound comes from, and a buffer is the data of the audio

    alGenBuffers(1, &AudioBuffer);
    alGenSources(1, &SourceMain);

    //Get audio data

    unsigned int Channels, SampleRate;
    drwav_uint64 TotalPCMFrameCount;
    drwav_int16 *SampleData = drwav_open_file_and_read_pcm_frames_s16("res/audio/laser_slam.wav", &Channels, &SampleRate, &TotalPCMFrameCount, NULL);
    if (SampleData == NULL) {
        cerr << "PCM frame read failure\n";
        
        alcMakeContextCurrent(0);
        alcDestroyContext(AudioContext);
        alcCloseDevice(AudioDevice);
        glfwTerminate();

        return 1;
    }

    //Get format
    ALenum format;
    if (Channels == 1) {
        format = AL_FORMAT_MONO16;
    } else if (Channels == 2) {
        format = AL_FORMAT_STEREO16;
    } else {
        cerr << "OpenAL does not support more than 2 channels (Mono or Stereo) for playback\n";
        
        alcMakeContextCurrent(0);
        alcDestroyContext(AudioContext);
        alcCloseDevice(AudioDevice);
        glfwTerminate();
        drwav_free(SampleData, NULL);

        return 1;
    }
    //Get size allocation
    size_t TotalPCMByteCount = TotalPCMFrameCount * Channels * sizeof(drwav_int16);
    //Add the data
    alBufferData(AudioBuffer, format, SampleData, TotalPCMByteCount, SampleRate);
    //Freeing memory
    drwav_free(SampleData, NULL);

    //Attach to the source
    alSourcei(SourceMain, AL_BUFFER, AudioBuffer);

    VertexBuffer mainVBO(quadData, sizeof(quadData), GL_STATIC_DRAW);
    mainVBO.addAttribute(0, 2, GL_FLOAT, 4, 0);
    mainVBO.addAttribute(1, 2, GL_FLOAT, 4, 2);

    Shader mainShader("src/shaders/main.vert", "src/shaders/main.frag");

    TextureBuffer playerTexture("res/img/goofy ahh thing.png");

    InputManager playerInput(30.0, 0.1);
    playerInput.BindAction(Action::Jump, GLFW_KEY_UP);
    playerInput.BindAction(Action::MoveLeft, GLFW_KEY_LEFT);
    playerInput.BindAction(Action::MoveRight, GLFW_KEY_RIGHT);

    GridSpace gridTest(glm::vec2(WIDTH/10, HEIGHT/10), glm::vec3(0.0));

    gridTest.Data = {
    0,0,1,0,2,0,3,0,4,0,5,0,6,0,7,0,8,0,9,0,
    -1,0,-1,1,-1,2,-1,3,-1,4,-1,5,-1,6,-1,7,-1,8,-1,9,-1,10,
    10,0,10,1,10,2,10,3,10,4,10,5,10,6,10,7,10,8,10,9,10,10
    };

    Animation CubeRotating(0, 49, 0.04167, true, "res/img/rotatingcube.png", 10, 5, GL_CLAMP_TO_BORDER,GL_CLAMP_TO_BORDER, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR, false); // 1/24 is irrational
    
    Camera2D cameraTest(glm::vec3(0.0, 0.0, 0.0), glm::vec2(1.0), 0.0);

    glm::vec2 OldDifference = glm::vec2(0.0);

    Particles particleTest("src/shaders/particles.comp", 50, glm::vec2(WIDTH / 2, HEIGHT / 2), 2.0, 10.0, 100.0, true, true);

    particleTest.RenderSolidColourState(glm::vec3(1.0, 0.8, 0.6));

    TextRenderer text("res/img/sans-serif.png", 15, 15, 72, 90, true);

    glm::mat4 Projection = glm::ortho(0.0f, (float)WIDTH, 0.0f, (float)HEIGHT, -100.0f, 100.0f);

    while(!glfwWindowShouldClose(window)) { 
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        double CurrentTime = glfwGetTime();
        DeltaTime = CurrentTime - LastTime;
        if (floor(CurrentTime) != floor(LastTime)) {
            stringstream titlestring;
            titlestring << "Cornbread Program (FPS: " << FPSCounter << ")";
            glfwSetWindowTitle(window, titlestring.str().c_str()); 
            FPSCounter = 0;
            alSourcePlay(SourceMain);
        }
        LastTime = CurrentTime;

        glfwPollEvents();
        playerInput.Update(window, DeltaTime);
        mainPlayer.KeyboardUpdate(playerInput, DeltaTime);

        //Camera updates
        glm::vec3 NewPos = cameraTest.CameraToEntity(mainPlayer, WIDTH, HEIGHT, lerpToTime(0.1, DeltaTime));
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

        CubeRotating.Update(CurrentTime, DeltaTime);

        glm::mat4 animModel = glm::mat4(1.0);
        animModel = glm::scale(animModel, glm::vec3(100, 100, 0));
        CubeRotating.RenderSprite(mainShader, mainVBO, animModel, View, Projection);

        particleTest.Update(DeltaTime, glm::vec2(0.0, -100.0), CurrentTime);
        particleTest.Render(View, Projection);

        glm::mat4 TextView = glm::mat4(1.0);

        text.RenderText("Test -=\\]{\n!@#$%^&*()", glm::vec2(0, HEIGHT - 90), 1.0, TextView, Projection);

        glfwSwapBuffers(window);

        FPSCounter++;
    }
    alcMakeContextCurrent(0);
    alcDestroyContext(AudioContext);
    alcCloseDevice(AudioDevice);
    glfwTerminate();
    return 0;
}