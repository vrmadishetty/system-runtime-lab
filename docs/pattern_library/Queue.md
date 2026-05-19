### **Queue (Circular Buffer) in Embedded Systems**

---

#### 1. What it is
```c
#define QUEUE_SIZE 8

typedef struct {
    uint8_t data[QUEUE_SIZE];
    uint8_t head;  /* Index for next extraction */
    uint8_t tail;  /* Index for next insertion */
    uint8_t count; /* Number of items currently in queue */
} Queue_t;

Queue_t rx_queue = { .head = 0, .tail = 0, .count = 0 };
```
A Queue is a First-In-First-Out (FIFO) linear data structure. In embedded systems, it is most commonly implemented as a **Circular Buffer** to allow continuous data flow using a fixed-size array without shifting memory.

---

#### 2. Why it is needed
**The Problem: Data Loss during Bursts**
```c
/* Global variable can only hold ONE byte */
volatile uint8_t rx_byte; 

void UART_ISR(void) {
    rx_byte = UART_DR; /* If a second byte arrives before main() reads, data is lost */
}
```
Without a queue, if the CPU is busy with a long task and multiple interrupts occur, only the last received byte is preserved.

**The Solution: Buffering the Burst**
```c
void UART_ISR_Fixed(void) {
    if (rx_queue.count < QUEUE_SIZE) {
        rx_queue.data[rx_queue.tail] = UART_DR;
        rx_queue.tail = (rx_queue.tail + 1) % QUEUE_SIZE;
        rx_queue.count++;
    }
}
```
The queue acts as a shock absorber. It stores incoming bytes during high-speed bursts, allowing the background `main()` loop to process them at its own pace without losing information.

---

#### 3. Where it is used
**UART Driver Ring Buffer**
```c
typedef struct {
    uint8_t storage[128];
    volatile uint16_t head;
    volatile uint16_t tail;
} UART_RingBuffer_t;

void UART_PutChar(UART_RingBuffer_t *rb, uint8_t c) {
    uint16_t next = (rb->tail + 1) % 128;
    if (next != rb->head) {
        rb->storage[rb->tail] = c;
        rb->tail = next;
    }
}
```
Queues are the standard mechanism for peripheral drivers (UART, CAN, SPI). They decouple the high-speed hardware interrupts from the lower-speed application logic.

---

#### 4. When to use it vs. Alternatives
**Queue (FIFO) vs. Stack (LIFO)**
```c
/* Queue: process oldest first (Serial data) */
uint8_t first_in = queue.buffer[head]; 

/* Stack: process newest first (Function calls/Undo) */
uint8_t last_in = stack.buffer[top--]; 
```
Use a **Queue** when the order of events must be preserved (Processing commands). Use a **Stack** when you need to "backtrack" or handle nested events (Recursive parsing).

---

#### 5. How it works internally
```c
typedef struct {
    uint32_t buffer[4]; /* Offset 0, Size 16 */
    uint8_t head;       /* Offset 16, Size 1 */
    uint8_t tail;       /* Offset 17, Size 1 */
    /* 2 bytes padding added here for 32-bit alignment */
} InternalQueue_t;

size_t q_size = sizeof(InternalQueue_t); /* Returns 20 */
```
The "Circular" behavior is achieved using the **Modulo (%) operator** or bitmasking. When `tail` reaches the end of the array, it wraps back to index 0. This creates an infinite loop within a finite memory block.

---

#### 6. Important interview questions and answers
**Q: How do you check if a circular queue is full?**
```c
bool is_full(Queue_t *q) {
    return q->count == QUEUE_SIZE;
}
```
In an interview, you can use a `count` variable (as shown above) or leave one slot empty and check if `(tail + 1) % SIZE == head`.

**Q: Is a simple circular buffer thread-safe?**
```c
/* ATOMICITY PROBLEM */
q->count++; /* This is read-modify-write; can be interrupted! */
```
No. If an ISR interrupts the main loop while updating `count`, the value can become corrupted. A Senior dev uses `atomic` blocks or separate `head`/`tail` indices without a shared `count`.

---

