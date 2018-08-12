struct stream_queue_node_s_t;

typedef struct stream_queue_node_s_t {
    Uint8 * stream;
    int length;
    struct stream_queue_node_s_t * next_node;
    int samples_played;
} stream_queue_node_t;

typedef struct {
    int length;
    stream_queue_node_t * first_node;
    stream_queue_node_t * last_node;
} stream_queue_t;

void stream_queue_enqueue(stream_queue_t * stream_queue, Uint8 * stream,
                          int length);
void stream_queue_clear_first(stream_queue_t * stream_queue);

typedef struct {
    stream_queue_t ** streams;
    Uint8 num_stream_queues;
    SDL_AudioDeviceID device;
    SDL_AudioSpec * audio_spec;
} audio_state_t;

void play_sound(void * userdata, Uint8 * stream, int len);

Uint8 * pulse(int frequency, int length);
Uint8 * sawtooth(int frequency, int length);
Uint8 * sine_wave(int frequency, int length);
Uint8 * noise(int frequency, int length);

void volume_filter(Uint8 * stream, int length, int attack, int decay,
                   int sustain, int release);

audio_state_t * init_audio_state(Uint8 num_stream_queues);
void destroy_audio_state(audio_state_t * audio_state);
