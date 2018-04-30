#include <SDL.h>
#include "dungeon_audio.h"

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

Uint8 * sawtooth(int frequency, int length) {
    int samples_per_cycle = 44100 / frequency,
        stream_index = 0,
        index;

    Uint8 * stream = malloc(sizeof(Uint8) * length);

    while (stream_index < length) {
        for (index = 0; index < samples_per_cycle; index += 1) {
            stream[stream_index] = 255 * index / samples_per_cycle;
            stream_index += 1;
            if (stream_index > length) {
                index = samples_per_cycle;
            }
        }
    }
    return stream;
}