#### 7. Common beginner mistakes
**Incorrect: Forgetting to wrap the index**
```c
void enqueue_bad(Queue_t *q, uint8_t val) {
    q->data[q->tail++] = val; /* ERROR: tail will go past array bounds */
}
```
**Correct: Using modulo for circularity**
```c
void enqueue_good(Queue_t *q, uint8_t val) {
    q->data[q->tail] = val;
    q->tail = (q->tail + 1) % QUEUE_SIZE; /* SUCCESS: wraps to 0 */
}
```

---

#### 8. Sample Syntax and Access Pattern (MANDATORY)
```c
/* 1. Definition */
typedef struct {
    int buffer[5];
    int head;
    int tail;
} FastQueue_t;

/* 2. Variable and Pointer */
FastQueue_t my_q = { .head = 0, .tail = 0 };
FastQueue_t *pq = &my_q;

/* 3. Enqueue Access */
pq->buffer[pq->tail] = 100;
pq->tail = (pq->tail + 1) % 5;

/* 4. Dequeue Access */
int val = pq->buffer[pq->head];
pq->head = (pq->head + 1) % 5;

/* 5. Function Passing */
bool queue_is_empty(const FastQueue_t *q) {
    return q->head == q->tail;
}
```

---

#### 9. Create Markdown file content for `pattern_library`
```markdown
# Pattern: Circular Buffer (Queue)

### Implementation
```c
typedef struct {
    uint8_t *buffer;
    uint16_t size;
    uint16_t head;
    uint16_t tail;
} RingBuf_t;

bool ring_buf_put(RingBuf_t *rb, uint8_t data) {
    uint16_t next = (rb->tail + 1) % rb->size;
    if (next == rb->head) return false; // Full
    rb->buffer[rb->tail] = data;
    rb->tail = next;
    return true;
}
```

### Technical Notes
1. **Interrupt Safety:** Mark `head` and `tail` as `volatile` if shared between ISR and Main.
2. **Power of 2 Optimization:** If `size` is 2, 4, 8... use `(tail + 1) & (size - 1)` instead of modulo for speed.
3. **No-Copy Pattern:** For large structs, store pointers in the queue rather than the whole struct.
```

---

#### 10. Mental Model / Brain Tattoo
**The Circular Conveyor Belt**
```c
idx = (idx + 1) % SIZE;
```
A queue is like a **Circular Conveyor Belt**. Items are placed at the **Tail** and picked up from the **Head**. The belt never ends because it loops back on itself. If the Tail catches up to the Head, the belt is **Full**. If the Head catches up to the Tail, the belt is **Empty**.

### **Interview Tasks: Queues (Circular Buffers) in Embedded Systems**

---

#### 1. Coding Tasks

**[Easy] Task 1: Basic Enqueue Operation**
*   **Problem Statement:** Implement a function to insert a single byte into a fixed-size circular buffer. If the buffer is full, the function must return `false` and not modify the buffer.
*   **Constraints:** Buffer size is fixed at 128 bytes. Do not use dynamic memory.
*   **Function Signature:**
    ```c
    typedef struct {
        uint8_t buffer[128];
        uint16_t head;
        uint16_t tail;
    } RingBuf_t;

    bool ring_buf_put(RingBuf_t *rb, uint8_t data);
    ```
*   **Expected Behavior:** Inserts `data` at the `tail` index, increments `tail` wrapping around to 0 when necessary, and returns `true` on success.

**[Easy] Task 2: Queue Status Check**
*   **Problem Statement:** Implement a function to calculate exactly how many bytes are currently stored in the circular buffer.
*   **Constraints:** The queue uses the "keep one slot open" method to differentiate full from empty.
*   **Function Signature:**
    ```c
    uint16_t ring_buf_get_count(const RingBuf_t *rb);
    ```
*   **Expected Behavior:** Returns the mathematical difference between `tail` and `head`, accounting for the wrap-around condition.

**[Medium] Task 3: Power-of-Two Optimization**
*   **Problem Statement:** You are writing an ultra-fast queue for an Audio I2S driver. The modulo operator (`%`) takes too many clock cycles on your Cortex-M0. Rewrite the enqueue logic using bitwise operations.
*   **Constraints:** The queue size is guaranteed to be a power of two (e.g., 256).
*   **Function Signature:**
    ```c
    #define AUDIO_BUF_SIZE 256
    typedef struct {
        uint16_t buffer[AUDIO_BUF_SIZE];
        uint32_t head;
        uint32_t tail;
    } AudioQueue_t;

    void audio_queue_fast_put(AudioQueue_t *q, uint16_t sample);
    ```
