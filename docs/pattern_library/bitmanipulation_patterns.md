### **Bitwise Operators in Embedded Systems**

---

#### 1. What it is
```c
uint8_t a = 0x05;      /* Binary: 0000 0101 */
uint8_t b = 0x09;      /* Binary: 0000 1001 */

uint8_t res_and = a & b;  /* 0x01 (AND) */
uint8_t res_or  = a | b;  /* 0x0D (OR)  */
uint8_t res_xor = a ^ b;  /* 0x0C (XOR) */
uint8_t res_not = ~a;     /* 0xFA (NOT) */
uint8_t res_lsh = a << 1; /* 0x0A (Left Shift) */
uint8_t res_rsh = a >> 1; /* 0x02 (Right Shift) */
```
Bitwise operators perform logic operations on individual bits within a variable. In C, these are handled directly by the CPU's Arithmetic Logic Unit (ALU).

---

#### 2. Why it is needed
**The Problem: Inefficient Memory & Multi-step Logic**
```c
/* Trying to extract bit 3 using logical operators */
bool is_bit_3_set = false;
if (val == 8 || val == 9 || val == 10 || val == 11 || val == 12 || val == 13 || val == 14 || val == 15) {
    is_bit_3_set = true;
}
```
Using logical `if` statements to check a single bit is slow, consumes massive code space, and is impossible to maintain for 32-bit registers.

**The Solution: Masking**
```c
bool is_bit_3_set = (val & (1 << 3)) != 0;
```
A single bitwise AND operation identifies the state of a specific bit in a single CPU cycle, regardless of the value of other bits.

---

#### 3. Where it is used
**GPIO Register Control**
```c
#define GPIO_ODR_ADDR 0x40020014
#define LED_PIN       5

void toggle_led(void) {
    volatile uint32_t *odr = (uint32_t *)GPIO_ODR_ADDR;
    
    /* Read-Modify-Write Pattern */
    *odr ^= (1 << LED_PIN); 
}
```
In hardware drivers, we use XOR (`^`) to toggle pins, OR (`|`) to set bits without disturbing others, and AND-NOT (`& ~`) to clear specific bits.

---

#### 4. When to use it vs alternatives
**Bitwise vs. Bitfields**
```c
/* Alternative: Bitfields (Can have compiler-specific padding/endian issues) */
struct {
    uint8_t bit0 : 1;
    uint8_t bit1 : 1;
} status;

/* Bitwise: 100% Portable and Explict */
#define STATUS_BIT0 (1 << 0)
#define STATUS_BIT1 (1 << 1)
uint8_t status_reg;
```
Bitfields are easier to read but can vary between compilers. Bitwise operations are preferred in industrial-strength drivers because they are 100% portable and give the developer total control over the memory layout.

---

#### 5. How it works internally
```c
uint8_t x = 0xA5; /* 1010 0101 */
/* sizeof(x) is 1 byte (8 bits) */

/* Left shift 1: 0100 1010 (Result: 0x4A) */
/* The most significant bit (1) is shifted into the "carry flag" */
/* A zero is shifted into the least significant bit */
```
Bitwise operations map directly to machine instructions (e.g., `AND`, `ORR`, `EOR`, `LSL` in ARM assembly). They operate on the entire word in the CPU register simultaneously.

---

#### 6. Important interview questions and answers
**Q: How do you swap two variables without a temporary variable?**
```c
x = x ^ y;
y = x ^ y;
x = x ^ y;
```
Using the XOR swap algorithm, we can swap values using only bitwise logic, saving a few bytes of stack/RAM.

**Q: How do you check if a number is a power of 2?**
```c
bool is_pow2 = (n != 0) && ((n & (n - 1)) == 0);
```
If a number is a power of 2, it only has one bit set. Subtracting 1 flips all bits after that bit. ANDing them results in 0.

---

#### 7. Common beginner mistakes
**Incorrect: Logical AND instead of Bitwise AND**
```c
if (val && 0x01) { /* Performs logical check (val is non-zero) */ }
```
**Correct: Bitwise AND for masking**
```c
if (val & 0x01) { /* Checks if LSB is specifically 1 */ }
```
Logical operators (`&&`, `||`) treat any non-zero value as `1`. Bitwise operators (`&`, `|`) look at the specific 1s and 0s.

---

#### 8. Sample Syntax and Access Pattern (MANDATORY)
```c
uint32_t reg = 0;

/* SET BIT 5 */
reg |= (1 << 5);

/* CLEAR BIT 5 */
reg &= ~(1 << 5);

/* TOGGLE BIT 5 */
reg ^= (1 << 5);

/* READ BIT 5 */
uint8_t state = (reg >> 5) & 1;

/* MULTI-BIT MASK (Setting bits 0-3 to 0xA) */
reg &= ~0x0F;      /* Clear bits 0-3 */
reg |= (0x0A & 0x0F); /* Set bits 0-3 */
```
The above patterns (Set, Clear, Toggle, Read, and Multi-bit update) are the foundation of all firmware development.

