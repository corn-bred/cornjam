#pragma once
#include <GLFW/glfw3.h>
#include <memory>

class InputManager {
    private:
    float _KeyTimer[GLFW_KEY_LAST + 1];
    const float c_MaximumTime = 1.0;
    public:

    InputManager(float maximumTime) : c_MaximumTime(maximumTime) {
        memset(_KeyTimer, -0.001, sizeof(_KeyTimer));
    }

    void Update(GLFWwindow *window, float deltaTime) {

        for(int i = 0; i <= GLFW_KEY_LAST; i++) {
            int isKeyPressed = glfwGetKey(window, i);

            if(isKeyPressed == GLFW_PRESS) { //POSITIVE

                if (_KeyTimer > 0) //Was it already pressed in the previous frame?
                    _KeyTimer[i] += deltaTime;
                else
                    _KeyTimer[i] = deltaTime;

                if (_KeyTimer[i] > c_MaximumTime)
                    _KeyTimer[i] = c_MaximumTime;

            } else { //NEGATIVE

                if (_KeyTimer < 0) //Was it already released in the previous frame?
                    _KeyTimer[i] -= deltaTime;
                else
                    _KeyTimer[i] = -deltaTime;

                if (_KeyTimer[i] < c_MaximumTime)
                    _KeyTimer[i] = -c_MaximumTime;

            }

        }

    }

    inline float GetKeyTime(int Key) { //The longer a key is pressed, the higher the number
        return _KeyTimer[Key];
    }

    inline bool isKeyPressed(int Key) {
        return (_KeyTimer[Key] > 0);
    }

    inline bool isKeyReleased(int Key) {
        return (_KeyTimer[Key] < 0);
    }

    inline bool wassKeyPressedRecently(int Key, float Time) { //Checks whether the key was pressed within Time or shorter
        return(_KeyTimer[Key] < Time && _KeyTimer[Key] > 0);
    }

    inline bool wasKeyReleasedRecently(int Key, float Time) { //Checks whether the key was released within Time or shorter
        return(_KeyTimer[Key] > -Time && _KeyTimer[Key] < 0);
    }

    inline bool isKeyPressedFor(int Key, float Time) { //Checks whether the key was pressed for Time or longer
        return(_KeyTimer[Key] > Time);
    }

    inline bool isKeyReleasedFor(int Key, float Time) { //Checks whether the key was released for Time or longer
        return(_KeyTimer[Key] < -Time);
    }
};