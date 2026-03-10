CROSS   = riscv64-unknown-elf-
CC      = $(CROSS)gcc
OBJCOPY = $(CROSS)objcopy
OBJDUMP = $(CROSS)objdump

# Path to repo root, relative to this Makefile
ROOT = ../../..

# Flags
CFLAGS  = -O0 -ggdb -march=rv32i -mabi=ilp32 -ffreestanding -fno-builtin
# Keep -nostdlib for the link step
LDFLAGS = -nostdlib -T $(ROOT)/platform/riscv/qemu/linker/memory.ld

# Sources/headers
STARTUP = $(ROOT)/platform/riscv/qemu/startup/start.S
SRC_DIR = ./src
INC_DIR = ./include
SRC     = $(wildcard $(SRC_DIR)/*.c)
HEADERS = $(wildcard $(INC_DIR)/*.h)

# Build artifacts
BUILD = build
OBJS  = $(BUILD)/start.o $(patsubst $(SRC_DIR)/%.c,$(BUILD)/%.o,$(SRC))
ELF   = $(BUILD)/main.elf
BIN   = $(BUILD)/main.bin
DUMP  = $(BUILD)/main.dump

# Default rule
.PHONY: all
all: $(ELF) $(BIN)

# Ensure build dir exists
$(BUILD):
	@mkdir -p $(BUILD)

# Compile startup (assembly)
$(BUILD)/start.o: $(STARTUP) | $(BUILD)
	$(CC) $(CFLAGS) -I$(INC_DIR) -c $< -o $@

# Compile C sources
$(BUILD)/%.o: $(SRC_DIR)/%.c $(HEADERS) | $(BUILD)
	$(CC) $(CFLAGS) -I$(INC_DIR) -c $< -o $@

# Link
$(ELF): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(LDFLAGS) -o $@

# Convert to binary
$(BIN): $(ELF)
	$(OBJCOPY) -O binary $< $@

# Optional: disassembly
.PHONY: dump
dump: $(ELF)
	$(OBJDUMP) -d $< > $(DUMP)
	@echo "Wrote $(DUMP)"

# Generate assembly from C sources (one .s per input in build/)
.PHONY: assembly
assembly: | $(BUILD)
	@for f in $(SRC); do \
		out="$(BUILD)/$${f##*/}"; out="$${out%.c}.s"; \
		echo "$(CC) $(CFLAGS) -I$(INC_DIR) -S $$f -o $$out"; \
		$(CC) $(CFLAGS) -I$(INC_DIR) -S $$f -o $$out; \
	done

# Print binary (little-endian 32-bit words)
.PHONY: printbinary
printbinary: $(BIN)
	xxd -e -c 4 -g 4 $(BIN)

# Run QEMU
.PHONY: startqemu
startqemu: $(ELF)
	qemu-system-riscv32 -S -M virt -nographic -bios none -kernel $(ELF) -gdb tcp::1234

# Connect GDB
.PHONY: connectgdb
connectgdb: $(ELF)
	gdb-multiarch $(ELF) -ex "target remote localhost:1234" -ex "break _start" -ex "continue" -q

# Clean
.PHONY: clean
clean:
	rm -rf $(BUILD)