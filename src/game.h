#pragma once
#include "scenes.h"
#include "globals.h"
#include <memory>

class GameState {
    private:
    std::unique_ptr<Scene> CurrentScene;
    std::unique_ptr<Scene> PendingScene;

    bool isRunning = true;

    public:

    void ChangeScene(Scene *newScene) {
        PendingScene.reset(newScene); //new ownershippp
    }

    void ApplyPendingScene() {
        if (PendingScene) {
            if (CurrentScene) CurrentScene->Exit();

            CurrentScene = std::move(PendingScene);

            if (CurrentScene) CurrentScene->Init();
        }
    }

    void Init() {
        ChangeScene(new PlayingScene());
    }

    void Play(GLFWwindow *window) {
        audio.Update();
        input.Update();
        
        ApplyPendingScene();

        if (CurrentScene) {
            CurrentScene->Update();
        }

        if (CurrentScene) {
            CurrentScene->Render();
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
};