#include "game.h"
#include "scenes.h"
#include <misc/globals.h>
#include <cornjam/inputmanager.h>
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