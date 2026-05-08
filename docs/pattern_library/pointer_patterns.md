# Function Pointer Patterns in Embedded Systems

Function pointers are the primary tool for achieving **Polymorphism** and **Asynchronous Event Handling** in C. For Senior/Staff roles, the focus shifts from syntax to **Architectural Integrity** and **Memory Efficiency**.

---

## 1. The Jump Table (CLI Pattern)
**Referenced Lab:** `labs/function_pointers/cli_jumpTable.c`

### **What it is**
An array of structs where each entry maps a key (like a string or ID) to a function.

### **Why it is used**
To replace large `switch-case` or `if-else` blocks. It makes the code extensible; you can add new commands just by adding an entry to the array, without modifying the logic of the "processor" loop.

### **When to use it**
*   Implementing Command Line Interfaces (CLI).
*   Handling communication protocols (Modbus, CAN, Serial).
*   Dispatching events in a state machine.

> **💡 Senior Pro-Tip for the Interview:**
> If asked about Jump Tables vs. `if-else`, mention **Branch Prediction**.
> *   **Senior Answer:** "While jump tables provide O(1) lookup, they rely on 'indirect jumps' which are harder for the CPU's branch predictor to guess than a simple `if-else`. However, in embedded systems, the benefits of code-size reduction and long-term maintainability usually outweigh the minor performance cost of a pipeline stall."

```c
typedef struct {
    char *name;
    void (*handler)(void);
} Command_t;

// Optimization: Stored in Flash using 'const'
const Command_t commands[] = {
    {"help",   cmd_help},
    {"reboot", cmd_reboot},
    {"status", cmd_status} 
};
```

---

## 2. The Safe Callback (Async Pattern)
**Referenced Lab:** `labs/function_pointers/safe_callback.c`

### **What it is**
A registration mechanism where one layer "gives" a function to a lower layer (like a driver).

### **Why it is used**
**Inversion of Control.** The driver knows *when* an event happens (e.g., a timer expires), but it doesn't know *what* the application wants to do. The callback allows the driver to remain generic.

### **When to use it**
*   Interrupt Service Routines (ISRs).
*   Timer expiration notifications.
*   Peripheral data arrival (UART/DMA).

### **Industrial Strength Check**
Always verify the pointer is not NULL before execution to prevent system crashes (HardFault).

```c
void trigger_interrupt(void) {
    if (g_interrupt_cb != NULL) {
        g_interrupt_cb();
    } else {
        // Defensive: handle missing callback
        printf("no callback registered \n");
    }
}
```

---

## 3. The HAL / vTable (Interface Pattern)
**Referenced Lab:** `labs/function_pointers/hal_callbacks.c`

### **What it is**
A struct containing multiple function pointers that define a "Service" or "Interface."

### **Why it is used**
To achieve **Hardware Abstraction**. Your application code calls `driver->turn_on()`, and it doesn't care if the underlying hardware is an STM32, NXP, or a Simulator. 

### **When to use it**
*   Building a Hardware Abstraction Layer (HAL).
*   Supporting multiple board variants with the same firmware image.
*   Unit testing (using "Mock" drivers).

```c
typedef struct {
    bool (*init)(void);
    void (*turn_on)(void);
    void (*turn_off)(void); 
} LED_Driver_t;

// Application logic remains unchanged regardless of the 'driver' passed
void blink_led(const LED_Driver_t *driver) {
    if (driver && driver->init && driver->init()) {
        driver->turn_on();
        driver->turn_off();
    }
}
```

---

## 4. The Contextual Callback (Object Pattern)
**Referenced Lab:** `labs/function_pointers/sensor_context.c`

### **What it is**
A callback that passes a `void *context` pointer back to the user.

### **Why it is used**
To solve the "State" problem. In C, functions don't have memory. If you have 5 different temperature sensors, the callback needs to know *which* sensor triggered and where to store its specific data. The `context` pointer carries this "private" data.

### **When to use it**
*   Complex systems with multiple instances of the same hardware.
*   Linking drivers to higher-level Application Objects.
*   Embedded Linux kernel drivers (e.g., `file_operations`).

```c
typedef void (*SensorCallback)(int id, float value, void *context);

void temperature_Callback(int id, float val, void *context) {
    // Cast context back to specific application structure
    struct AppConfig *config = (struct AppConfig *)context;
    printf("Sensor %d in %s read %.2f\n", id, config->room_name, val);
}
```

---

## 5. Interview Prep: High-Signal Terminology

Use these terms to demonstrate seniority and architectural thinking:

*   **Indirection:** The overhead of looking up an address in memory before jumping to it.
*   **Decoupling:** Reducing dependencies between layers (e.g., a driver doesn't need to know about the application).
*   **Opaque Handles:** Hiding internal structure details from the user to enforce modularity.
*   **Reset Vector:** A hardware-level function pointer that points to the first instruction after a reset.
*   **Late Binding:** Determining which code to execute at runtime rather than at compile time.
*   **Re-entrancy:** Ensuring a function (or callback) can be safely interrupted and called again.

### **The "Brain Tattoo"**
Function pointers are the **Waiters** of the C world. You give the **Kitchen** (Driver) an order, and the **Waiter** (Callback) brings the result back to your **Table** (Context) when it's ready.
