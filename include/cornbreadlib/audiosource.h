#pragma
#include <AL/al.h>
#include <AL/alc.h>

#include <cornbreadlib/audiobuffer.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class AudioSource {
    public:
    ALuint ID;
    AudioSource() {
        alGenSources(1, &ID);
    }

    void BindBufferToSource(AudioBuffer &BufferMain) {
        alSourcei(ID, AL_BUFFER, BufferMain.ID);
    }

    void PlaySource() {
        alSourcePlay(ID);
    }

    void SetSourcePosition(glm::vec3 Position) {
        alSource3f(ID, AL_POSITION, Position.x, Position.y, Position.z);
    }

    void IsSourceLooping(bool Looping) {
        alSourcei(ID, AL_LOOPING, Looping);
    }

    void SetSourcePitch(float Pitch) {
        alSourcef(ID, AL_PITCH, Pitch);
    }

    void SetSourceGain(float Gain) {
        alSourcef(ID, AL_GAIN, Gain);
    }

    void SetSourceVelocity(glm::vec3 Velocity) {
        alSource3f(ID, AL_VELOCITY, Velocity.x, Velocity.y, Velocity.z);
    }

    void SetSourceDirection(glm::vec3 NormalizedVec) {
        alSource3f(ID, AL_DIRECTION, NormalizedVec.x, NormalizedVec.y, NormalizedVec.z);
    }

    int GetSourceState() {
        int temp;
        alGetSourcei(ID, AL_SOURCE_STATE, &temp);
        return temp;
    }
};