*   **Expected Behavior:** Inserts the sample and wraps the `tail` index using only bitwise AND (`&`) and addition (`+`).

**[Medium] Task 4: Block Dequeue (Chunk Reading)**
*   **Problem Statement:** A DMA controller needs 16 bytes at a time, but reading byte-by-byte from the queue is too slow. Write a function that extracts a block of data from the queue into a destination array.
*   **Constraints:** You must use `memcpy` for speed. You must handle the scenario where the 16 bytes are split across the "wrap-around" boundary of the circular buffer.
*   **Function Signature:**
    ```c
    /* Returns the actual number of bytes copied (may be less than len if queue is almost empty) */
    uint16_t ring_buf_read_block(RingBuf_t *rb, uint8_t *dest, uint16_t len);
    ```
*   **Expected Behavior:** Copies up to `len` bytes into `dest`. If the data wraps from the end of the buffer to the beginning, two separate `memcpy` calls may be required.

**[Hard] Task 5: Lock-Free ISR Producer / Main Consumer**
*   **Problem Statement:** A UART ISR (Interrupt Service Routine) inserts data into the queue. The `main()` loop reads it. You cannot disable interrupts (`__disable_irq()`) in the `main()` loop because it increases latency.
*   **Constraints:** Implement the `put` and `get` functions such that they are safe from race conditions without using mutexes or disabling interrupts.
*   **Function Signatures:**
    ```c
    typedef struct {
        uint8_t buffer[64];
        volatile uint8_t head;
        volatile uint8_t tail;
    } SafeQueue_t;

    /* Called ONLY from ISR */
    void isr_safe_put(SafeQueue_t *q, uint8_t data);

    /* Called ONLY from Main Loop */
    bool main_safe_get(SafeQueue_t *q, uint8_t *out_data);
    ```
*   **Expected Behavior:** Memory ordering and index updating must be done in a specific sequence to prevent the ISR from overwriting data before `main()` has fully updated the `head` pointer.

**[Hard] Task 6: Queue of Pointers (Zero-Copy)**
*   **Problem Statement:** You are passing large TCP/IP packets (1500 bytes each) between two RTOS tasks. Copying the packets into the queue takes too much CPU time. Design a queue that holds *pointers* to the packets instead of the data itself.
*   **Constraints:** The queue holds up to 10 pointers. Provide the structure definition and the enqueue function.
*   **Function Signature:**
    ```c
    typedef struct {
        uint8_t payload[1500];
        uint32_t len;
    } NetPacket_t;

    typedef struct {
        /* Define your buffer array here */
        /* Define your indices here */
    } PtrQueue_t;

    bool ptr_queue_put(PtrQueue_t *q, NetPacket_t *packet_ptr);
    ```
*   **Expected Behavior:** The function stores the 32-bit address of the packet, not the 1500 bytes of data.

---

#### 2. Theory Questions

*   **[Easy]** Why is a circular buffer preferred over a standard array for streaming data like UART RX?
*   **[Easy]** If a circular buffer has a size of 10, why can it usually only hold 9 items?
*   **[Medium]** What is the difference between a Queue and a Stack, and name one embedded hardware component that uses a Stack internally.
*   **[Medium]** In the struct `typedef struct { uint8_t buf[10]; uint16_t head; uint16_t tail; }`, what is the `sizeof` this struct on a 32-bit ARM processor, and why is it not 14?
*   **[Hard]** Why is the modulo operator (`%`) considered "expensive" on basic microcontrollers (like an 8-bit AVR or Cortex-M0), and how do we avoid it in queue implementations?

---

#### 3. Scenario-Based Questions

*   **Scenario A:** You observe that your circular buffer works perfectly 99% of the time, but occasionally, the `main()` loop reads a corrupted byte. The queue is filled by an ISR. What is the most likely cause, and how do you fix it?
*   **Scenario B:** You are implementing a Queue to hold commands for an e-ink display. The display takes 500ms to update. The user presses buttons rapidly, filling the queue. What should your `enqueue` function do when the queue is full? Discard the oldest command, or ignore the new command? Justify your architectural choice.
*   **Scenario C:** You define `uint8_t head;` and `uint8_t tail;` for a queue that is 256 bytes large. The system crashes when it fills up. Why?

