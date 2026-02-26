### This project explores system runtime evolution across platforms.
* bare-metal -> RTOS -> Linux naturally
* Neutral to MCU/SoC/OS

### Toolchain

Execute the following commands to install the toolchain, QEMU and GDB

```bash
sudo apt update -y
sudo apt install -y gcc-riscv64-unknown-elf qemu-system-misc gdb-multiarch
```

### RISC-V Reference Card

Details of the RISC-V 32i Instruction Encoding: [Download the PDF](https://github.com/jameslzhu/riscv-card/releases/download/latest/riscv-card.pdf)

### Tools
1. [RISC-V Instruction Decoder](https://luplab.gitlab.io/rvcodecjs/)
1. [GDB Dashboard](https://github.com/cyrus-and/gdb-dashboard)
1. [Assembler (as) Documentation](https://ftp.gnu.org/old-gnu/Manuals/gas/html_chapter/as_7.html)
    1. [Document for RISC-V Assembler](https://sourceware.org/binutils/docs-2.31/as/RISC_002dV_002dDirectives.html)

### C
1. [ISO Standard](https://www.open-std.org/jtc1/sc22/wg14/www/docs/n1570.pdf)
1. [GNU C Manual](https://www.gnu.org/software/gnu-c-manual/gnu-c-manual.pdf)