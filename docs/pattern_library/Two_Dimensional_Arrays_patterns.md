# Two-Dimensional (2D) Arrays in Embedded Systems

In the embedded world, 2D arrays are more than just *tables*. They are a primary way to represent grids of physical reality—such as pixels on a display, matrices of keypad buttons, or frames of sensor data.

In embedded C, a 2D array is often called an **array of arrays**. At senior levels, the emphasis is on how these structures map to **linear physical memory** and how to access them efficiently.

---

## 1. What It Is (Simple Definition)

A **2D array** is a data structure that organizes data into rows and columns, similar to a spreadsheet or grid.

- **Syntax**:
  ```c
  uint8_t matrix[rows][columns];
  ```
- **Example**:
  ```c
  uint8_t display[8][8]; // An 8x8 LED matrix
  ```
- **Memory View**: Although visualized as a grid, a 2D array is stored as one continuous block of linear memory.

---

## 2. Why It Is Needed (Problems It Solves)

- **Spatial Mapping**: Representing data with X and Y coordinates (LED matrices, thermal images, keypads).
- **Look-Up Tables (LUTs)**: Storing precomputed results based on two variables (e.g., battery state of charge as a function of *voltage* and *temperature*).
- **Buffer Management**: Managing multiple fixed-size buffers efficiently.

  ```c
  char buffers[4][64]; // 4 UART buffers of 64 bytes each
  ```

---

## 3. Where It Is Used (Real Products / Systems)

- **Display Drivers**: Framebuffers for LCD, OLED, and LED displays.
- **Keypad Scanning**: 4×4 or 3×4 matrices representing physical keypads.
- **Digital Signal Processing (DSP)**: Image frames and mathematical matrices.
- **State Machines**: Transition tables where `table[current_state][event]` gives the next state.

---

## 4. When to Use It vs. Alternatives

### Comparison: 2D Array vs. Array of Pointers

| Feature | 2D Array (`arr[R][C]`) | Array of Pointers (`*ptr[R]`) |
|-------|------------------------|-------------------------------|
| Memory layout | Contiguous (single block) | Fragmented (multiple blocks) |
| Access speed | Faster | Slower |
| Flexibility | Fixed size at compile time | Rows can vary in size |
| Safety | High | Lower (pointer risks) |

**Senior Tip**: In bare-metal embedded systems, prefer 2D arrays whenever possible. Contiguous memory is cache-friendly and can be transferred efficiently using DMA (Direct Memory Access).

---

## 5. How It Works Internally (The Mental Model)

Physical memory is **one-dimensional**. A 2D array is flattened by the compiler using **row-major order** (C standard).

- Stored row by row: first row, then second row, and so on.
- **Address calculation** for `array[i][j]`:

  ```text
  Address = BaseAddress + (i * NumberOfColumns + j) * sizeof(Type)
  ```

Understanding this calculation is crucial for debugging and performance optimization.

---

## 6. Important Interview Questions & Answers

### Q1: How do you pass a 2D array to a function?

You must specify the **number of columns**, because the compiler needs it for address calculation.

```c
void process(uint8_t arr[][8]); // ✅ Valid
void process(uint8_t **arr);   // ❌ Invalid for true 2D array
```

---

### Q2: What is the difference between `array[3][4]` and `*(*(array + 3) + 4)`?

There is **no difference**. Both expressions access the same memory location. The second form simply makes the pointer arithmetic explicit.

✅ Writing the pointer form in interviews demonstrates strong understanding of memory mechanics.

---

### Q3: Why prefer a 2D array for a 1024×768 framebuffer?

From the CPU’s perspective, both 1D and 2D arrays occupy the same memory (786,432 bytes). The advantage of a 2D array is **human readability**:

```c
pixel = buffer[y][x];           // Clear and safe
pixel = buffer[y * width + x]; // Error-prone
```

---

## 7. Common Beginner Mistakes

