#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "game.h"
#include "inputmanager.h"
#include <cornbreadlib/audiomanager.h>

extern int WIDTH, HEIGHT;
extern double CurrentTime, DeltaTime;
extern glm::mat4 Projection;
extern GameState game;
extern InputManager input;
extern AudioManager audio;