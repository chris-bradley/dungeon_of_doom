#include "stdio.h"
#include "SDL.h"

typedef struct {
    int samples_played;
    int length;
    Uint8 * stream;
} audio_state_t;

void play_sound(void * userdata, Uint8 * stream, int len) {
    audio_state_t * audio_state = (audio_state_t *) userdata;
    int silence;

    if (len > audio_state->length - audio_state->samples_played) {
        silence = len - audio_state->length + audio_state->samples_played;
        len -= silence;
    } else {
        silence = 0;
    }
    memcpy(stream, audio_state->stream + audio_state->samples_played, len);
    memset(stream + len, 0, silence);
    audio_state->samples_played += len;
}

Uint8 * pulse(int frequency, int length) {
    int samples_per_cycle = 44100 / frequency,
        pulse_chunk_length = samples_per_cycle / 2,
        stream_index = 0,
        wave_value = 255;

    Uint8 * stream = malloc(sizeof(Uint8) * length);

    while (stream_index < length) {
        if (pulse_chunk_length > length - stream_index) {
            pulse_chunk_length = length - stream_index;
        }
        memset(stream + stream_index, wave_value, pulse_chunk_length);
        stream_index += pulse_chunk_length;
        if (wave_value == 255) {
            wave_value = 0;
        } else {
            wave_value = 255;
        }
    }
    return stream;
}

int main(int argc, char* argv[]) {
    if (SDL_Init(SDL_INIT_AUDIO) != 0) {
        fprintf(stderr, "SDL Init Failure!: %s\n", SDL_GetError());
        return 1;
    }

    SDL_AudioSpec desired, *obtained = NULL;
    audio_state_t * audio_state = malloc(sizeof(audio_state_t));
    *audio_state = (audio_state_t) {
        .samples_played=0,
        .length=44100 * 4,
        .stream=pulse(440, 44100 * 4)
    };
    desired = (SDL_AudioSpec) {
        .freq=44100,
        .format=AUDIO_S8,
        .channels=1,
        .samples=4096,
        .callback=play_sound,
        .userdata=(void *) audio_state
    };
    obtained = (SDL_AudioSpec *) malloc(sizeof(SDL_AudioSpec));
    SDL_AudioDeviceID dev = SDL_OpenAudioDevice(
        NULL,
        0,
        &desired,
        obtained,
        SDL_AUDIO_ALLOW_ANY_CHANGE
    );
    if (dev == 0) {
        fprintf(stderr, "SDL_OpenAudioDevice() failure: %s\n", SDL_GetError());
    } else {
        if (desired.freq != obtained->freq) {
            fprintf(stderr, "obtained->freq is %i\n", obtained->freq);
        }
        if (desired.format != obtained->format) {
            fprintf(stderr, "obtained->format is %i\n", obtained->format);
        }
        if (desired.channels != obtained->channels) {
            fprintf(stderr, "obtained->channels is %i\n", obtained->channels);
        }
        if (desired.samples != obtained->samples) {
            fprintf(stderr, "obtained.samples is %i\n", obtained->samples);
        }
        fprintf(stderr, "obtained.silence is %i\n", obtained->silence);
        fprintf(stderr, "obtained.size is %i\n", obtained->size);
        SDL_PauseAudioDevice(dev, 0);
        SDL_Delay(5000);
        SDL_CloseAudioDevice(dev);
    }
    SDL_Quit();
    free(audio_state->stream);
    free(audio_state);
    free(obtained);
    return 0;
}
