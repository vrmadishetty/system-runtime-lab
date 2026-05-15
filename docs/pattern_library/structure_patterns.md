# Structure and Structure Pointer Patterns (Embedded C)

## 1. What it is
A struct is a user-defined type that groups different variables into a contiguous memory block. A Structure Pointer stores the address of that block.

```c
typedef struct {
    uint32_t timestamp;
    float temperature;
    uint8_t sensor_id;
} SensorData_t;

SensorData_t current_reading;     /* Variable */
SensorData_t *ptr = &current_reading; /* Pointer */
```

## 2. Why it is needed
**The Problem: Stack Bloat & Memory Fragmentation**
Passing large structs by value copies all data to the stack, which is slow and can cause overflows.
```c
/* BAD: Copies 64 bytes to stack every call */
void log_data_bad(SensorData_t data) {
    printf("ID: %d\n", data.sensor_id);
}
```

**The Solution: Efficient Indirection**
Passing a pointer only uses 4-8 bytes of stack and allows the function to modify the original data.
```c
/* GOOD: Passes 4-byte address, O(1) efficiency */
void log_data_good(const SensorData_t *data) {
    if (data != NULL) {
        printf("ID: %d\n", data->sensor_id);
    }
}
```

## 3. Where it is used
**Peripheral Register Mapping (CMSIS Style)**
Struct pointers are used to overlay a structure onto a specific physical hardware address.
```c
typedef struct {
    volatile uint32_t MODER;   /* GPIO port mode register          */
    volatile uint32_t OTYPER;  /* GPIO port output type register   */
    volatile uint32_t OSPEEDR; /* GPIO port output speed register  */
    volatile uint32_t ODR;     /* GPIO port output data register   */
} GPIO_Regs_t;

/* Map struct to hardware address */
#define GPIOA ((GPIO_Regs_t *)0x40020000)

void init_led(void) {
    GPIOA->MODER |= (1 << 10); /* Access member via pointer */
}
```

## 4. When to use it vs. Alternatives
**Struct Pointer vs. Array**
Use structs for mixed-type objects; use arrays for same-type buffers.
```c
/* Struct Pointer: Best for Objects or Drivers */
typedef struct { void (*init)(void); int id; } Driver_t;

/* Array: Best for Streams or Lists */
uint8_t rx_buffer[128]; 
```

## 5. How it works internally
The compiler aligns members to CPU-friendly boundaries (Padding). The pointer allows the CPU to calculate addresses using Base + Offset.
```c
struct __attribute__((aligned(4))) Example {
    char a;    /* Offset 0, size 1 */
    /* 3 bytes padding here */
    int b;     /* Offset 4, size 4 */
};

/* Internal Address math for ptr->b:
   Address = (uintptr_t)ptr + offsetof(struct Example, b); */
```

## 6. Important Interview Questions & Answers
**Q: Why use volatile in register structs?**
```c
volatile uint32_t ODR; 
```
*A: It tells the compiler that the value can change outside the programs control (by hardware), preventing the optimizer from caching the value in a CPU register.*

**Q: What is the risk of returning a struct pointer?**
```c
SensorData_t* get_data(void) {
    SensorData_t temp = {0};
    return &temp; /* ERROR: Returns address of stack variable */
}
```
*A: As shown above, the local variable temp is destroyed when the function exits, making the returned pointer dangling and dangerous.*

## 7. Common Beginner Mistakes
**Incorrect: Arrow operator on non-pointer**
```c
SensorData_t data;
data->sensor_id = 1; /* COMPILER ERROR: data is not a pointer */
```

**Corrected: Using Dot and Arrow correctly**
```c
SensorData_t data;
SensorData_t *p = &data;
data.sensor_id = 1; /* Correct for variable */
p->sensor_id = 1;   /* Correct for pointer  */
```

## 8. Sample Syntax and Access Pattern (MANDATORY)
```c
/* 1. Define the Template */
typedef struct {
    uint8_t cmd;
    uint32_t param;
} Packet_t;

/* 2. Instantiate and Point */
Packet_t my_packet;
Packet_t *pkt_ptr = &my_packet;

/* 3. Access using both operators */
my_packet.cmd = 0xA5;    /* Dot (.) for instances */
pkt_ptr->param = 0x1234; /* Arrow (->) for pointers */

/* 4. Passing to function */
void send_packet(const Packet_t *p) {
    if (p) transmit(p->cmd, p->param);
}

/* 5. Direct Casting (Hardware Mapping) */
#define CMD_BUFFER ((Packet_t *)0x20001000)
```

## 10. Mental Model / Brain Tattoo: The Luggage Tag
Passing a struct is like carrying a heavy suitcase (Memory Copy). Passing a structure pointer is like handing over a Luggage Tag (Pointer).
```c
/* Handing over the tag, not the weight */
void check_luggage(const Suitcase_t *tag); 
```
The tag points to the physical suitcase. When you use tag->contents, you are looking inside the original suitcase at its specific location in the RAM Warehouse.
