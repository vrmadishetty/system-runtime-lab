#include <stdio.h>
#include <stddef.h>
#include <stdbool.h>


/**
 * TASK 3: The Syntax Trap (Senior Level)
 * 1. Write the typedef for an array of 5 function pointers (char* arg, returns bool).
 * 2. Declare an instance of it.
 */

 // array of  5 function pointers
 typedef bool (*FuncArray5[5]) (char *arg);


// declare an instance
FuncArray5 handlers;

bool handler1(char *arg) {
    printf("Pass Handler1: %s\n", arg);
    return true;
}

bool handler2(char *arg) {
    return arg != NULL;
}

int main () {
    handlers[0] = handler1;
    handlers[1] = handler2;

    handlers[0]("help");
    handlers[0]("status");
    handlers[1]("reboot");

    return 0;
}
