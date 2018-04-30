typedef struct {
    int samples_played;
    int length;
    Uint8 * stream;
} audio_state_t;

void play_sound(void * userdata, Uint8 * stream, int len);

Uint8 * pulse(int frequency, int length);
Uint8 * sawtooth(int frequency, int length);