1. **Index Out of Bounds**: Accessing `arr[rows][cols]` instead of `arr[rows-1][cols-1]`.
2. **Incorrect Function Passing**: Passing a fixed 2D array to a function expecting `uint8_t **`.
3. **Memory Waste**: Declaring oversized arrays (e.g., `[100][100]`) but using only a small portion.
4. **Inefficient Access Patterns**: Traversing columns in the outer loop and rows in the inner loop, which breaks cache locality and harms performance.

---

*In embedded systems, every byte and every cycle counts. A solid understanding of 2D arrays directly translates to safer, faster, and more reliable firmware.*


# 2D Array Patterns in Embedded Systems

In Embedded C, a 2D array is a **contiguous linear block of memory with a coordinate system**. For senior roles, the emphasis is on **memory layout**, **cache behavior**, and **DMA efficiency**.

---

## 1. The Mental Model: Row-Major Order

C stores 2D arrays in **row-major order**. This means `matrix[0][0]` is followed immediately by `matrix[0][1]`, **not** `matrix[1][0]`.

**Physical Memory Representation of `arr[2][3]`:**

```
[0,0] [0,1] [0,2] [1,0] [1,1] [1,2]
```

All elements live in **one continuous line of memory**.

---

## 2. Core Patterns

### A. State Transition Table

Used to implement complex state machines without deeply nested `switch` or `if` statements.

```c
typedef enum { STATE_IDLE, STATE_ACTIVE, STATE_ERROR, MAX_STATES } state_t;
typedef enum { EV_START, EV_STOP, EV_FAULT, MAX_EVENTS } event_t;

// table[current_state][event] = next_state
const state_t transition_table[MAX_STATES][MAX_EVENTS] = {
    [STATE_IDLE]   = { [EV_START] = STATE_ACTIVE, [EV_STOP] = STATE_IDLE },
    [STATE_ACTIVE] = { [EV_STOP]  = STATE_IDLE,   [EV_FAULT] = STATE_ERROR },
    [STATE_ERROR]  = { [EV_START] = STATE_IDLE } // Reset on start
};

// Senior usage
current_state = transition_table[current_state][incoming_event];
```

✅ **Why Seniors Like This Pattern**
- Deterministic O(1) transitions
- Highly readable and auditable logic
- Easy to verify against system specifications

---

### B. Look-Up Tables (LUTs)

Used for computationally expensive operations or non-linear mappings.

```c
// Pre-calculated Battery Voltage vs Temperature table
// table[temp_index][volt_index] = battery_percentage
const uint8_t battery_lut[5][10] = { /* precomputed values */ };
```

✅ Common Uses
- Trigonometric functions (sin/cos)
- Sensor linearization
- Calibration curves

---

## 3. Senior-Level Insights

### 1. DMA Compatibility

Because 2D arrays are **contiguous in RAM**, an entire row—or even the full matrix—can be transferred using **Direct Memory Access (DMA)** in a single operation.

⚠️ **Warning:** An array of pointers such as `uint8_t *arr[R]` is **not DMA-safe** because rows may be scattered in memory.

---

### 2. Cache Locality & Performance

Always place the **column index in the inner loop** to preserve cache locality.

**Correct (Fast):**
```c
for (i = 0; i < R; i++)
    for (j = 0; j < C; j++)
        sum += arr[i][j]; // Sequential memory access
```

**Incorrect (Slow):**
```c
for (j = 0; j < C; j++)
    for (i = 0; i < R; i++)
        sum += arr[i][j]; // Cache-thrashing access pattern
```

---

### 3. Pointer Decay Explained

When passed to a function, a 2D array **decays into a pointer to its first row**. This is why the number of columns must be explicitly declared.

```c
void process(int arr[][NUM_COLS]);
```

The compiler uses `NUM_COLS` to perform correct address calculations.

---

## 4. Interview Prep: High-Signal Terminology

- **Row-Major Order**: The standard C memory layout for matrices
- **Contiguous Memory**: Adjacent memory storage (critical for DMA)
- **Pointer Arithmetic**: Manual address computation: `*(base + i*cols + j)`
- **Stride**: Byte offset needed to access the same column in the next row
- **Flattening**: Treating a 2D array as 1D for serialization or speed

---

*At senior level, mastering 2D arrays is not about syntax—it is about predictability, performance, and control over hardware.*
