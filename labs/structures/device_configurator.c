#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

/*
### **Task 1: The Device Configurator (Validation via Pointers)**
**Scenario:** You are writing a driver for a Communication Module.
*   **Struct:** Define a `ComConfig_t` containing `uint32_t baudrate`, `uint8_t parity` (0-2), and `bool is_active`.
*   **Goal:** Write a function `void activate_config(ComConfig_t *config)`.
*   **Requirements:**
    1.  If the baudrate is less than 9600, force it to 9600.
    2.  If parity is invalid (greater than 2), reset it to 0.
    3.  Set `is_active` to `true`.
    4.  The function must handle a `NULL` pointer safely.
*/

typedef struct DeviceConfig {
    uint32_t baudrate;
    uint8_t parity;
    bool is_active;

} ComConfig_t;

void activate_config(ComConfig_t *config) {

    if (config == NULL) {
        printf("Error: Config pointer is NULL.\n");
        return; 
    }
    if (config->baudrate < 9600) {
        printf("Warning: Baudrate too low. Setting to default 9600.\n");
        config->baudrate = 9600;
    }
    if (config->parity > 2) {
        printf("Warning: Invalid parity. Setting to default 0 (none).\n");
        config->parity = 0;
    }

    config->is_active = true;
    printf("Configuration activated: Baudrate=%u, Parity=%u, Active=%s\n",
           config->baudrate, config->parity, config->is_active ? "Yes" : "No");
}
int main() {
    ComConfig_t config;

    config.baudrate = 9600;
    config.parity = 0;
    config.is_active = true;

    activate_config(&config);

    return 0;
}