/*
Task 5: The Sensor with Context (Advanced Callback)**
Scenario:** The problem with simple callbacks is they don't carry data. If a UART receives a byte, how does the callback know *which* UART fired or *what* the data is? We solve this with **Context Pointers (`void *`)**.

1.  Define a callback type: `typedef void (*SensorCallback)(int sensor_id, float temp_value, void *context);`
2.  Create a struct `TemperatureSensor`:
    *   `int id;`
    *   `SensorCallback on_ready;`
    *   `void *user_context;` // The magic ingredient!
3.  Write an initialization function: 
    `void init_sensor(struct TemperatureSensor *sensor, int id, SensorCallback cb, void *ctx)`
4.  Write a simulation function that acts like the hardware interrupt:
    `void simulate_sensor_reading(struct TemperatureSensor *sensor, float simulated_temp)`
    *   *Hint:* This function should check if `on_ready` is not NULL, and if so, call it, passing the `sensor->id`, the `simulated_temp`, and the `sensor->user_context`.
5.  **Challenge:** 
    *   In `main()`, define a custom struct `AppConfig` that holds a string (e.g., `"Living Room"`).
    *   Create a `TemperatureSensor` instance.
    *   Write a callback function that casts the `void *context` back to an `AppConfig*` and prints: *"Sensor [ID] in [Room Name] reads [Temp] degrees."*
*/
#include <stdio.h>

/* callback with context */
typedef void (*SensorCallback) (int sensor_id, float temp_value, void *context);

typedef struct {
    int id;
    SensorCallback on_ready;
    void *user_context; /* context pointer */
} TemperatureSensor_t;

void init_sensor(TemperatureSensor_t *sensor, int id, SensorCallback cb, void *ctx) {

    if (sensor == NULL) { // safe against NULL sensor
        return;
    }
    sensor->id = id;
    sensor->on_ready = cb;
    sensor->user_context = ctx;
}

void simulate_sensor_reading(TemperatureSensor_t *sensor, float simulated_temp) {
       
    if (sensor == NULL) { // safe against NULL sensor
        return;
    }

    if(sensor->on_ready != NULL) {

        sensor->on_ready(sensor->id, simulated_temp, sensor->user_context);

    }
    else {
        printf("sensor is not registered !!");
    }
}

struct AppConfig {
    const char *room_name;
};


void temperature_Callback (int sensor_id, float temp_value, void *context) {
    struct AppConfig *config = (struct AppConfig *)context;

    if (config != NULL) {
        printf("Sensor %d in %s read %.2f degress.\n",
        sensor_id,
        config->room_name,
        temp_value);
    }
}

int main() {
    
    struct AppConfig living_room = {
        .room_name = "Living Room"
    };

    TemperatureSensor_t sensor;
    init_sensor (&sensor,
                    1,
                temperature_Callback,
            &living_room);

    
    simulate_sensor_reading(&sensor, 23.5f);
    simulate_sensor_reading(&sensor, 24.1f);

    return 0;
}

