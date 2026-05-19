#include <stdio.h>
#include <stdlib.h>

typedef struct node {
    int data;
    struct node* next_node;
} __attribute__((packed)) node_s;

static node_s* __head = NULL;
static node_s* __tail = NULL;

int init(int data) {
    if(__head) {
        printf("Queue already initialized.\n");
        return -1;
    }

    __head = malloc(sizeof(node_s));
    if(!__head) {
        printf("Memory allocation failed.\n");
        return -1;
    }
    __head->data = data;
    __head->next_node = NULL;
    __tail = __head;
    return 0;
}

int add(int data) {
    if(!__head) {
        printf("Queue not initialized.\n");
        return -1;
    }

    node_s *cur;

	node_s *n = malloc(sizeof(node_s));
	if(!n) {
		printf("Memory allocation failed.\n");
		return -1;
	}

    n->data = data;
    n->next_node = NULL;
	__tail->next_node = n;
    __tail = n;
    return 0;
}

void traverse(void) {
	for(node_s *cursor = __head; 
		cursor != NULL; 
		cursor = cursor->next_node )
	{
		printf("%d --> ", cursor->data);
	}
    printf("NULL\n");
}

int main() {

	init(101);
    for(int i = 0; i < 10; i++) {
        add(i);
    }

	traverse();

	return 0;
}
