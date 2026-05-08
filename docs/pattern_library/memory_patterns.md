# Memory Management Patterns in Embedded C

Embedded memory management is about **Predictability**. In Senior roles, the goal is to avoid non-deterministic behavior and system-wide crashes caused by fragmentation.

---

## 1. The Allocation Tiers (What/Why/When)

| Pattern | Mechanism | Timing | Best For |
| :--- | :--- | :--- | :--- |
| **Static** | Global/Static variables | Compile-Time | Fixed buffers, HAL drivers, OS kernel. |
| **Stack** | Local variables | Runtime (Auto) | Temporary calculations, small local structures. |
| **Heap** | `malloc` / `free` | Runtime (Man) | Data with unpredictable size (e.g., JSON parsing). |

---

## 2. Dynamic Allocation: The "Senior" Alternatives

Standard `malloc` is often dangerous in long-running embedded systems due to **Fragmentation**. We use these patterns instead:

### A. Memory Pools (Fixed-Block Allocator)
Instead of one big heap, we create an array of fixed-size blocks.
*   **Why:** Zero fragmentation, O(1) constant time allocation.
*   **When:** Used for RTOS messages, network packets, or task structures.

```c
#define POOL_SIZE 10
#define BLOCK_SIZE 32

typedef struct {
    uint8_t data[BLOCK_SIZE];
    bool is_free;
} MemoryBlock_t;

MemoryBlock_t pool[POOL_SIZE]; // Static allocation of the pool!
```

### B. Static Allocation with Max Limits
Instead of allocating exactly what you need at runtime, you allocate the **Worst-Case Scenario** at compile time.
*   **Why:** Guarantees that if the code compiles, it will never run out of memory.
*   **When:** Command buffers, sensor data arrays.

---

## 3. Industrial Strength Checklist

1.  **Check for NULL:** Never use a pointer from `malloc` without `if (ptr != NULL)`.
2.  **Size Limits:** Always use `n` versions of functions (e.g., `snprintf` vs `sprintf`) to prevent buffer overflows.
3.  **Ownership:** Clearly define which function is responsible for `free()`-ing a pointer. 
4.  **Avoid Heap in ISRs:** Never call `malloc` or `free` inside an Interrupt Service Routine. They are usually not thread-safe and take too long.

---

## 4. Interview Prep: High-Signal Terminology

*   **Fragmentation:** The "Swiss Cheese" effect where free memory is broken into unusable small pieces.
*   **Memory Leak:** Allocated memory that is never freed, eventually "choking" the system.
*   **Deterministic:** Code that always takes the same amount of time to run (Critical for Hard Real-Time).
*   **Dangling Pointer:** A pointer still holding the address of memory that has already been `free()`-ed.
*   **Stack Overflow:** When too many nested function calls or large local variables exceed the stack boundary.