---

#### 9. Create Markdown file content for `pattern_library`
```markdown
# Pattern: Bitwise Manipulation

### Implementation
```c
#define SET_BIT(reg, bit)    ((reg) |= (1U << (bit)))
#define CLR_BIT(reg, bit)    ((reg) &= ~(1U << (bit)))
#define TOGGLE_BIT(reg, bit) ((reg) ^= (1U << (bit)))
#define READ_BIT(reg, bit)   (((reg) >> (bit)) & 1U)
```

### Technical Notes
1. **Efficiency:** These map to single-cycle CPU instructions.
2. **Atomicity:** On many architectures, `reg |= (1 << 5)` is NOT atomic (Read-Modify-Write). Use bit-banding or atomic instructions in multi-threaded contexts.
3. **Unsigned:** Always use `1U` to avoid signed integer overflow issues during shifts.
```

---

#### 10. Mental Model / Brain Tattoo
**The Digital Switchboard**
```c
reg |= (1 << 5);  /* Flip switch #5 UP */
reg &= ~(1 << 5); /* Flip switch #5 DOWN */
```
Imagine a row of 8 physical light switches. Each bitwise operator is a hand movement:
*   **OR (`|`):** Reaching in and flipping a switch **UP**. It doesn't matter if it was already up.
*   **AND-NOT (`& ~`):** Reaching in and flipping a switch **DOWN**.
*   **XOR (`^`):** Reaching in and **toggling** the switch to the opposite of its current state.
*   **AND (`&`):** Checking with a finger to see if a switch is currently up.

### **Interview Tasks: Bitwise Operators in Embedded Systems**

---

#### 1. Coding Tasks

**[Easy] Task 1: Hardware Register Macros**
*   **Problem Statement:** Create C preprocessor macros to SET, CLEAR, and TOGGLE a specific bit position in a 32-bit hardware register.
*   **Constraints:** Must safely handle 32-bit values without invoking signed integer overflow (Undefined Behavior in C).
*   **Function Signature:**
    ```c
    #define SET_BIT(REG, BIT_POS)    /* Your code here */
    #define CLEAR_BIT(REG, BIT_POS)  /* Your code here */
    #define TOGGLE_BIT(REG, BIT_POS) /* Your code here */
    ```
*   **Expected Behavior:** Replaces the target bit without altering the surrounding bits in the `REG`.

**[Easy] Task 2: Endianness / Byte Swap**
*   **Problem Statement:** An ADC sensor transmits a 16-bit value in Big-Endian format over SPI. Your microcontroller is Little-Endian. Swap the upper and lower bytes.
*   **Constraints:** Must be implemented in a single line of code using only bitwise operators.
*   **Function Signature:**
    ```c
    uint16_t hal_spi_swap_bytes(uint16_t raw_sensor_data);
    ```
*   **Expected Behavior:** `0xAABB` becomes `0xBBAA`.

**[Easy] Task 3: Interrupt Flag Check**
*   **Problem Statement:** A UART Status Register has a "Transmission Complete" (TC) flag at bit position 6. Write a function to check if this bit is set.
*   **Constraints:** Must return a strict boolean (`true` or `false`).
*   **Function Signature:**
    ```c
    bool is_uart_tx_complete(uint32_t status_register);
    ```
*   **Expected Behavior:** Returns `true` if bit 6 is `1`, otherwise `false`.

**[Medium] Task 4: Multi-Bit Field Update**
*   **Problem Statement:** A motor controller register configures the "Speed" using bits 4, 5, and 6 (a 3-bit field). Write a function to update ONLY these three bits with a new value (0 to 7) without modifying any other bits in the register.
*   **Constraints:** Must utilize the Read-Modify-Write (RMW) pattern.
*   **Function Signature:**
    ```c
    void hal_motor_set_speed(volatile uint32_t *ctrl_reg, uint8_t speed_val);
    ```
*   **Expected Behavior:** Clears the existing bits 4-6, then sets them to the corresponding bits of `speed_val`.

**[Medium] Task 5: 12-Bit Sensor Assembly**
*   **Problem Statement:** An I2C temperature sensor splits its 12-bit reading across two 8-bit registers. Register A contains the upper 8 bits. Register B contains the lower 4 bits (left-aligned in the byte). Combine them into a single 16-bit integer.
*   **Constraints:** Handle the alignment mismatch carefully.
*   **Function Signature:**
    ```c
    uint16_t combine_12bit_reading(uint8_t reg_a_msb, uint8_t reg_b_lsb);
    ```
*   **Expected Behavior:** If `reg_a` is `0xFF` and `reg_b` is `0xF0`, the output should be `0x0FFF`.

