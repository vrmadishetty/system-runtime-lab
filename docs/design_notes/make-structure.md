
### Top: Toolchain variables

```make
CROSS   = riscv64-unknown-elf-
CC      = $(CROSS)gcc
OBJCOPY = $(CROSS)objcopy
OBJDUMP = $(CROSS)objdump
```

*   **What:** Centralizes the toolchain prefix. `CC`/`OBJCOPY`/`OBJDUMP` expand from it.
*   **Why:** Easy to switch to `riscv32-unknown-elf-` later; avoids hardcoding everywhere.
*   **Tip:** If you do get a dedicated RV32 toolchain, change `CROSS` only.

***

### Project layout root

```make
ROOT = ../../..
```

*   **What:** Points from the Makefile’s directory to the repo root.
*   **Why:** Lets you reference startup code and linker script stored outside the local dir:
    *   `$(ROOT)/platform/riscv/qemu/startup/start.S`
    *   `$(ROOT)/platform/riscv/qemu/linker/memory.ld`
*   **Pitfall:** Wrong relative depth = “No such file or directory.” Validate with `ls $(ROOT)/...`.

***

### Flags (C and Link)

```make
CFLAGS  = -O0 -ggdb -march=rv32i -mabi=ilp32 -ffreestanding -fno-builtin
LDFLAGS = -nostdlib -Wl,-melf32lriscv -T $(ROOT)/platform/riscv/qemu/linker/memory.ld
```

*   **`-O0 -ggdb`**: No optimization + full debug info. Great for GDB and stepping through code.
*   **`-march=rv32i -mabi=ilp32`**: Compile for RV32 base ISA, ILP32 ABI.
*   **`-ffreestanding -fno-builtin`**: Don’t assume a hosted C environment; avoid implicit libc/builtins.
*   **`-nostdlib` (linker)**: Don’t link standard runtime or libc; you provide `_start` and link script.
*   **`-Wl,-melf32lriscv`**: (Key!) Forces **32-bit linker emulation** even when using a `riscv64-...` driver.
*   **`-T memory.ld`**: Uses your custom linker script to place sections (text/data/bss, stack, etc.).
*   **Reason the structured Makefile initially failed:** You compiled RV32 objects, but at **link time** the driver defaulted to RV64 emulation. Passing `-march/-mabi` and/or `-Wl,-melf32lriscv` at **link** fixes it.

***

### Sources and includes

```make
STARTUP = $(ROOT)/platform/riscv/qemu/startup/start.S
SRC_DIR = ./src
INC_DIR = ./include
SRC     = $(wildcard $(SRC_DIR)/*.c)
HEADERS = $(wildcard $(INC_DIR)/*.h)
```

*   **What:** Organized paths and dynamic discovery of `.c` and `.h`.
*   **Why:** Automatically picks up new `.c` files; you don’t edit lists by hand.
*   **Tip:** `-I$(INC_DIR)` is added to compile commands so headers resolve.

***

### Build artifacts and object list

```make
BUILD = build
OBJS  = $(BUILD)/start.o $(patsubst $(SRC_DIR)/%.c,$(BUILD)/%.o,$(SRC))
ELF   = $(BUILD)/main.elf
BIN   = $(BUILD)/main.bin
DUMP  = $(BUILD)/main.dump
```

*   **What:** All outputs go under `build/` to keep your tree clean.
*   **`$(patsubst ...)`**: Transforms `src/foo.c` → `build/foo.o` for every C source.
*   **Why:** Scalable as you add files; keeps a simple naming scheme.

***

### Default target

```make
.PHONY: all
all: $(ELF) $(BIN)
```

*   **What:** `make` builds the ELF and the BIN by default.
*   **Why:** The ELF is for QEMU/GDB; BIN is useful for raw flashes or inspection.
*   **`.PHONY`**: Declares `all` isn’t a file; always runs its dependencies when asked.

***

### Ensure build directory exists

```make
$(BUILD):
	@mkdir -p $(BUILD)
```

*   **What:** Creates `build/` on demand.
*   **Why:** Prevents “No such file or directory” when emitting objects.
*   **Note:** It’s used as an **order-only prerequisite** (`| $(BUILD)`) in later rules—meaning “make sure the dir exists, but don’t force rebuilds just because the dir is newer.”

***

### Compile startup assembly

```make
$(BUILD)/start.o: $(STARTUP) | $(BUILD)
	$(CC) $(CFLAGS) -I$(INC_DIR) -c $< -o $@
```

*   **What:** Builds `start.S` into `build/start.o`.
*   **`$<`**: First prerequisite (`start.S`); **`$@`**: target (`build/start.o`).
*   **Why:** `_start` lives here; critical when using `-nostdlib`.

***

### Compile C sources (pattern rule)

```make
$(BUILD)/%.o: $(SRC_DIR)/%.c $(HEADERS) | $(BUILD)
	$(CC) $(CFLAGS) -I$(INC_DIR) -c $< -o $@
```

*   **What:** For every `src/foo.c`, produce `build/foo.o`.
*   **Why:** You get per-file compilation (faster incremental builds) instead of trying to compile all C files into a single `main.o`.
*   **Including `$(HEADERS)` as a dep:** Triggers recompilation if headers change (simple but may over-rebuild; for fine-grained deps you can use `-MMD -MP` later).

***

### Link step

