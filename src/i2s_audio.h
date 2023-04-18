void i2s_audio_init();
void i2s_audio_start();
void i2s_audio_give_buffer(void *src, size_t len, uint8_t bit_depth); // TODO Volume scaling