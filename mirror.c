#include <stdio.h>
#include <stdlib.h>
#include "pico/stdlib.h"
#include "servo.h"
#include "ws2812.pio.h"

static inline void put_pixel(uint32_t pixel_grb) {
    pio_sm_put_blocking(pio0, 0, pixel_grb << 8u);
}

static inline uint32_t urgb_u32(uint8_t r, uint8_t g, uint8_t b) {
    return
            ((uint32_t) (r) << 8) |
            ((uint32_t) (g) << 16) |
            (uint32_t) (b);
}

void pattern_random(uint len) {
    for (int i = 0; i < len; ++i)
        put_pixel(rand());
}

const int neo_PIN = 16;
const int neo_LENGTH = 29;

bool direction = true;
int currentMillis = 400;
int servoPin = 15;

int main()
{
    PIO neo_pio = pio0;
    int neo_sm = 0;

    uint neo_offset = pio_add_program(neo_pio, &ws2812_program);

    ws2812_program_init(neo_pio, neo_sm, neo_offset, neo_PIN, 800000, true);

    setServo(servoPin, currentMillis);
    while (true)
    {
        pattern_random(neo_LENGTH);
        currentMillis += (direction)?5:-5;
        if (currentMillis >= 2500) direction = false;
        if (currentMillis <= 300) direction = true;
        setMillis(servoPin, currentMillis);
        sleep_ms(50);
    }
}