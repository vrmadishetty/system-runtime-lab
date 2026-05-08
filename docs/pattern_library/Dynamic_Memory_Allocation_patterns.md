# Dynamic Memory Allocation in Embedded Systems

Dynamic memory allocation is a **make-or-break topic** for Senior and Staff embedded roles. While `malloc()` and `free()` are routine in desktop software, their use in embedded firmware is **highly controversial** and often **strictly regulated** due to safety, determinism, and long-term reliability concerns.

---

## 1. What It Is (Simple Definition)

**Dynamic Memory Allocation** is the process of reserving and releasing memory at **runtime** from a dedicated region of RAM called the **heap**, instead of at compile time.

- **Key APIs**:
  ```c
  void* malloc(size_t size);
  void* calloc(size_t n, size_t size);
  void* realloc(void* ptr, size_t new_size);
  void  free(void* ptr);
  ```

---

## 2. Why It Is Needed (Problems It Solves)

Dynamic allocation is used only when other options are insufficient:

- **Variable Data Sizes**: When data size is unknown at compile time (e.g., Wi‑Fi packets, JSON payloads).
- **Memory Reuse**: Reclaiming RAM from Task A and reusing it for Task B in extremely constrained systems.
- **Complex Data Structures**: Linked lists, trees, graphs, or queues where nodes are added and removed dynamically.

---

## 3. Where It Is Used (and Not Used)

### Common Usage
- **Embedded Linux / High-End RTOS**: Networking stacks (TCP/IP), file systems, UI frameworks.
- **Gateways & IoT Hubs**: Devices handling variable numbers of connections or clients.

### Explicitly Avoided
- **Safety-Critical Systems**: Automotive (ISO 26262), Medical Devices, Avionics.

❌ The reason: **Non-deterministic timing**, fragmentation, and failure modes that appear months after deployment.

---

## 4. When to Use It vs. Alternatives

### Memory Allocation Trade-Offs

| Feature | Static Allocation | Stack Allocation | Dynamic (Heap) |
|-------|-------------------|------------------|---------------|
| Lifecycle | Program lifetime | Function scope | Manual (`malloc` → `free`) |
| Speed | Fastest | Very fast | Slowest |
| Safety | Highest | Moderate | Lowest |
| Flexibility | None | Low | High |

### ✅ Senior Rule of Thumb
1. **Static** → Buffers you always need
2. **Stack** → Small, temporary data
3. **Heap** → Only if size is unpredictable or too large for stack

---

## 5. How Dynamic Allocation Works Internally (High Level)

1. **Heap Region**: A contiguous block of RAM defined by the **linker script**.
2. **Allocator**: A management algorithm (e.g., `dlmalloc`) tracks free and used blocks.
3. **Allocation**: `malloc(size)` searches for a block large enough; may split blocks.
4. **Freeing**: `free(ptr)` returns memory to the heap and attempts **coalescing** with neighbors.

---

## 6. Important Interview Questions & Answers

### Q1: What is Fragmentation and why is it deadly in embedded systems?

**Fragmentation** occurs when free memory exists in many small, non-contiguous blocks.

- You may have **10 KB free total**
- But no single **1 KB continuous block**

✅ Result: `malloc(1024)` fails

⚠️ In long-running devices (smart meters, routers), fragmentation can crash systems **months after deployment**.

---

### Q2: How do you solve non-deterministic `malloc()` timing?

✅ Use **Memory Pools (Fixed-Size Block Allocators)**.

- Pre-allocate fixed-size blocks
- Allocation is **O(1)**
- Zero fragmentation

This makes memory usage **RTOS-safe** and deterministic.

---

### Q3: What is a Memory Leak?

A **memory leak** happens when:

```c
ptr = malloc(100);
// free(ptr); ← missing!
```

In embedded systems:
- The program never exits
- The OS does not reclaim memory
- Leaks are **fatal over time**

---

## 7. Common Beginner Mistakes

1. **Ignoring NULL**:
   ```c
   ptr = malloc(100);
   // Using ptr without checking ptr == NULL
   ```
2. **Use-After-Free**: Accessing memory after `free(ptr)`.
3. **Dangling Pointers**: Not setting `ptr = NULL` after freeing.
4. **Over-Allocation**: Using heap for data better suited to stack or static memory.

---

# Memory Management Patterns in Embedded C

Embedded memory management prioritizes **predictability**. At senior levels, the goal is to avoid fragmentation-driven crashes and non-deterministic execution.

---

## 1. Allocation Tiers (What / Why / When)

| Pattern | Mechanism | Timing | Best For |
|-------|-----------|--------|----------|
| **Static** | Global / static vars | Compile-time | Drivers, kernel objects, fixed buffers |
| **Stack** | Local variables | Automatic | Temporary calculations |
| **Heap** | `malloc` / `free` | Manual | Truly unpredictable sizes |

---

## 2. Dynamic Allocation: Senior-Safe Alternatives

### A. Memory Pools (Fixed Block Allocator)

Instead of a single heap, create **pre-allocated fixed-size blocks**.

✅ Advantages
- Zero fragmentation
- Deterministic timing
- RTOS friendly

```c
#define POOL_SIZE  10
#define BLOCK_SIZE 32

typedef struct {
    uint8_t data[BLOCK_SIZE];
    bool is_free;
} MemoryBlock_t;

MemoryBlock_t pool[POOL_SIZE]; // Statistically allocated pool
```

Used for RTOS messages, packet buffers, task objects.

---

### B. Static Allocation with Worst-Case Limits

Allocate the **maximum possible size** at compile time.

✅ Benefits
- Compile-time memory guarantees
- No runtime failure paths

✅ Used for
- Command buffers
- Sensor data arrays

---

## 3. Industrial-Strength Checklist

1. ✅ **Check for NULL** after every allocation
2. ✅ **Use bounded APIs** (`snprintf` over `sprintf`)
3. ✅ **Define ownership** of allocated memory
4. ❌ **Never use heap in ISRs** (non-deterministic + unsafe)

---

## 4. Interview Prep: High-Signal Terminology

- **Fragmentation**: "Swiss cheese" heap memory
- **Memory Leak**: Allocated but never freed memory
- **Deterministic**: Predictable execution time
- **Dangling Pointer**: Pointer to freed memory
- **Stack Overflow**: Exceeding stack boundaries due to deep calls or large locals

---

*At senior level, the best dynamic allocation strategy is often knowing when **not** to use it.*
