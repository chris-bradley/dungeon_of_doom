#include "stdio.h"
#include "SDL.h"

typedef struct {
    int samples_played;
    int frequency;
    int length;
} audio_state_t;

void pulse(void * userdata, Uint8 * stream, int len) {
    audio_state_t * audio_state = (audio_state_t *) userdata;
    int samples_per_cycle = 44100 / audio_state->frequency,
        pulse_chunk_length = samples_per_cycle / 2,
        cycle_position = audio_state->samples_played % samples_per_cycle,
        stream_index = 0,
        wave_value,
        silence;

    if (len > audio_state->length - audio_state->samples_played) {
        silence = len - audio_state->length + audio_state->samples_played;
        len -= silence;
    } else {
        silence = 0;
    }
    if (len > 0) {
        if (cycle_position < pulse_chunk_length) {
            memset(stream, 255, pulse_chunk_length - cycle_position);
            stream_index += pulse_chunk_length - cycle_position;
            cycle_position = pulse_chunk_length;
        }
        memset(stream + stream_index, 0, samples_per_cycle - cycle_position);
        stream_index += samples_per_cycle - cycle_position;
        wave_value = 255;
    }

    while (stream_index < len) {
        if (pulse_chunk_length > len - stream_index) {
            pulse_chunk_length = len - stream_index;
        }
        memset(stream + stream_index, wave_value, pulse_chunk_length);
        stream_index += pulse_chunk_length;
        if (wave_value == 255) {
            wave_value = 0;
        } else {
            wave_value = 255;
        }
    }
    memset(stream + stream_index, 0, silence);
    audio_state->samples_played += len;
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
        .frequency=440,
        .length=44100 * 4
    };
    desired = (SDL_AudioSpec) {
        .freq=44100,
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
            fprintf(stderr, "obtained.samples is %i\n", obtained->samples);
        }
        fprintf(stderr, "obtained.silence is %i\n", obtained->silence);
        fprintf(stderr, "obtained.size is %i\n", obtained->size);
        SDL_PauseAudioDevice(dev, 0);
        SDL_Delay(5000);
        SDL_CloseAudioDevice(dev);
    }
    SDL_Quit();
    free(audio_state);
    free(obtained);
    return 0;
}
