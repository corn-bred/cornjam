#include "game.h"
#include "scenes.h"
#include "globals.h"
#include "inputmanager.h"
#include <cornbreadlib/audiomanager.h>

void GameState::ChangeScene(Scene *newScene) {
    PendingScene.reset(newScene); //new ownershippp
}

void GameState::ApplyPendingScene() {
    if (PendingScene) {

        if (CurrentScene) CurrentScene->Exit();

        CurrentScene = std::move(PendingScene);

        if (CurrentScene) CurrentScene->Init();

    }
}

void GameState::Init() {
    input.BindAction(Action::Jump, GLFW_KEY_UP);
    input.BindAction(Action::MoveLeft, GLFW_KEY_LEFT);
    input.BindAction(Action::MoveRight, GLFW_KEY_RIGHT);
    input.BindAction(Action::Pause, GLFW_KEY_ENTER);
    ChangeScene(new PlayingScene());
}

void GameState::Play(GLFWwindow *window) {
    glfwPollEvents();
    
    audio.Update();
    input.Update(window);
    
    ApplyPendingScene();

    if (CurrentScene) {
        CurrentScene->Update();
    }

    if (CurrentScene) {
        CurrentScene->Render();
    }

    glfwSwapBuffers(window);
}