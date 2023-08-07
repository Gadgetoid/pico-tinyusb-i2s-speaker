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

static const int BUTTON = 27;
static const uint ENC_A = 28;
static const uint ENC_B = 29;

#define PICO_AUDIO_I2S_AMP_ENABLE AMP_EN

enum
{
  VOLUME_CTRL_0_DB = 0,
  VOLUME_CTRL_10_DB = 2560,
  VOLUME_CTRL_20_DB = 5120,
  VOLUME_CTRL_30_DB = 7680,
  VOLUME_CTRL_40_DB = 10240,
  VOLUME_CTRL_50_DB = 12800,
  VOLUME_CTRL_60_DB = 15360,
  VOLUME_CTRL_70_DB = 17920,
  VOLUME_CTRL_80_DB = 20480,
  VOLUME_CTRL_90_DB = 23040,
  VOLUME_CTRL_100_DB = 25600,
  VOLUME_CTRL_SILENCE = 0x8000,
};