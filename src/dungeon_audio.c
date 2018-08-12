#include <SDL.h>
#include "dungeon_audio.h"

void stream_queue_enqueue(stream_queue_t * stream_queue, Uint8 * stream,
                          int length) {
    stream_queue_node_t * stream_node = malloc(sizeof(stream_queue_node_t));
    *stream_node = (stream_queue_node_t) {
        .stream = stream,
        .length = length,
        .next_node = NULL
    };
    if (stream_queue->last_node != NULL) {
        stream_queue->last_node->next_node = stream_node;
    }
    stream_queue->last_node = stream_node;
    if (stream_queue->first_node == NULL) {
        stream_queue->first_node = stream_node;
    }
}

void stream_queue_clear_first(stream_queue_t * stream_queue) {
    stream_queue_node_t * first_node = stream_queue->first_node;
    stream_queue->first_node = first_node->next_node;
    if (stream_queue->first_node == NULL) {
        stream_queue->last_node = NULL;
    }
    free(first_node->stream);
    free(first_node);
}

void play_sound(void * userdata, Uint8 * stream, int len) {
    audio_state_t * audio_state = (audio_state_t *) userdata;
    stream_queue_t * cur_stream;
    stream_queue_node_t * cur_node;
    int silence, node_remaining, index;

    SDL_memset(stream, 0, len);

    for (index = 0; index < audio_state->num_stream_queues; index += 1) {
        silence = len;
        cur_stream = *(audio_state->streams + index);
        while (cur_stream->first_node != NULL && silence > 0) {
            cur_node = cur_stream->first_node;
            node_remaining = cur_node->length - cur_node->samples_played;
            if (node_remaining > silence) {
                node_remaining = silence;
            }
            SDL_MixAudioFormat(
                stream + len - silence,
                cur_node->stream + cur_node->samples_played,
                audio_state->audio_spec->format,
                node_remaining,
                SDL_MIX_MAXVOLUME
            );
            silence -= node_remaining;
            cur_node->samples_played += node_remaining;
            if (cur_node->samples_played == cur_node->length) {
                stream_queue_clear_first(cur_stream);
            }
        }
    }
}

Uint8 * pulse(int frequency, int length) {
    Uint8 * stream = malloc(sizeof(Uint8) * length);
    if (frequency == 0) {
        memset(stream, 0, length);
        return stream;
    }

    int samples_per_cycle = 44100 / frequency,
        pulse_chunk_length = samples_per_cycle / 2,
        stream_index = 0,
        wave_value = 255;

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
    Uint8 * stream = malloc(sizeof(Uint8) * length);
    if (frequency == 0) {
        memset(stream, 0, length);
        return stream;
    }

    int samples_per_cycle = 44100 / frequency,
        stream_index = 0,
        index;

    while (stream_index < length) {
        for (index = 0; index < samples_per_cycle; index += 1) {
            stream[stream_index] = 255 * index / samples_per_cycle;
            stream_index += 1;
            if (stream_index >= length) {
                index = samples_per_cycle;
            }
        }
    }
    return stream;
}

#define PI 3.14159265358979323846

Uint8 * sine_wave(int frequency, int length) {
    Uint8 * stream = malloc(sizeof(Uint8) * length);
    if (frequency == 0) {
        memset(stream, 0, length);
        return stream;
    }

    int samples_per_cycle = 44100 / frequency,
        stream_index = 0,
        index;

    while (stream_index < length) {
        for (index = 0; index < samples_per_cycle; index += 1) {
            stream[stream_index] = 127 + (
                127 * sin(index * 2 * PI / samples_per_cycle)
            );
            stream_index += 1;
            if (stream_index > length) {
                index = samples_per_cycle;
            }
        }
    }
    return stream;
}

Uint8 * noise(int frequency, int length) {
    Uint8 * stream = malloc(sizeof(Uint8) * length);
    if (frequency == 0) {
        memset(stream, 0, length);
        return stream;
    }

    int samples_per_shift = 44100 / frequency / 4,
        stream_index = 0,
        index,
        bits_to_feed;
    Uint8 wave_val;

    wave_val = 0;
    bits_to_feed = rand() % 7;
    for (
            stream_index = 0;
            stream_index < length;
            stream_index += samples_per_shift
    ) {
        wave_val = wave_val << 1;
        if ((wave_val & 127) == 0 || bits_to_feed > 0) {
            if (bits_to_feed == 0) {
                bits_to_feed = (rand() % 6) + 1;
            }
            wave_val = wave_val | 1;
            bits_to_feed -= 1;
        }
        for (index = 0; index < samples_per_shift; index += 1) {
            if (stream_index + index == length) {
                index = samples_per_shift;
            } else {
                stream[stream_index + index] = wave_val;
            }
        }
    }
    return stream;
}

