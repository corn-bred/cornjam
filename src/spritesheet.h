#pragma once
#include <cornbreadlib/texturebuffer.h>
#include <cornbreadlib/vertexbuffer.h>
#include <cornbreadlib/shaders.h>
#include <cmath>

class Anim_FrameHandler {
    private:
    int CurrentFrame;
    float FrameTime; //Moduloed time, if greater than FrameDuration, reset
    public:

    Anim_FrameHandler(int startframe, int endframe, float frameduration, bool islooping = false){
        StartFrame = startframe;
        EndFrame = endframe;
        FrameDuration = frameduration;
        isLooping = islooping;

        CurrentFrame = StartFrame;
        FrameTime = 0.0f;
    }

    int StartFrame, EndFrame;
    float FrameDuration;
    bool isLooping;

    int GetCurrentFrame(float currentTime, float deltaTime) {

        if (isLooping) {

            int TotalFrames = EndFrame - StartFrame + 1;
            return (int(floor(currentTime / FrameDuration)) % TotalFrames) + StartFrame;

        } else {
            FrameTime += deltaTime;

            if (FrameTime >= FrameDuration) {
                float Loopovers = FrameTime / FrameDuration;
                int LoopoversFloor = floor(Loopovers);

                FrameTime = FrameDuration * (Loopovers - LoopoversFloor);

                if (CurrentFrame + LoopoversFloor <= EndFrame) CurrentFrame += LoopoversFloor;
                else CurrentFrame = EndFrame;
            }

            return CurrentFrame;
        }

    }

    void PlayAnimation() {
        CurrentFrame = StartFrame;
        FrameTime = 0.0f;
    }
};

class Anim_SpriteRenderer {
    
};