---

#### 4. Follow-up Deep Dive (Advanced)

*   **Memory Layout:** If you place a `uint8_t buffer[3]` before a `uint32_t tail` inside a struct, what impact does struct padding have if this queue is placed in a packed communication packet?
*   **Optimization Trade-offs:** You are asked to write a queue. Approach 1 uses a `count` variable. Approach 2 does not use `count` but checks `(tail + 1) % size == head`. Which approach is more "ISR Safe" and why?
*   **Volatile keyword:** In a single-core embedded system without an RTOS, if `head` and `tail` are NOT marked `volatile`, describe the exact sequence of assembly-level events where the `main()` loop might get stuck in an infinite `while(!queue_is_empty(&q))` loop, even if the ISR is receiving data.


Here are the detailed solutions for the coding tasks, strictly adhering to Embedded C standards.

---

### **Task 1: Basic Enqueue Operation**

```c
#include <stdint.h>
#include <stdbool.h>

#define BUF_SIZE 128

typedef struct {
    uint8_t buffer[BUF_SIZE];
    uint16_t head;
    uint16_t tail;
} RingBuf_t;

bool ring_buf_put(RingBuf_t *rb, uint8_t data) {
    if (rb == NULL) {
        return false;
    }

    uint16_t next_tail = (rb->tail + 1) % BUF_SIZE;

    /* If next_tail equals head, the buffer is full */
    if (next_tail == rb->head) {
        return false; 
    }

    rb->buffer[rb->tail] = data;
    rb->tail = next_tail;
    
    return true;
}
```
**Explanation:** 
This uses the "keep one slot open" method to detect a full queue. We calculate the `next_tail` index. If `next_tail` matches `head`, inserting data would overwrite the oldest unread byte, so we reject it. We only update `rb->tail` *after* the data is written, which is a crucial habit for ISR safety.

---

### **Task 2: Queue Status Check**

```c
uint16_t ring_buf_get_count(const RingBuf_t *rb) {
    if (rb == NULL) {
        return 0;
    }

    if (rb->tail >= rb->head) {
        return rb->tail - rb->head;
    } else {
        /* Tail has wrapped around, but head has not */
        return (BUF_SIZE - rb->head) + rb->tail;
    }
}
```
**Explanation:** 
Because it's a circular buffer, `tail` is not always numerically greater than `head`. If `tail` wraps back to 0 while `head` is still at 120, a simple `tail - head` results in a negative number (which underflows `uint16_t`). We must handle the wrap-around case by adding the distance from `head` to the end of the buffer, plus the distance from the start of the buffer to `tail`.

---

### **Task 3: Power-of-Two Optimization**

```c
#define AUDIO_BUF_SIZE 256
/* Mask is Size - 1. Because 256 is a power of 2, 255 is 0x00FF */
#define AUDIO_BUF_MASK (AUDIO_BUF_SIZE - 1) 

typedef struct {
    uint16_t buffer[AUDIO_BUF_SIZE];
    uint32_t head;
    uint32_t tail;
} AudioQueue_t;

void audio_queue_fast_put(AudioQueue_t *q, uint16_t sample) {
    /* Fast wrap-around using Bitwise AND instead of Modulo */
    uint32_t next_tail = (q->tail + 1) & AUDIO_BUF_MASK;

    if (next_tail != q->head) {
        q->buffer[q->tail] = sample;
        q->tail = next_tail;
    }
}
```
**Explanation:** 
Division and Modulo (`%`) instructions can take 10-40 CPU cycles on Cortex-M0/M3. A bitwise AND (`&`) takes 1 cycle. If the size is 256 (`0x100`), then size-1 is 255 (`0xFF`). `(255 + 1) & 0xFF` equals `0`, perfectly mimicking modulo arithmetic with zero hardware division cost.

---

### **Task 4: Block Dequeue (Chunk Reading)**

