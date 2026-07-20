#include <misc/globals.h>
#include "game.h"
#include <cornjam/inputmanager.h>
#include <cornbreadlib/audiomanager.h>
#include <glm/gtc/matrix_transform.hpp>

glm::mat4 Projection = glm::ortho(0.0f, (float)WIDTH, 0.0f, (float)HEIGHT);

GameState game;
InputManager input(30.0f, 0.1f);
AudioManager audio; 