```make
$(ELF): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(LDFLAGS) -o $@
```

*   **What:** Links all objects into `build/main.elf`.
*   **Why:** Using `gcc` as the driver pulls in the right linker and passes flags.
*   **Important:** We include `$(CFLAGS)` here so `-march/-mabi` are present at **link time**, mirroring your earlier single-line command. Combined with `-Wl,-melf32lriscv`, this guarantees **RV32** emulation.

***

### Convert ELF → BIN

```make
$(BIN): $(ELF)
	$(OBJCOPY) -O binary $< $@
```

*   **What:** Raw binary image (no ELF headers), handy for device programming or hex dumps.

***

### Disassembly (optional)

```make
.PHONY: dump
dump: $(ELF)
	$(OBJDUMP) -d $< > $(DUMP)
	@echo "Wrote $(DUMP)"
```

*   **What:** Human-readable disassembly of your final ELF.
*   **Why:** Great for understanding codegen and symbol placement.

***

### Generate assembly from C sources

```make
.PHONY: assembly
assembly: | $(BUILD)
	@for f in $(SRC); do \
		out="$(BUILD)/$${f##*/}"; out="$${out%.c}.s"; \
		echo "$(CC) $(CFLAGS) -I$(INC_DIR) -S $$f -o $$out"; \
		$(CC) $(CFLAGS) -I$(INC_DIR) -S $$f -o $$out; \
	done
```

*   **What:** Produces `.s` files (compiler output) for each `.c` into `build/`.
*   **Why:** Educational and useful for performance tuning or verifying calling conventions.

***

### Inspect binary words

```make
.PHONY: printbinary
printbinary: $(BIN)
	xxd -e -c 4 -g 4 $(BIN)
```

*   **What:** Hex dump as little-endian 32-bit words.
*   **Why:** Matches how RV32 fetches words; easy to eyeball instruction encodings.

***

### QEMU + GDB convenience targets

```make
.PHONY: startqemu
startqemu: $(ELF)
	qemu-system-riscv32 -S -M virt -nographic -bios none -kernel $(ELF) -gdb tcp::1234

.PHONY: connectgdb
connectgdb: $(ELF)
	gdb-multiarch $(ELF) -ex "target remote localhost:1234" -ex "break _start" -ex "continue" -q
```

*   **`qemu-system-riscv32`**: Emulate RV32 “virt” machine.
    *   `-S`: don’t start CPU; wait for debugger.
    *   `-nographic`: console over stdio; no GUI.
    *   `-bios none`: don’t load a firmware/BIOS.
    *   `-kernel $(ELF)`: load your ELF at the address the linker script specifies.
    *   `-gdb tcp::1234`: open a GDB server on port 1234.
*   **`gdb-multiarch`**: Convenient GDB that supports multiple architectures.
    *   `-ex ...`: auto-connect to the QEMU stub, set a breakpoint at `_start`, and run.
*   **Why:** One command to launch QEMU, one to attach GDB—smooth bring-up loop.

***

### Clean

```make
.PHONY: clean
clean:
	rm -rf $(BUILD)
```

*   **What:** Remove all generated files, keeping the tree tidy.
*   **Why:** Ensures deterministic rebuilds when you change flags or linkers.

***

## Common pitfalls (you’ve already met a few)

*   **Tabs vs Spaces:** Recipe lines **must** start with a **TAB**, not spaces—else “missing separator.”
*   **CRLF line endings:** Windows line endings in Makefiles produce confusing errors. Normalize with `sed -i 's/\r$//' Makefile`.
*   **Relative paths:** These are relative to **where `make` is run**, not the repo root (unless you design it otherwise). You fixed this with `ROOT` and careful references.
*   **Link-time RV64 default:** When using a `riscv64-unknown-elf-` toolchain, the linker may default to **64-bit emulation**. Fix by:
    *   Including `-march/-mabi` at link time **and/or**
    *   Forcing `-Wl,-melf32lriscv` in `LDFLAGS`, **or**
    *   Switching to `riscv32-unknown-elf-` toolchain.

***

## How to extend this Makefile (next steps)

*   **Auto-dependency generation:** Rebuild only the `.o` files whose headers changed.
    ```make
    CFLAGS += -MMD -MP
    # then include generated .d files
    -include $(OBJS:.o=.d)
    ```
*   **Multiple modules:** Add more `SRC_DIR` entries or pattern rules for subdirs.
*   **Unit tests (host build):** Add a separate `HOST_CC=gcc` path and a `make test` target that compiles host-side utilities.
*   **Configurable toolchain/ISA:** Allow overrides:
    ```make
    CROSS ?= riscv64-unknown-elf-
    MARCH ?= rv32i
    MABI  ?= ilp32
    CFLAGS += -march=$(MARCH) -mabi=$(MABI)
    ```
    Then run: `make MARCH=rv32imac`.
*   **Profiles:** `make DEBUG=1` to switch `-O0/-ggdb` vs `-O2/-g0`, etc.

***

## TL;DR

*   The structured Makefile is solid now because **link time** gets the same RV32 hints as compile time (and explicitly forces 32-bit emulation).
*   The Makefile organizes builds cleanly, supports incremental compilation, and gives you helpful developer targets for QEMU/GDB, disassembly, and inspection.
*   The commit message above sums up the changes concisely and is ready to use.
