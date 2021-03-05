#include <stdio.h>
#include <stdlib.h>

#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "hardware/clocks.h"
#include "ws2812.pio.h"
#include "pwm.pio.h"

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

// Write `period` to the input shift register
void pio_pwm_set_period(PIO pio, uint sm, uint32_t period) {
    pio_sm_set_enabled(pio, sm, false);
    pio_sm_put_blocking(pio, sm, period);
    pio_sm_exec(pio, sm, pio_encode_pull(false, false));
    pio_sm_exec(pio, sm, pio_encode_out(pio_isr, 32));
    pio_sm_set_enabled(pio, sm, true);
}

// Write `level` to TX FIFO. State machine will copy this into X.
void pio_pwm_set_level(PIO pio, uint sm, uint32_t level) {
    pio_sm_put_blocking(pio, sm, level);
}



const int neo_PIN = 16;
const int neo_LENGTH = 29;

const int servo1_PIN = 15;
const int servo2_PIN = 14;

int main()
{
    stdio_init_all();

    puts("Hello, Mirror!");

    PIO neo_pio = pio0;
    int neo_sm = 0;

    uint neo_offset = pio_add_program(neo_pio, &ws2812_program);

    ws2812_program_init(neo_pio, neo_sm, neo_offset, neo_PIN, 800000, true);

    PIO servo1_pio = pio1;
    int servo1_sm = 0;
    uint servo1_offset = pio_add_program(servo1_pio, &pwm_program);
    printf("Loaded program at %d\n", servo1_offset);

    pwm_program_init(servo1_pio, servo1_sm, servo1_offset, servo1_PIN);
    pio_pwm_set_period(servo1_pio, servo1_sm, (1u << 16) - 1);

    PIO servo2_pio = pio1;
    int servo2_sm = 1;
    uint servo2_offset = pio_add_program(servo2_pio, &pwm_program);
    printf("Loaded program at %d\n", servo2_offset);

    pwm_program_init(servo2_pio, servo2_sm, servo2_offset, servo2_PIN);
    pio_pwm_set_period(servo2_pio, servo2_sm, (1u << 16) - 1);

    int level = 0;
    while (1) {
        pattern_random(neo_LENGTH);

        printf("Level = %d\n", level);
        pio_pwm_set_level(servo1_pio, servo1_sm, (level+134) * (level+134));
        pio_pwm_set_level(servo2_pio, servo2_sm, (level+134) * (level+134));
        level = (level + 1) % 150;
        sleep_ms(50);
    }
}