**[Medium] Task 6: Bit Reversal (SPI LSB/MSB format)**
*   **Problem Statement:** Your hardware SPI peripheral only supports MSB-first transmission, but the target display requires LSB-first. Write a function to reverse the order of bits in an 8-bit byte.
*   **Constraints:** Optimize for embedded systems (minimize branching).
*   **Function Signature:**
    ```c
    uint8_t reverse_bits(uint8_t data);
    ```
*   **Expected Behavior:** `0b10000011` becomes `0b11000001`.

**[Medium] Task 7: Hamming Weight (Set Bit Count)**
*   **Problem Statement:** To calculate a simple parity check, count the number of '1' bits in a 32-bit data payload.
*   **Constraints:** Implement an algorithm that runs in O(k) time, where 'k' is the number of set bits, rather than looping 32 times.
*   **Function Signature:**
    ```c
    uint8_t count_set_bits(uint32_t payload);
    ```
*   **Expected Behavior:** `0x0000000F` returns `4`. `0x80000000` returns `1`.

**[Medium] Task 8: Circular Bit Shift (Rotate)**
*   **Problem Statement:** Perform a circular left shift (rotate left) on an 8-bit value by `n` positions.
*   **Constraints:** Bits shifted out of the MSB must re-enter at the LSB. Handle edge cases where `n > 8`.
*   **Function Signature:**
    ```c
    uint8_t rotate_left(uint8_t value, uint8_t n);
    ```
*   **Expected Behavior:** Rotating `0b11000000` left by 1 results in `0b10000001`.

**[Hard] Task 9: Find Lowest Set Bit (Interrupt Vectoring)**
*   **Problem Statement:** An Interrupt Pending Register has multiple bits set, representing multiple pending hardware interrupts. Find the bit position (0-31) of the *lowest* set bit to service the highest priority interrupt first.
*   **Constraints:** Do not use a `for` or `while` loop that iterates 32 times. Use bitwise math to isolate the lowest set bit.
*   **Function Signature:**
    ```c
    int8_t get_highest_priority_irq(uint32_t pending_reg);
    ```
*   **Expected Behavior:** `0x000000A8` (Binary: `1010 1000`) returns `3` (since bit 3 is the lowest set bit). Return `-1` if no bits are set.

---

#### 2. Theory Questions

1.  **Memory & Types:** Why is it dangerous to perform bitwise shift operations (`<<` or `>>`) on signed integers (`int32_t`) in C? What is the difference between an Arithmetic Shift and a Logical Shift?
2.  **Edge Cases:** In a 32-bit system, what happens if you evaluate `uint32_t val = (1 << 31);`? Why do embedded coding standards (like MISRA-C) mandate `1U << 31`?
3.  **Performance:** If you need to multiply an unsigned variable by `32`, how would you rewrite this using bitwise operators, and why might a modern compiler (like GCC with `-O2`) make this manual optimization obsolete?
4.  **Logical vs. Bitwise:** Explain why `if (register_val & 0x01)` behaves fundamentally differently from `if (register_val && 0x01)`.

---

#### 3. Scenario-Based Questions

1.  **The ISR Race Condition:** You have a global `uint32_t status_flags` variable. The `main()` loop sets bit 0 using `status_flags |= (1 << 0)`. A hardware timer ISR clears bit 1 using `status_flags &= ~(1 << 1)`. Explain the exact sequence of assembly-level events where the `main()` loop's modification could be accidentally overwritten and lost. How do you solve this?
2.  **Hardware Alignment Bug:** A junior developer uses bitwise shifting to pack four 8-bit sensor readings into a 32-bit `uint32_t` buffer. Later, they cast a `uint32_t*` pointer to the middle of a `uint8_t` array to extract the data, causing a HardFault. Explain why pointer casting over bitwise extraction caused the hardware to fault.
3.  **Protocol Parsing:** You receive a raw CAN bus frame payload. The identifier is packed across the last 3 bits of byte 0 and the first 8 bits of byte 1. Describe the bitwise approach (masking and shifting) you would use to extract this 11-bit identifier into a `uint16_t` variable.

---

#### 4. Follow-up Deep Dive (Advanced)

1.  **Bit-Banding:** On ARM Cortex-M processors, what is "Bit-Banding"? How does it allow you to avoid the Read-Modify-Write (RMW) atomicity issue using a hardware memory-map alias instead of bitwise operators?
2.  **Volatile Keyword Interactions:** If `REG` is defined as a `volatile uint32_t*`, how many actual hardware memory accesses occur when the CPU executes `*REG |= (1 << 5);`? 
3.  **Optimization Trade-offs:** You implement a Bit Reversal function using a Look-Up Table (LUT) of 256 bytes vs. a loop with bitwise shifts. Contrast these two approaches specifically regarding Time Complexity, Space Complexity, and Cache Miss latency on a microcontroller with 32KB of SRAM.