## RISC-V 
**compilation command**
```
riscv64-unknown-elf-gcc -O0 -ggdb -nostdlib -march=rv32i -mabi=ilp32 -Wl,-Tm.ld m.s -o main.elf
```
**Generate object to .bin**
```
riscv64-unknown-elf-objcopy -O binary main.elf main.bin
```
 **QEMU**   
 ```
 qemu-system-riscv32 -S -M virt -nographic -bios none -kernel main.elf -gdb tcp::1234
 ```

 **Start gdb***
 ```
 gdb-multiarch main.elf -ex "target remote localhost:1234" -ex "break _start" -ex "continue" -q
 ```