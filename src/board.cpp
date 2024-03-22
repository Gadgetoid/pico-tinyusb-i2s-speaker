#include "encoder.hpp"
#include "rgbled.hpp"
#include "button.hpp"
#include "board_config.h"

#include "pico/bootrom.h"
#include "hardware/structs/rosc.h"
#include "hardware/watchdog.h"
#include "hardware/sync.h"

using namespace encoder;
using namespace pimoroni;

Encoder volume_control(pio1, 0, {ENC_A, ENC_B});
RGBLED rgbled(LED_R, LED_G, LED_B);
Button button(BUTTON, pimoroni::ACTIVE_LOW, 0);

bool pressed = false;
absolute_time_t pressed_time;

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

    bool get_mute_button_pressed() {
        return button.read();
    }

    void handle_mute_button_held() {
#ifdef DEBUG_BOOTLOADER_SHORTCUT
        bool current = button.raw();
        if(current && pressed) {
            if (absolute_time_diff_us(pressed_time, get_absolute_time()) >= 2000000ul) {
                sleep_ms(500);
                save_and_disable_interrupts();
                rosc_hw->ctrl = ROSC_CTRL_ENABLE_VALUE_ENABLE << ROSC_CTRL_ENABLE_LSB;
                reset_usb_boot(0, 0);
            }
        } else if (current) {
            pressed_time = get_absolute_time();
            pressed = true;
        } else {
            pressed = false;
        }
#endif
    }

    void system_led(uint8_t r, uint8_t g, uint8_t b) {
        rgbled.set_rgb(r, g, b);
    }
}