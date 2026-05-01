#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdint.h>

/** 
Task 4: The Hardware Abstraction Layer (HAL) Interface**
Scenario:** You are writing code that can run on both an STM32 and an NXP microcontroller. You need a generic way to control an LED without changing your main application logic.
1.  Create a struct named `LED_Driver`.
2.  Inside the struct, define three function pointers:
    *   `init`: takes void, returns bool (success/fail).
    *   `turn_on`: takes void, returns void.
    *   `turn_off`: takes void, returns void.
3.  **Challenge:** 
    *   Write two sets of dummy functions: one for STM32 (`stm32_led_init`, etc.) and one for NXP (`nxp_led_init`, etc.). Each should just `printf` what it is doing.
    *   Create two instances of your `LED_Driver` struct, one populated with the STM32 functions and one with the NXP functions.
    *   Write a function `void blink_led(const LED_Driver *driver)` that initializes the LED, turns it on, and turns it off. Pass both structs to it in your `main()`.
*/


typedef struct {
    bool (*init) (void);
    void (*turn_on) (void);
    void (*turn_off) (void); 

} LED_Driver_t;

bool stm32_led_init (void) {
    printf("[STM32] GPIO Clock Enabled\n");
    return true;
}
void stm32_led_turn_on (void) {
    printf("[STM32] led turn on \n");
}

void stm32_led_turn_off (void) {
    printf("[STM32] led turn off \n");
}

bool nxp_led_init (void) {
    printf("[NXP] GPIO Clock Enabled\n");
    return true;
}
void nxp_led_turn_on (void) {
    printf("[NXP] led turn on \n");
}
void nxp_led_turn_off (void) {
    printf("[NXP] led turn off \n");
}


const LED_Driver_t st_stm32 = {
    stm32_led_init,
    stm32_led_turn_on,
    stm32_led_turn_off
};

const LED_Driver_t st_nxp = {
    nxp_led_init,
    nxp_led_turn_on,
    nxp_led_turn_off
};

// The Generic Application Function (Doesn't care about the hardware)
void blink_led(const LED_Driver_t *driver) {

    // never assume hardware initialization works. If init fails, you should
    // not attempt to turn_on LED.
    if (driver && driver->init && driver->init())
    {
        driver->turn_on();
        driver->turn_off();
    } else {
        printf("Error: Driver intialization failed or driver is NULL! \n");
    }

}

int main() {
    blink_led (&st_stm32);
    blink_led (&st_nxp);
    blink_led(NULL);
    return 0;
}