```c
#include <string.h> /* For memcpy */

uint16_t ring_buf_read_block(RingBuf_t *rb, uint8_t *dest, uint16_t len) {
    uint16_t count = ring_buf_get_count(rb);
    
    /* Can only read what is available */
    if (len > count) {
        len = count;
    }
    
    if (len == 0) return 0;

    /* Case 1: Contiguous read (no wrap-around needed for this chunk) */
    if (rb->head < rb->tail || (BUF_SIZE - rb->head) >= len) {
        memcpy(dest, &rb->buffer[rb->head], len);
        rb->head = (rb->head + len) % BUF_SIZE;
    } 
    /* Case 2: Split read (Data wraps around the end of the array) */
    else {
        uint16_t bytes_to_end = BUF_SIZE - rb->head;
        uint16_t bytes_at_start = len - bytes_to_end;

        /* First chunk: from head to the physical end of the array */
        memcpy(dest, &rb->buffer[rb->head], bytes_to_end);
        
        /* Second chunk: from physical start of array to the remaining len */
        memcpy(dest + bytes_to_end, &rb->buffer[0], bytes_at_start);
        
        rb->head = bytes_at_start; /* New head is naturally bytes_at_start */
    }

    return len;
}
```
**Explanation:** 
Using `memcpy` is fast, but it requires contiguous physical memory. If `head` is at index 126, and we want 5 bytes, we must `memcpy` 2 bytes from index 126 to 127, and then make a *second* `memcpy` call for the remaining 3 bytes starting at index 0.

---

### **Task 5: Lock-Free ISR Producer / Main Consumer**

```c
typedef struct {
    uint8_t buffer[64];
    volatile uint8_t head;
    volatile uint8_t tail;
} SafeQueue_t;

/* PRODUCER: Only modifies 'tail' */
void isr_safe_put(SafeQueue_t *q, uint8_t data) {
    uint8_t next_tail = (q->tail + 1) % 64;
    
    if (next_tail != q->head) {
        /* CRITICAL ORDERING: Write data FIRST */
        q->buffer[q->tail] = data;
        
        /* Memory Barrier recommended here on Cortex-M7/M4: __DMB(); */
        
        /* Update tail LAST. Main loop won't see data until this line executes */
        q->tail = next_tail; 
    }
}

/* CONSUMER: Only modifies 'head' */
bool main_safe_get(SafeQueue_t *q, uint8_t *out_data) {
    if (q->head == q->tail) {
        return false; /* Empty */
    }
    
    /* CRITICAL ORDERING: Read data FIRST */
    *out_data = q->buffer[q->head];
    
    /* Memory Barrier recommended here: __DMB(); */
    
    /* Update head LAST. ISR won't see free space until this line executes */
    q->head = (q->head + 1) % 64;
    
    return true;
}
```
**Explanation:** 
This is a Single-Producer Single-Consumer (SPSC) lock-free queue. It works because the ISR *only* writes to `tail`, and `main` *only* writes to `head`. As long as we read/write the payload *before* updating the indices, the other side will never read invalid data or overwrite valid data. No global `count` variable is used, as `count` requires Read-Modify-Write, which is not atomic.

---

### **Task 6: Queue of Pointers (Zero-Copy)**

```c
typedef struct {
    uint8_t payload[1500];
    uint32_t len;
} NetPacket_t;

#define PTR_QUEUE_SIZE 10

typedef struct {
    NetPacket_t *buffer[PTR_QUEUE_SIZE]; /* Array of POINTERS */
    uint16_t head;
    uint16_t tail;
} PtrQueue_t;

bool ptr_queue_put(PtrQueue_t *q, NetPacket_t *packet_ptr) {
    if (q == NULL || packet_ptr == NULL) return false;

    uint16_t next_tail = (q->tail + 1) % PTR_QUEUE_SIZE;
    
    if (next_tail == q->head) {
        return false; /* Full */
    }

    q->buffer[q->tail] = packet_ptr; /* Store only the 4-byte address */
    q->tail = next_tail;
    
    return true;
}
```
**Explanation:** 
Instead of a `uint8_t buffer[]`, we declare `NetPacket_t *buffer[]`. When enqueueing, we don't copy 1500 bytes. We simply copy the 32-bit (4-byte) memory address of the packet into the queue. This is known as "Zero-Copy" architecture and is mandatory for high-performance networking in embedded Linux or RTOS.