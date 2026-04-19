# Shortcut: Memorize Hex of Powers of 2

| Decimal | Hex |
| -------- | -------- |
| 1 | 0x01 |
| 2 | 0x02 |
| 4 | 0x04 |
| 8 | 0x08 |
| 16 | 0x10 |
| 32 | 0x20 |
| 64 | 0x40 |
| 128 | 0x80 |
| 256 | 0x100 |
| 512 | 0x200 |
| 1024 | 0x400 |
| 2048 | 0x800 |
| 4096 | 0x1000 |

All hex values have a combination of 1, 2, 4, 8 and all the decimal increase or added with the same number to get the next number.

Lets take and example to convert 130 decimal to Binary:
130 nearer number to multiple of 16. 128 is the nearer number.
``` Hex 
    i.e 130 ->  128 + 2
    Binary for `128` is  1000 0000 and `2` is 0000 0010.
    Addition of binary 1000 0010 -> 128 + 2 -> 0x80 + 0x02 -> 0x82 -> 130         
```
## Examples: Tricky Hex - Conversions

### Section 1: Basic Decimal -> Hex
* Convert 199 to hex.
    Nearer number to 199 is 128 i,e 128 +  64 + 7 
                                    1000 0000 + 0100 0000 + 0000 0111
                                    -> 1100 0111
                                    -> 0xC7

### Section 2: Multi‑byte Integer Byte Extraction (little‑endian)
* 1. For the integer 65535, write the 4 bytes in memory (little‑endian). 

65535 in binary 1111 1111 1111 1111 -> 0xFFFF
because 65535 = (2^16) - 1

little indian : 
                | 0xF | 0xF | 0xF | 0xF |

* 2. For the integer 123456, write the 4 bytes in memory (little‑endian). 
123456 -> 0001 1110 0010 0100 0000 -> 00 01 E2 40
Little indian stores Least Significant byte first - 40 E2 01 00

* 3. For the integer -1, write the 4 bytes in memory (little‑endian). 

-1 -> MSB will be 1 i.e 11111111 11111111 11111111 11111111
little indian : 
                | 0xFF | 0xFF | 0xFF | 0xFF
### Section 3: char* Interpretation 
* For each integer, determine:

    The lowest byte value
    The value printed when using:

    signed char
    unsigned char     
* 1. Integer = 200

Integer = 200; // int size 32 bits
step 1: nearer value for 200 is 128 -> 72
 --> 128 + 72
 --> 128 + 64 + 8
 --> 1000 0000 + 0100 0000 + 0000 1000
 --> 1100 1000
 --> C8
 --> 00 00 00 C8
 Little‑endian means the least significant byte is stored first.
 unsigned char --> C8
 signed char --> range -128 to +127
 A number above 127 is interpreted as negative (2’s complement).
 0xC8 in Binary : 1100 1000
 Because the top bit - 1 -> negative number
 Convert to signed value:
 Step to convert
 1. Invert bits: 0011 0111
 2. Add 1: 0011 1000 = 0x38 = decimal is -56

 ## Quick shortcut (mental math)
 If you want a faster method:

 ```c
 signed_value = byte - 256
 ```

 For bytes ≥ 128.
 Example
```c
    200 - 256 = -56
 ```
Same result.