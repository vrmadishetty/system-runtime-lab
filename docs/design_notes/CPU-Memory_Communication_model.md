# CPU–Memory Communication Model

# 1. Original Notes

## Basic CPU–Memory Interaction Model

### Diagram

    Instruction Bus
         ------------------------->
      +---------+            +---------+
      |   CPU   |            | Memory  |
      |         |<---------->|         |
      |         |  Data Bus  |         |
      +---------+            +---------+

    Address Bus : CPU → Memory  
    Data Bus    : CPU ↔ Memory  
    Control     : RD / WR

***

Address Bus : CPU → Memory Data Bus    : CPU ↔ Memory Control     : RD / WR
 
Two main **bus pairs** exist in this simplified system model:
 
1. **Instruction Bus Pair**
2. **Data Bus Pair**

Here is your **Basic CPU–Memory Interaction Model** rewritten in clean, structured, and properly formatted Markdown, with the ASCII diagram preserved and clarified:

 
## 1. Address Bus (Instruction Fetch)
 
During instruction execution, the CPU sends the **address of the instruction** to memory.
 
The signal lines that carry this address from **CPU → Memory** are called the **Address Bus**.
 
Key characteristics:
 
- **Unidirectional**
- Driven by the CPU
- Selects the location in memory that needs to be accessed
 
---
 
## 2. Instruction Fetch
 
Once the CPU sends the instruction address:
 
1. Memory locates the instruction.
2. Memory sends the instruction back to the CPU.
 
The instruction travels through the **instruction/data bus**.
 
Examples of instructions:
 
- Addition
- Subtraction
- Load
- Store
- Branch
 
---
 
## 3. Purpose of an Instruction
 
An instruction tells the CPU:
 
- **What operation to perform**
- **Which data to use**
- **Where the data resides**
 
Examples:
 
- Load data from memory
- Perform arithmetic operations
- Store the result back to memory
 
---
 
## 4. Data Access
 
When data is required:
 
1. The CPU places a **data address** on the **address bus**.
2. Memory accesses the location.
3. Data is transferred via the **data bus**.
 
The **data bus is bidirectional**, meaning:
 
Memory → CPU  (Read operation)
 
CPU performs computation
 
CPU → Memory  (Write operation)
 
---
 
## 5. Control Signals
 
A separate control signal determines the operation type.
 
RD / WR
 
Meaning:
 
- **RD** → Read operation
- **WR** → Write operation
 
This control signal informs memory whether the CPU wants to:
 
- Retrieve data
- Store data
 
---
 
## Key Concept
 
This model represents the **Load/Store Architecture**.
 
The CPU workflow is:
 
1. Load data from memory
2. Perform operations in registers
3. Store the result back to memory
 
---
 
# 2. System-Level Mental Model
 
All programs ultimately execute through the **CPU instruction cycle**.
 
## CPU Instruction Cycle
 
1. Fetch
 
 
2. Decode
 
 
3. Execute
 
 
4. Memory Access
 
 
5. Write Back
 
---
 
## Example Assembly Execution

Example ARM assembly sequence:

```assembly
LDR R1, [0x20000000]   ; Load value from memory
ADD R2, R1, #5         ; Perform computation
STR R2, [0x20000004]   ; Store result back to memory
```

***

## Hardware Execution Sequence

| Step | Hardware Event                                    |
| ---- | ------------------------------------------------- |
| 1    | CPU places instruction address on the address bus |
| 2    | Memory returns instruction on the data bus        |
| 3    | CPU decodes the instruction                       |
| 4    | CPU places data address on the address bus        |
| 5    | Memory returns data through the data bus          |
| 6    | CPU executes the instruction                      |
| 7    | CPU writes the result back to memory              |


***

# 3. Mapping to C Programming

Firmware engineers interact with hardware using **memory‑mapped I/O (MMIO)**.  
This means peripheral registers are accessed just like normal variables — they simply *live at fixed memory addresses*.

### Example

```c
#define GPIO_DATA (*(volatile uint32_t*)0x40020000)

GPIO_DATA = 1;
```

## Hardware Activity Triggered

When the CPU executes:

```c
GPIO_DATA = 1;
```

the following low‑level hardware operations occur:

1.  **CPU places address `0x40020000` on the Address Bus**  
    This selects the memory‑mapped GPIO register.

2.  **CPU places value `1` on the Data Bus**  
    The data to be written is prepared for transfer.

3.  **WR (Write) control signal asserted**  
    The CPU signals that the current bus cycle is a *write* operation.

4.  **Peripheral register updated**  
    The GPIO hardware latches the value and updates the output pin(s).

## What is Memory‑Mapped I/O (MMIO)?

**MMIO** is a technique where hardware registers are assigned fixed memory addresses.  
Reading or writing to those addresses directly controls hardware — no special instructions needed.

## Common Peripherals Mapped Using MMIO

*   **GPIO** (General‑Purpose I/O)
*   **UART** (Serial communication)
*   **SPI** (Synchronous serial interface)
*   **Timers**
*   **DMA Controllers**
*   **I²C**
*   **ADC / DAC**
*   **Interrupt controller registers**
 