void volume_filter(Uint8 * stream, int length, int attack, int decay,
                   int sustain, int release) {
    int stream_index = 0;
    float multiplier,
          sustain_multiplier = sustain / 16.0;
    while (stream_index < attack) {
        multiplier = (float) stream_index / attack;
        stream[stream_index] *= multiplier;
        stream_index += 1;
    }
    while (stream_index < attack + decay) {
        multiplier = (
            1 - ((float) stream_index - attack) / decay
        ) * (1 - sustain_multiplier) + sustain_multiplier;
        stream[stream_index] *= multiplier;
        stream_index += 1;
    }
    while (stream_index < length - release) {
        stream[stream_index] *= sustain_multiplier;
        stream_index += 1;
    }
    while (stream_index < length) {
        multiplier =
            sustain_multiplier * ((float) length - stream_index) / release;
        stream[stream_index] *= multiplier;
        stream_index += 1;
    }
}

audio_state_t * init_audio_state(Uint8 num_stream_queues) {
    int index;
    stream_queue_t * new_stream_queue;
    audio_state_t * audio_state = malloc(sizeof(audio_state_t));
    *audio_state = (audio_state_t) {
        .streams = malloc(sizeof(stream_queue_t *) * num_stream_queues),
        .num_stream_queues = num_stream_queues
    };
    for (index = 0; index < num_stream_queues; index += 1) {
        new_stream_queue = malloc(sizeof(stream_queue_t));
        *new_stream_queue = (stream_queue_t) {
            .length=0,
            .first_node=NULL,
            .last_node=NULL
        };
        *(audio_state->streams + index) = new_stream_queue;
    };
    SDL_AudioSpec desired = {
        .freq=44100,
        .format=AUDIO_S8,
        .channels=1,
        .samples=4096,
        .callback=play_sound,
        .userdata=audio_state
    };
    SDL_AudioSpec * obtained = malloc(sizeof(SDL_AudioSpec));
    SDL_AudioDeviceID device = SDL_OpenAudioDevice(
        NULL,
        0,
        &desired,
        obtained,
        SDL_AUDIO_ALLOW_ANY_CHANGE
    );
    if (device == 0) {
        fprintf(stderr, "SDL_OpenAudioDevice() failure: %s\n", SDL_GetError());
        exit(1);
    }
    audio_state->device = device;
    audio_state->audio_spec = obtained;
    if (desired.freq != audio_state->audio_spec->freq) {
        fprintf(
            stderr,
            "audio_state->audio_spec->freq is %i\n",
            audio_state->audio_spec->freq
        );
    }
    if (desired.format != audio_state->audio_spec->format) {
        fprintf(
            stderr,
            "audio_state->audio_spec->format is %i\n",
            audio_state->audio_spec->format
        );
    }
    if (desired.channels != audio_state->audio_spec->channels) {
        fprintf(
            stderr,
            "audio_state->audio_spec->channels is %i\n",
            audio_state->audio_spec->channels
        );
    }
    if (desired.samples != audio_state->audio_spec->samples) {
        fprintf(
            stderr,
            "audio_state->audio_spec.samples is %i\n",
            audio_state->audio_spec->samples
        );
    }
    fprintf(
        stderr,
        "audio_state->audio_spec.silence is %i\n",
        audio_state->audio_spec->silence
    );
    fprintf(
        stderr,
        "audio_state->audio_spec.size is %i\n",
        audio_state->audio_spec->size
    );
    return audio_state;
}

void destroy_audio_state(audio_state_t * audio_state) {
    int index;
    stream_queue_t * cur_stream;
    SDL_CloseAudioDevice(audio_state->device);
    for (index = 0; index < audio_state->num_stream_queues; index += 1) {
        cur_stream = *(audio_state->streams + index);
        while (cur_stream->first_node != NULL) {
            stream_queue_clear_first(cur_stream);
        }
        free(cur_stream);
    }
    free(audio_state->streams);
    free(audio_state->audio_spec);
    free(audio_state);
}
