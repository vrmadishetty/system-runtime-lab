#include <stdio.h>
#include <stdint.h>

//#define TIMER ((my_Timer_Regs_t *)0x40001000)

typedef struct {
	volatile uint32_t  CTRL;    /* Control Register: Bit 0 is ENABLE */
	volatile uint32_t  VAL;     /* Current Value Register */
	volatile uint32_t  RELOAD;  /* Reload Value Register */
}Timer_Regs_t;


/* Assume hardware is at this address */
uint32_t dummy_memory[10]; 
#define TIMER_BASE_ADDR ((uintptr_t)&dummy_memory[0])


void reset_timer(uintptr_t base_addr) {

	//if(base_addr == NULL) return;
	
	Timer_Regs_t *my_Timer_Regs_ptr_t = (Timer_Regs_t *)base_addr;
    if(my_Timer_Regs_ptr_t == NULL) {
        printf("Error: Invalid timer base address.\n");
        return;
    }
	
	my_Timer_Regs_ptr_t->CTRL &= ~(1 << 0); /* Disable timer */
	my_Timer_Regs_ptr_t->VAL = 0;   /* Clear count */
    my_Timer_Regs_ptr_t->RELOAD = 1000;     /* Set default reload */
	
}

int main() {
    printf("Simulating hardware at address: %p\n", (void*)TIMER_BASE_ADDR);
    reset_timer(TIMER_BASE_ADDR);

	Timer_Regs_t *verify = (Timer_Regs_t *)TIMER_BASE_ADDR;
	printf("Timer Reset - CTRL: %d, VAL: %d, RELOAD: %d\n", verify->CTRL, verify->VAL, verify->RELOAD);
	
	return 0;
}