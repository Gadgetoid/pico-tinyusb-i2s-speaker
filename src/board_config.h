#pragma once
#include "pico/stdlib.h"

static const uint PIN_DCDC_PSM_CTRL = 23;
static const uint AMP_EN = 13;
static const uint PCM_DOUT = 14;
static const uint PCM_BCLK = 15;
static const uint PCM_FS = 16;
static const uint LED_R = 17;
static const uint LED_G = 18;
static const uint LED_B = 19;
static const uint USER_SW = 24; // BOOT / USER
static const uint QW_SDA = 4;
static const uint QW_SCL = 5;

#define PICO_AUDIO_I2S_AMP_ENABLE AMP_EN