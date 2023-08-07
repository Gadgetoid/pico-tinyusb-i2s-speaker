#include "encoder.hpp"
#include "rgbled.hpp"
#include "board_config.h"

using namespace encoder;
using namespace pimoroni;

Encoder volume_control(pio1, 0, {ENC_A, ENC_B});
RGBLED rgbled(LED_R, LED_G, LED_B);

extern "C" {
    void system_init() {
        // DCDC PSM control
        // 0: PFM mode (best efficiency)
        // 1: PWM mode (improved ripple)
        gpio_init(PIN_DCDC_PSM_CTRL);
        gpio_set_dir(PIN_DCDC_PSM_CTRL, GPIO_OUT);
        gpio_put(PIN_DCDC_PSM_CTRL, 1); // PWM mode for less Audio noise

        volume_control.init();
    }

    int32_t get_volume_delta() {
        return volume_control.delta();
    }

    void system_led(uint8_t r, uint8_t g, uint8_t b) {
        rgbled.set_rgb(r, g, b);
    }
}