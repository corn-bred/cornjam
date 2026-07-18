#pragma once
#include <AL/al.h>
#include <AL/alc.h>
#include <dr_libs/dr_wav.h>
#include <iostream>

class AudioBuffer {
    public:
    ALuint ID;
    AudioBuffer(const char *AudioPath) {
        //Source is where the sound comes from, and a buffer is the data of the audio
        alGenBuffers(1, &ID);

        //Get audio data

        unsigned int Channels, SampleRate;
        drwav_uint64 TotalPCMFrameCount;
        drwav_int16 *SampleData = drwav_open_file_and_read_pcm_frames_s16(AudioPath, &Channels, &SampleRate, &TotalPCMFrameCount, NULL);
        if (SampleData == NULL) {
            std::cerr << "PCM frame read failure\n";

            alcMakeContextCurrent(0);
            
            return;
        }

        //Get format
        ALenum format;
        if (Channels == 1) {
            format = AL_FORMAT_MONO16;
        } else if (Channels == 2) {
            format = AL_FORMAT_STEREO16;
        } else {
            std::cerr << "OpenAL does not support more than 2 channels (Mono or Stereo) for playback\n";

            alcMakeContextCurrent(0);
            drwav_free(SampleData, NULL);

            return;
        }
        //Get size allocation
        size_t TotalPCMByteCount = TotalPCMFrameCount * Channels * sizeof(drwav_int16);
        //Add the data
        alBufferData(ID, format, SampleData, TotalPCMByteCount, SampleRate);
        //Freeing memory
        drwav_free(SampleData, NULL);
    }
};