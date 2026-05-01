#include<stdio.h>


/**
 * TASK 2: The "Safe" Callback (Defensive Programming)
 * 1. Create a global function pointer.
 * 2. Write a registration function.
 * 3. Write a 'trigger_interrupt' function that safely calls the pointer.
 */

typedef void (*InterruptCallback) (void);

 // global function pointer (safely initialized with NULL)
 InterruptCallback g_interrupt_cb = NULL;

 /* Registration function */
void register_timer_callback(InterruptCallback cb)
{
    g_interrupt_cb = cb;
}

void on_interrupt(void)
{
    printf("Interrupt handled\n");
}

void trigger_interrupt (void) {
    if (g_interrupt_cb != NULL)
    {
        g_interrupt_cb();
    }
    else {
        /* Defensive behaviour when no callback is registered */
        printf("no callback registered \n");
        /* In embedded system, this might be no-op oe error flag */
    }
 }

 int main()
 {
    trigger_interrupt(); // Should handle NULL safely
    register_timer_callback(on_interrupt); // callback registered
    trigger_interrupt(); // handles callback
    register_timer_callback(NULL); // un-registers callback
    trigger_interrupt(); // safe callback

    return 0;
 }

