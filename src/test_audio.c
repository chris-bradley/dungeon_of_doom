#include "stdio.h"
#include "SDL.h"
#include "dungeon_audio.h"

int main(int argc, char* argv[]) {
    int freq;
    if (SDL_Init(SDL_INIT_AUDIO) != 0) {
        fprintf(stderr, "SDL Init Failure!: %s\n", SDL_GetError());
        return 1;
    }

    audio_state_t * audio_state = init_audio_state(2);
    freq = audio_state->audio_spec->freq;
    Uint8 * stream = pulse(440, freq * 4, audio_state->audio_spec);
    volume_filter(stream, freq * 4, freq / 2, freq / 2, 8, freq / 2);
    stream_queue_t * main_stream = *(audio_state->streams);
    stream_queue_t * second_stream = *(audio_state->streams + 1);


    stream_queue_enqueue(
        main_stream,
        stream,
        freq * 4
    );
    stream_queue_enqueue(
        main_stream,
        sawtooth(440, freq * 4, audio_state->audio_spec),
        freq * 4
    );
    stream_queue_enqueue(
        main_stream,
        sine_wave(440, freq * 4, audio_state->audio_spec),
        freq * 4
    );
    stream_queue_enqueue(
        main_stream,
        noise(440, freq * 4, audio_state->audio_spec),
        freq * 4
    );
    SDL_PauseAudioDevice(audio_state->device, 0);
    SDL_Delay(15000);
    destroy_audio_state(audio_state);
    SDL_Quit();
    return 0;
}
