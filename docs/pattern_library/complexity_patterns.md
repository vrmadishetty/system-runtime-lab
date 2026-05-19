### **Time and Space Complexity (Big O) in Embedded Systems**

---

#### 1. What it is
```c
/* O(1) Time and Space Complexity */
typedef struct {
    uint32_t data;
} SimpleNode_t;

/* O(N) Time Complexity Loop */
void delay_loop(uint32_t n) {
    for (uint32_t i = 0; i < n; i++) {
        __asm__("nop"); 
    }
}
```
**Time complexity** measures how execution time grows as the input `n` grows. **Space complexity** measures how memory RAM/Stack usage grows as the input `n` grows.

---

#### 2. Why it is needed
```c
/* THE PROBLEM: High Time Complexity O(N) */
uint8_t find_sensor_state_slow(uint8_t id, uint8_t *array, uint8_t size) {
    for (uint8_t i = 0; i < size; i++) {
        if (array[i] == id) return 1; /* Requires scanning memory */
    }
    return 0;
}

/* THE SOLUTION: Trading Space for O(1) Time Complexity */
uint8_t sensor_state_fast[256]; /* Uses more RAM: O(N) Space */

uint8_t find_sensor_state_fast(uint8_t id) {
    return sensor_state_fast[id]; /* Direct lookup: O(1) Time */
}
```
Understanding complexity is needed to balance CPU speed (Time) against limited RAM (Space). The slow code saves RAM but wastes CPU cycles. The fast code uses more RAM but gives instant O(1) responses.

---

#### 3. Where it is used
```c
/* Real Embedded Usage: CRC Calculation LUT */
/* Space Complexity: O(N) where N is 256 entries (256 bytes) */
/* Time Complexity: O(1) per byte processed */
const uint8_t crc8_lut[256] = {
    0x00, 0x07, 0x0E, 0x09, 0x1C, 0x1B, 0x12, 0x15,
    /* ... remaining 248 entries ... */
};

uint8_t calculate_crc_fast(uint8_t data) {
    return crc8_lut[data]; /* O(1) Time access */
}
```
In embedded systems, Look-Up Tables (LUTs) are heavily used in drivers and protocols to achieve O(1) time complexity for math operations by spending Flash/RAM space.

---

#### 4. When to use it vs alternatives
```c
/* Runtime Math: Space O(1), Time O(N) or higher (CPU intensive) */
float calculate_voltage(uint16_t adc_val) {
    return (float)adc_val * (3.3f / 4095.0f);
}

/* Precomputed Array: Space O(N), Time O(1) */
extern const float voltage_lut[4096]; 
float get_voltage(uint16_t adc_val) {
    return voltage_lut[adc_val];
}
```
Use runtime math (Time > Space) when RAM/Flash is critically low. Use precomputed arrays/pointers (Space > Time) in hard real-time systems where execution speed must be deterministic and instantaneous.

---

#### 5. How it works internally (high level)
```c
uint32_t data_array[100]; /* Space Complexity: O(N) -> 100 * 4 = 400 bytes */
uint32_t val = data_array[50]; /* Time Complexity: O(1) */

/* Internal pointer arithmetic the CPU performs for O(1) access: */
/* Address = base_address + (50 * sizeof(uint32_t)) */
```
Arrays and structures provide O(1) time complexity because the compiler translates the index or member offset into a single hardware addition instruction, independent of the total size `N`. Space complexity is calculated by `sizeof(type) * N`.

---

#### 6. Important interview questions and answers
```c
typedef struct Node {
    uint32_t value;
    struct Node *next;
} Node_t;

uint32_t search_list(Node_t *head, uint32_t target) {
    while (head != NULL) {
        if (head->value == target) return 1;
        head = head->next;
    }
    return 0;
}
```
**Q: What is the Time and Space complexity of traversing this linked list?**
*A: The time complexity is O(N) because in the worst case, the loop must check every node. The space complexity is O(1) because we only use one local pointer (`head`), regardless of list length.*

---

#### 7. Common beginner mistakes
```c
/* INCORRECT: O(N^2) Time Complexity */
/* strlen() is O(N). Calling it inside an O(N) loop multiplies them. */
void process_string_bad(const char *str) {
    for (uint32_t i = 0; i < strlen(str); i++) {
        /* Process character */
    }
}

/* CORRECT: O(N) Time Complexity */
/* Call strlen() once (O(N)), then loop (O(N)). N + N = O(N) */
void process_string_good(const char *str) {
    uint32_t len = strlen(str); 
    for (uint32_t i = 0; i < len; i++) {
        /* Process character */
    }
}
```
Beginners often place hidden O(N) operations inside loops, accidentally creating catastrophic O(N²) time complexity in embedded processors.

---

#### 8. Sample Syntax and Access Pattern (MANDATORY)
```c
/* 1. Structure definition (Space: O(1) fixed size) */
typedef struct {
    uint32_t control;
    uint32_t status;
} Peripheral_t;

/* 2. Structure variable (Space: allocates 8 bytes) */
Peripheral_t my_periph;

/* 3. Structure pointer (Space: allocates 4 bytes for address) */
Peripheral_t *p_periph = &my_periph;

/* 4. Access using '.' and '->' (Time: O(1) single instruction) */
my_periph.control = 0x01;
p_periph->status = 0x00;

/* 5. Passing structure pointer to a function (Time: O(1), Space: O(1)) */
void enable_peripheral(Peripheral_t *p) {
    if (p) p->control = 0x01;
}

/* 6. Mapping struct to hardware address (Space: 0 bytes RAM used) */
#define HARDWARE_PERIPH ((Peripheral_t *)0x40000000)
```
Every operation in the struct access pattern above represents the gold standard of embedded programming: **O(1) Time Complexity** (constant execution speed) and **O(1) Space Complexity** (no dynamic memory growth).

---

#### 9. Create Markdown file content for `pattern_library`
```markdown
# Pattern: Time and Space Complexity Optimization

### Implementation
```c
/* Time/Space Trade-off Pattern */

/* High Time Complexity (O(N)), Low Space O(1) */
uint32_t count_bits_slow(uint32_t n) {
    uint32_t count = 0;
    while (n) {
        count += n & 1;
        n >>= 1;
    }
    return count;
}

/* Low Time Complexity (O(1)), High Space O(N) */
extern const uint8_t bit_count_lut[256]; 
uint32_t count_bits_fast(uint8_t n) {
    return bit_count_lut[n];
}
```

### Technical Notes
1. **O(1) Constant:** Execution time or memory usage never changes, regardless of data size. (e.g., struct access, array indexing).
2. **O(N) Linear:** Time or memory scales directly with the amount of data. (e.g., `for` loops, buffers).
3. **Embedded Rule:** RAM/Flash limits often force us to write slower O(N) code. When hard real-time latency is required, we spend RAM to create O(1) lookup tables.
```

---

#### 10. Mental Model / Brain Tattoo
```c
uint32_t result = lookup_table[index]; /* O(1) Time */
```
**The Math Book vs. The Cheat Sheet**
Calculating complexity is like taking a test. 
*   **Time Complexity O(N):** Calculating the answer by hand every time using a formula (Uses zero extra desk space, but takes a long time).
*   **Space Complexity O(N):** Bringing a pre-printed "Cheat Sheet" array (`lookup_table[index]`). It takes up a lot of physical space on your desk, but finding the answer takes exactly one second (O(1) Time).