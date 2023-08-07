#include "pico/audio.h"
#include "pico/audio_i2s.h"
#include "pico/multicore.h"
#include "board_config.h"
#include "pico/stdlib.h"
#include "hardware/dma.h"
#include "tusb.h"
#include <math.h>

static struct audio_buffer_pool *producer_pool;

void i2s_audio_init() {
    gpio_init(PICO_AUDIO_I2S_AMP_ENABLE);
    gpio_set_function(PICO_AUDIO_I2S_AMP_ENABLE, GPIO_FUNC_SIO);
    gpio_set_dir(PICO_AUDIO_I2S_AMP_ENABLE, GPIO_OUT);
    gpio_put(PICO_AUDIO_I2S_AMP_ENABLE, 1); // SD_MODE also selects audio channel, must be HIGH to enable amp, LOW to shutdown

    // initialize for 48k we allow changing later
    static audio_format_t audio_format_48k = {
            .format = AUDIO_BUFFER_FORMAT_PCM_S16,
            .sample_freq = 48000,
            .channel_count = 2,
    };

    static audio_buffer_format_t producer_format = {
            .format = &audio_format_48k,
            .sample_stride = sizeof(int16_t) * 2
    };

    // We need to make sure to claim an unused channel,
    // but then audio_i2s_setup tries to claim it again...
    // So claim & unclaim it, effectively checking it's unused.
    uint dma_channel = dma_claim_unused_channel(true);
    dma_channel_unclaim(dma_channel);

    producer_pool = audio_new_producer_pool(&producer_format, 3, CFG_TUD_AUDIO_FUNC_1_EP_OUT_SW_BUF_SZ / sizeof(int16_t)); // todo correct size

    audio_i2s_config_t config = {
            .data_pin = PICO_AUDIO_I2S_DATA_PIN,
            .clock_pin_base = PICO_AUDIO_I2S_CLOCK_PIN_BASE,
            .dma_channel = dma_channel,
            .pio_sm = 0,
    };

    const audio_format_t *output_format;
    output_format = audio_i2s_setup(&audio_format_48k, &config);
    if (!output_format) {
        panic("PicoAudio: Unable to open audio device.\n");
    }

    bool __unused ok;
    ok = audio_i2s_connect(producer_pool);
    assert(ok);
    {
        audio_buffer_t *buffer = take_audio_buffer(producer_pool, true);
        int16_t *samples = (int16_t *) buffer->buffer->bytes;
        for (uint i = 0; i < buffer->max_sample_count; i++) {
            samples[i*2+0] = 0;
            samples[i*2+1] = 0;
        }
        buffer->sample_count = buffer->max_sample_count;
        give_audio_buffer(producer_pool, buffer);
    }
}

static void core1_worker() {
    gpio_put(LED_R, 0);
    audio_i2s_set_enabled(true);
    gpio_put(LED_R, 1);
}

void i2s_audio_start() {
    core1_worker();
    //multicore_launch_core1(core1_worker);
}

void i2s_audio_give_buffer(void *src, size_t len, uint8_t bit_depth, uint8_t volume) {
    struct audio_buffer *audio_buffer = take_audio_buffer(producer_pool, false);

    if(audio_buffer) {
        int16_t *out = (int16_t *) audio_buffer->buffer->bytes;

        if(bit_depth == 16) {
            int16_t *in = (int16_t *) src;

            size_t in_samples = len / sizeof(int16_t) / 2;
            size_t samples = audio_buffer->max_sample_count;
            if(in_samples < samples) samples = in_samples;

            for (uint i = 0u; i < samples * 2; i+=2) {
                out[i+0] = ((uint32_t)in[i+0] * volume) / 256;
                out[i+1] = ((uint32_t)in[i+1] * volume) / 256;
            }

            audio_buffer->sample_count = samples;

        } else if (bit_depth == 24) {
            int32_t *in = (int32_t *) src;

            size_t in_samples = len / sizeof(int32_t) / 2;
            size_t samples = audio_buffer->max_sample_count;
            if(in_samples < samples) samples = in_samples;

            for (uint i = 0u; i < samples * 2; i+=2) {
                out[i+0] = ((in[i+0] / 65536) * volume) / 256;
                out[i+1] = ((in[i+1] / 65536) * volume) / 256;
            }

            audio_buffer->sample_count = samples;
        }

        give_audio_buffer(producer_pool, audio_buffer);
    }
}