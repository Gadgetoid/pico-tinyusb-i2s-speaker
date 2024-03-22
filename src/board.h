#include <stdint.h>

void system_init();
int32_t get_volume_delta();
void system_led(uint8_t r, uint8_t g, uint8_t b);
bool get_mute_button_pressed();
void handle_mute_button_held();