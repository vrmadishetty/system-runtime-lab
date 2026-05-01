#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdint.h>

/**
 * TASK 1: The Diagnostic CLI (Jump Table)
 * 1. Define 3 functions: cmd_help, cmd_reboot, cmd_status.
 * 2. Create a struct 'Command' with a string name and a function pointer.
 * 3. Create an array of 'Command' structs.
 * 4. Write a function 'process_command' that takes a string and executes the matching function.
 */

 void cmd_help(void) { printf("help command \n"); }
 void cmd_reboot(void) { printf("reboot command \n"); }
 void cmd_status(void) { printf("status command \n"); }

 typedef struct {
    char *name; // string name
    void (*handler) (void); // function pointer
 } Command_t;

 // array of 'Command' structs.
 /* RAM is expensive, but Flash is relatively abundant
const --> tells the compiler /linker to keep the table in flash memory 
Note: The string literals (like "help") are already in Flash, but the array itself can be moved there too.
*/
const Command_t commands[] = {
    {"help",cmd_help},
    {"reboot", cmd_reboot},
    {"status", cmd_status} 
};

const size_t command_count = sizeof(commands)/sizeof(commands[0]);

int process_command(char *input) {

    if (input == NULL)
    {
        return -1;
    }
    for (size_t i = 0; i < command_count; i++) {
        if (strcmp(input, commands[i].name) == 0) {
            if(commands[i].handler) {
                commands[i].handler();
                return 0; // success
            }
            return -2; //handler missing
        }

    }
    printf("Unknown command: %s\n", input);
    return -3;    
}

int main() {

    int ret = 0;
    ret = process_command("help");
    ret = process_command("reboot");
    ret = process_command("status");
    ret = process_command("check");

    return 0;

}