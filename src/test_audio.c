#include "stdio.h"
#include "SDL.h"

typedef struct {
    int remaining_pulse;
    int current_wave_value;
    int frequency;
} audio_state_t;

void pulse(void * userdata, Uint8 * stream, int len) {
    audio_state_t * audio_state = (audio_state_t *) userdata;
    int samples_per_cycle = 11025 / audio_state->frequency,
        pulse_chunk_length = samples_per_cycle / 2,
        stream_index = audio_state->remaining_pulse,
        wave_value = audio_state->current_wave_value;
    memset(stream, wave_value, stream_index);
    while (stream_index < len) {
        if (wave_value == 255) {
            wave_value = 0;
        } else {
            wave_value = 255;
        }
        if (pulse_chunk_length >= len - stream_index) {
            audio_state->remaining_pulse =
                pulse_chunk_length - len + stream_index;
            audio_state->current_wave_value = wave_value;
            pulse_chunk_length = len - stream_index;
        }
        memset(stream + stream_index, wave_value, pulse_chunk_length);
        stream_index += pulse_chunk_length;
    }
}

int main(int argc, char* argv[]) {
    if (SDL_Init(SDL_INIT_AUDIO) != 0) {
        fprintf(stderr, "SDL Init Failure!: %s\n", SDL_GetError());
        return 1;
    }

    SDL_AudioSpec desired, *obtained = NULL;
    audio_state_t * audio_state = malloc(sizeof(audio_state_t));
    *audio_state = (audio_state_t) {
        .current_wave_value = 0,
        .remaining_pulse = 0,
        .frequency=440
    };
    desired = (SDL_AudioSpec) {
        .freq=11025,
        .format=AUDIO_S8,
        .channels=1,
        .samples=4096,
        .callback=pulse,
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
            fprintf(stderr, "obstained.samples is %i\n", obtained->samples);
        }
        SDL_PauseAudioDevice(dev, 0);
        SDL_Delay(5000);
        SDL_CloseAudioDevice(dev);
    }
    SDL_Quit();
    free(audio_state);
    free(obtained);
    return 0;
}
