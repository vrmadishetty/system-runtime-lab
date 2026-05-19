#include <stdio.h>
#include <stdint.h>
#include <time.h>

/* --------------------------------------------------
   LUT (Look-Up Table) for fast bit counting
   Precomputed for all 256 possible byte values
-------------------------------------------------- */
const uint8_t bit_count_lut[256] = {
#define B2(n) n, n+1, n+1, n+2
#define B4(n) B2(n), B2(n+1), B2(n+1), B2(n+2)
#define B6(n) B4(n), B4(n+1), B4(n+1), B4(n+2)
    B6(0), B6(1), B6(1), B6(2)
};

/* --------------------------------------------------
   Slow method: O(N) time, O(1) space
-------------------------------------------------- */
uint32_t count_bits_slow(uint32_t n) {
    uint32_t count = 0;
    while (n) {
        count += n & 1;
        n >>= 1;
    }
    return count;
}

/* --------------------------------------------------
   Fast method: O(1) time, O(N) space (LUT)
-------------------------------------------------- */
uint32_t count_bits_fast(uint8_t n) {
    return bit_count_lut[n];
}

/* --------------------------------------------------
   Extended fast version for 32-bit using LUT
-------------------------------------------------- */
uint32_t count_bits_fast_32(uint32_t n) {
    return bit_count_lut[n & 0xFF] +
           bit_count_lut[(n >> 8) & 0xFF] +
           bit_count_lut[(n >> 16) & 0xFF] +
           bit_count_lut[(n >> 24) & 0xFF];
}

/* --------------------------------------------------
   Benchmark helper
-------------------------------------------------- */
void benchmark() {
    const int iterations = 10000000;
    uint32_t value = 0xDEADBEEF;
    clock_t start, end;

    /* Slow method */
    start = clock();
    for (int i = 0; i < iterations; i++) {
        count_bits_slow(value);
    }
    end = clock();
    double slow_time = (double)(end - start) / CLOCKS_PER_SEC;

    /* Fast method */
    start = clock();
    for (int i = 0; i < iterations; i++) {
        count_bits_fast_32(value);
    }
    end = clock();
    double fast_time = (double)(end - start) / CLOCKS_PER_SEC;

    printf("\nBenchmark Results:\n");
    printf("Slow (O(N))  time: %.4f sec\n", slow_time);
    printf("Fast (O(1))  time: %.4f sec\n", fast_time);
}

/* --------------------------------------------------
   MAIN
-------------------------------------------------- */
int main(void) {
    uint32_t num = 29;  // binary: 11101

    printf("Number: %u\n", num);

    /* Slow method */
    uint32_t slow_result = count_bits_slow(num);
    printf("Slow count: %u\n", slow_result);

    /* Fast method (8-bit) */
    uint32_t fast_result_8 = count_bits_fast((uint8_t)num);
    printf("Fast count (8-bit LUT): %u\n", fast_result_8);

    /* Fast method (32-bit) */
    uint32_t fast_result_32 = count_bits_fast_32(num);
    printf("Fast count (32-bit LUT): %u\n", fast_result_32);

    /* Run benchmark */
    benchmark();

    return 0;
}