---
 

# 4. Load / Store Architecture

Most modern **RISC processors** use a *Load/Store Architecture*.

### Examples of Load/Store ISAs

*   **ARM**
*   **RISC‑V**
*   **MIPS**

In these architectures, **only two types of instructions access memory**:

*   **LOAD**
*   **STORE**

All arithmetic operations occur **inside CPU registers**.

### Example

```assembly
LDR R1, [A]
LDR R2, [B]
ADD R3, R1, R2
STR R3, [C]
```

### Why This Design?

Register access is significantly faster than memory access because registers are:

*   **On-chip**
*   **Extremely low latency**
*   **High bandwidth**

***

# 5. RTOS Perspective

In an RTOS environment:

*   Tasks execute CPU instructions
*   Each instruction may involve memory access
*   Context switching requires saving & restoring registers

### Context Switch Behavior

*   **Registers → Saved to memory**
*   **Memory → Restored to registers**

This increases **data bus traffic**.

### Impact of Context Switching

*   **Latency**
*   **Bus utilization**
*   **Power consumption**

***

# 6. Linux System Perspective

Linux introduces **virtual memory**.

### Memory Access Path

    CPU
    ↓
    Virtual Address
    ↓
    MMU Translation
    ↓
    Physical Address
    ↓
    Memory

### Expanded System Path

    CPU
    ↓
    MMU
    ↓
    Cache
    ↓
    Memory Controller
    ↓
    DRAM

Your original CPU–memory model forms the foundation of this larger stack.

***

# 7. Cache Interaction

Modern processors use multiple cache levels.

### Typical Cache Hierarchy

    CPU
    ↓
    L1 Cache
    ↓
    L2 Cache
    ↓
    L3 Cache
    ↓
    DRAM

### Cache Miss Penalty

Accessing DRAM instead of L1 cache may cost:

*   **100–300 CPU cycles**

Efficient software aims to **maximize cache locality**.

***

# 8. Modern SoC Bus Protocols

Modern System‑on‑Chips use structured interconnect standards.

### Protocol Overview

| Protocol     | Usage                             |
| ------------ | --------------------------------- |
| **AMBA AXI** | High‑performance SoC interconnect |
| **AMBA AHB** | Microcontroller system bus        |
| **AMBA APB** | Peripheral register bus           |
| **PCIe**     | High‑speed external I/O           |

### Example SoC Interconnect

    CPU
    ↓
    AXI Interconnect
    ↓
    AHB Bus
    ↓
    APB Bus
    ↓
    Peripherals

***

# 9. Senior Insights (Silicon Interview Layer)

### How a CPU Reads Data From Memory

    CPU places address on Address Bus
    ↓
    RD signal asserted
    ↓
    Memory controller retrieves data
    ↓
    Data returned via Data Bus
    ↓
    CPU latches the data

### Typical Interview Follow‑Up Questions

*   Cache miss behavior
*   Memory latency
*   DRAM timing
*   Memory controller arbitration

***

## Load/Store Architecture Interview Trap

**Question:**

> Can an `ADD` instruction operate directly on memory?

**Incorrect form:**

    ADD [A], [B]

**Correct sequence:**

```assembly
LDR R1, [A]
LDR R2, [B]
ADD R3, R1, R2
STR R3, [C]
```

Arithmetic operations occur **only inside registers**.

***

## Memory Bandwidth

Memory bandwidth determines how much data can be transferred per second.

### Formula

    Bandwidth = Bus Width × Frequency

### Example

    64‑bit bus × 1 GHz = 8 GB/s

High bandwidth benefits:

*   GPUs
*   AI accelerators
*   High‑speed DMA

***

## Power Consumption Impact

Relative energy cost of accessing different memory tiers:

| Operation       | Relative Cost |
| --------------- | ------------- |
| Register Access | **1×**        |
| L1 Cache Access | \~**5×**      |
| DRAM Access     | \~**100×**    |

Therefore, CPUs aim to **minimize DRAM accesses**.

***

## Throughput vs Latency

Two critical memory performance metrics:

| Metric         | Meaning                         |
| -------------- | ------------------------------- |
| **Latency**    | Time for a single memory access |
| **Throughput** | Number of accesses per second   |

Examples:

*   **Cache → Low latency**
*   **DRAM → Higher latency but larger capacity**

***

# 10. Brain Tattoo Mental Model

At the most fundamental level:

    CPU
    ↓
    Address Bus
    ↓
    Memory
    ↓
    Data Bus
    ↓
    CPU

Everything else in modern processors is an **optimization layer**:

*   MMU
*   Cache
*   Bus Interconnect
*   DMA
*   Memory Controller

***

## Firmware Insight

A single C statement:

```c
x = *ptr;
```

Triggers multiple hardware events:

*   Address Bus → Memory
*   Data Bus → CPU
*   RD signal asserted
*   Cache lookup
*   Possible DRAM access

One line of C can trigger **dozens** of hardware operations.

***