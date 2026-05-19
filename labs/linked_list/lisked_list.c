#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct node {
    //char data[39];
    int data;
    struct node* next_node;
} __attribute__((packed)) node_s;

node_s *__head = NULL;


int init(int data) {

    if(__head) {
        printf("List already initialized.\n");
        return -1;
    }

	// Allocate memory assign the address to __head;
	__head =  malloc(sizeof(node_s));
	
	// sanity check
	if(!__head) {
		printf("Memory allocation failed.\n");
		return -1;
	}
	printf("node address: %p\n", __head->next_node);

    // make node to point to NULL because there is nothing
    // to point to.
	__head->next_node = NULL;

	//memcpy(__head->data, data, strlen(data) + 1);
	
	__head->data = data;
	return 0;
}

int add (int data) {

	/*if(!data) {
		printf("Invalid data. \n");
		return -1;
	} */
	
	node_s *n = malloc(sizeof(node_s));
	
	if(!n) {
		printf("Memory allocation failed\n");
		return -2;
	}
	
	
	n->next_node = __head;
	//memcpy(n->data, data, strlen(data));
	n->data = data;

    __head = n;
	
	return 0;
}

int add_at_the_end(int data) {
    
    node_s *cur;

	node_s *n = malloc(sizeof(node_s));
	if(!n) {
		printf("Memory allocation failed.\n");
		return -1;
	}

    n->data = data;
    n->next_node = NULL;
	
	for(cur = __head;
		cur->next_node != NULL;
		cur = cur->next_node) {
            /*
                Nothing to do here. Just moving the cursor to the end of the list.
            */
	}
	
	cur->next_node = n;
    return 0;
}

void traverse(void) {
	for(node_s *cursor = __head; 
		cursor != NULL; 
		cursor = cursor->next_node )
	{
		printf("%d ", cursor->data);
	}
    printf("\n");
}

int exists (int data) {
	for(node_s *n = __head; 
		n != NULL; 
		n = n->next_node) {
		if(n->data == data) {
			return 1;
		}
	}
	return 0;
}

int delete (int data) {
	node_s *cur, *prev;
	for(prev = NULL, cur=__head; 
		cur != NULL; 
		prev = cur, cur = cur->next_node) {
			if(cur->data == data) {
				prev->next_node = cur->next_node;
				free(cur);
				printf("Deleted: %d\n", data);
				return 1;
			}
		}
	return 0;
}

int insert(int key, int data) {

	node_s *cur = __head;
	node_s *prev = NULL;
	
	while(cur != NULL) {
		
		if(cur->data == key) {
			node_s *new_node = malloc(sizeof(node_s));
			if(!new_node) {
				printf("Memory allocation failed");
				return -1;
			}
			new_node->data = data;
			new_node->next_node = cur;

			if(prev == NULL) {
				__head = new_node;
				return 0;
			} else {	
				prev->next_node = new_node;
			}
			return 0;
		}

		prev = cur; // save the current
		cur = cur->next_node;

	}
	printf("Invalid key: %d", key);
	return -2;
}

int main() {

	//init("Hello world");
	//init("Ramana");
	init(101);
    for(int i = 0; i < 10; i++) {
        add(i);
    }

    for(int i = 0; i < 10; i++) {
        add_at_the_end(i);
    }
	traverse();
    printf("Does 101 exist? %d\n", exists(101));
    printf("Does 102 exist? %d\n", exists(102));

	delete(101);
	traverse();
	delete(5);
	traverse();

	insert(3, 100);
	traverse();
	return 0;
}