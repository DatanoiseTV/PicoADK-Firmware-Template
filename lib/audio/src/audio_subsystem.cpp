#include "audio_subsystem.h"

#include "project_config.h"

audio_buffer_pool_t *init_audio()
{

    static audio_format_t audio_format = {
        .sample_freq = 44100,
        .pcm_format = AUDIO_PCM_FORMAT_S32,
        .channel_count = (audio_channel_t)2};

    static audio_buffer_format_t producer_format = {
        .format = &audio_format,
        .sample_stride = 8};

    audio_buffer_pool_t *producer_pool = audio_new_producer_pool(&producer_format, 3,
                                                                 SAMPLES_PER_BUFFER); // todo correct size
    bool __unused ok;
    const audio_format_t *output_format;
#if USE_AUDIO_I2S
    audio_i2s_config_t config = {
        .data_pin = PIN_I2S_DOUT,
        .clock_pin_base = PIN_I2S_BCK,
        .dma_channel = 0,
        .pio_sm = 0};

    output_format = audio_i2s_setup(&audio_format, &audio_format, &config);
    if (!output_format)
    {
        panic("PicoAudio: Unable to open audio device.\n");
    }

    printf("PicoAudio: Audio output format: %d Hz, %d bit, %d channel\n",
           output_format->sample_freq,
           output_format->pcm_format == AUDIO_PCM_FORMAT_S16 ? 16 : 32,
           output_format->channel_count);

    ok = audio_i2s_connect(producer_pool);
    assert(ok);
    { // initial buffer data
        audio_buffer_t *buffer = take_audio_buffer(producer_pool, true);
        int32_t *samples = (int32_t *)buffer->buffer->bytes;

        printf("PicoAudio: Initializing audio buffer with %d samples\n",
               buffer->buffer->size / sizeof(int32_t));
        for (uint i = 0; i < buffer->max_sample_count; i++)
        {
            samples[i * 2 + 0] = 0;
            samples[i * 2 + 1] = 0;
        }
        buffer->sample_count = buffer->max_sample_count;
        give_audio_buffer(producer_pool, buffer);
    }
    audio_i2s_set_enabled(true);
#elif USE_AUDIO_PWM
    output_format = audio_pwm_setup(&audio_format, -1, &default_mono_channel_config);
    if (!output_format)
    {
        panic("PicoAudio: Unable to open audio device.\n");
    }
    ok = audio_pwm_default_connect(producer_pool, false);
    assert(ok);
    audio_pwm_set_enabled(true);
#elif USE_AUDIO_SPDIF
    output_format = audio_spdif_setup(&audio_format, &audio_spdif_default_config);
    if (!output_format)
    {
        panic("PicoAudio: Unable to open audio device.\n");
    }
    // ok = audio_spdif_connect(producer_pool);
    ok = audio_spdif_connect(producer_pool);
    assert(ok);
    audio_spdif_set_enabled(true);
#endif
    return producer_pool;
}
