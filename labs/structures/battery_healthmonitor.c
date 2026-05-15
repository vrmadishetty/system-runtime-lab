
/*
### **Task 2: The Battery Health Monitor (Array Processing)**
**Scenario:** A device has multiple battery cells. You need to identify the weakest one.
*   **Struct:** Define `BatteryCell_t` with `int cell_id` and `float voltage`.
*   **Goal:** Write a function `BatteryCell_t* find_weakest_cell(BatteryCell_t *cells, int count)`.
*   **Requirements:**
    1.  Iterate through an array of cells.
    2.  Return the **memory address** (pointer) of the cell with the lowest voltage.
    3.  Return `NULL` if the array is empty or the pointer is invalid.
*/

#include <stdio.h>

typedef struct {
    int cell_id;
    float voltage;
} BatteryCell_t;

BatteryCell_t* find_weakest_cell(BatteryCell_t *cells, int count) {
    if (cells == NULL || count <= 0) return NULL;
	
	BatteryCell_t *weakest = &cells[0]; // start with the first one
	for (int i =1; i < count; i++) 
	{
		if(cells[i].voltage < weakest->voltage) {
			weakest = &cells[i];  /* Update pointer to the new weakest cell */
		}
	}
	return weakest;
}

int main () {
    BatteryCell_t pack [] = {
        {1, 3.7f},
        {2, 3.6f},
        {3, 3.8f}
    };
	
	BatteryCell_t *weak_ptr = find_weakest_cell(pack, 3);
	if(weak_ptr) {
		printf("Weak cell: %d, Weak voltage: %f\n", weak_ptr->cell_id, weak_ptr->voltage);
	}
	return 0;
}