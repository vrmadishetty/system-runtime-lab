## Goal

Understand how **C variables** map to **assembly instructions**, focusing on how the compiler places data in memory and how the CPU accesses it.

***

## Topics

This module covers:

*   **Stack allocation**  
    How local variables are placed on the stack and accessed via stack/frame pointers.

*   **Pointer dereferencing**  
    How pointer reads/writes translate into load/store instructions.

*   **Volatile behavior**  
    Why `volatile` forces the compiler to always emit memory accesses.

***

## Commands

### Build

```bash
make
```

### Generate Assembly Output

```bash
make assembly
```

### Debug with GDB

```bash
make debug
```