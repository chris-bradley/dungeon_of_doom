#include "stdio.h"
#include "SDL.h"
#include "dungeon_audio.h"

int main(int argc, char* argv[]) {
    if (SDL_Init(SDL_INIT_AUDIO) != 0) {
        fprintf(stderr, "SDL Init Failure!: %s\n", SDL_GetError());
        return 1;
    }

    SDL_AudioSpec desired, *obtained = NULL;
    audio_state_t * audio_state = malloc(sizeof(audio_state_t));
    *audio_state = (audio_state_t) {
        .streams = malloc(sizeof(stream_queue_t))
    };
    *audio_state->streams = (stream_queue_t) {
        .length=0,
        .first_node=NULL,
        .last_node=NULL
    };
    stream_queue_enqueue(
        audio_state->streams,
        pulse(440, 44100 * 4),
        44100 * 4
    );
    stream_queue_enqueue(
        audio_state->streams,
        sawtooth(440, 44100 * 4),
        44100 * 4
    );
    stream_queue_enqueue(
        audio_state->streams,
        sine_wave(440, 44100 * 4),
        44100 * 4
    );
    stream_queue_enqueue(
        audio_state->streams,
        noise(440, 44100 * 4),
        44100 * 4
    );
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
        SDL_Delay(15000);
        SDL_CloseAudioDevice(dev);
    }
    SDL_Quit();
    while (audio_state->streams->first_node != NULL) {
        stream_queue_clear_first(audio_state->streams);
    }
    free(audio_state->streams);
    free(audio_state);
    free(obtained);
    return 0;
}
