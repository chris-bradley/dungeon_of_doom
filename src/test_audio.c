#include "stdio.h"
#include "SDL.h"
#include "dungeon_audio.h"

int main(int argc, char* argv[]) {
    if (SDL_Init(SDL_INIT_AUDIO) != 0) {
        fprintf(stderr, "SDL Init Failure!: %s\n", SDL_GetError());
        return 1;
    }

    audio_state_t * audio_state = init_audio_state(2);
    Uint8 * stream = pulse(440, 44100 * 4);
    volume_filter(stream, 44100 * 4, 22050, 22050, 8, 22050);
    stream_queue_t * main_stream = *(audio_state->streams);

    stream_queue_enqueue(
        main_stream,
        stream,
        44100 * 4
    );
    stream_queue_enqueue(
        main_stream,
        sawtooth(440, 44100 * 4),
        44100 * 4
    );
    stream_queue_enqueue(
        main_stream,
        sine_wave(440, 44100 * 4),
        44100 * 4
    );
    stream_queue_enqueue(
        main_stream,
        noise(440, 44100 * 4),
        44100 * 4
    );
    SDL_PauseAudioDevice(audio_state->device, 0);
    SDL_Delay(15000);
    destroy_audio_state(audio_state);
    SDL_Quit();
    return 0;
}
