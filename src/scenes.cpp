#include "scenes.h"
#include "game.h"          
#include <cornjam/inputmanager.h>  
#include <misc/globals.h>

//PlayingScene

void PlayingScene::Init() {
}

void PlayingScene::Update() {
}

void PlayingScene::Render() {
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}

void PlayingScene::Exit() {
}