### **Linked Lists in Embedded Systems**

---

#### 1. What it is
```c
typedef struct Node {
    uint32_t data;
    struct Node *next;
} Node_t;

Node_t *head = NULL;
```
A Linked List is a dynamic data structure where each element (Node) contains a data field and a pointer to the next node in the sequence.

---

#### 2. Why it is needed
**The Problem: Fixed-Size Array Fragmentation**
```c
#define MAX_TASKS 10
uint32_t task_ids[MAX_TASKS]; /* Fixed size */

void add_task_array(uint32_t id) {
    /* If index 2 and 5 are free, we have to search for holes.
       We cannot easily insert in the middle without shifting memory. */
}
```
In embedded systems, arrays require a contiguous block of memory. If memory is fragmented or the number of items is unknown, an array may fail to allocate or waste space.

**The Solution: Non-Contiguous Dynamic Growth**
```c
void add_task_list(Node_t **head, uint32_t id, Node_t *new_node) {
    new_node->data = id;
    new_node->next = *head;
    *head = new_node;
}
```
Linked lists allow nodes to be scattered anywhere in RAM. You only use memory for the exact number of items currently in the list, and inserting/deleting in the middle is O(1) if you have the pointer.

---

#### 3. Where it is used
**RTOS Ready-List (Kernel Style)**
```c
typedef struct TaskControlBlock {
    uint8_t priority;
    void (*task_func)(void);
    struct TaskControlBlock *next_task;
} TCB_t;

TCB_t *ready_list_head;

void schedule_next(void) {
    if (ready_list_head) {
        ready_list_head = ready_list_head->next_task;
    }
}
```
Real-Time Operating Systems (RTOS) use linked lists to manage tasks. When a task's priority changes or a timer expires, the kernel moves TCB structures between "Ready," "Blocked," and "Timed-Out" linked lists.

---

#### 4. When to use it vs. Alternatives
**Linked List vs. Array**
```c
/* Array: Fast access (O(1)), Slow insertion (O(N)) */
uint32_t val = my_array[5]; 

/* Linked List: Slow access (O(N)), Fast insertion (O(1)) */
node_ptr->next = new_node;
```
Use an **Array** for static data or when you need fast random access by index. Use a **Linked List** for dynamic queues, stacks, or when memory is fragmented.

---

#### 5. How it works internally
```c
typedef struct {
    uint8_t data;     /* Offset 0, Size 1 */
    /* 3 bytes padding for 32-bit pointer alignment */
    struct Node *next; /* Offset 4, Size 4 */
} InternalNode_t;

size_t node_size = sizeof(InternalNode_t); /* Returns 8 */
```
Nodes are linked by memory addresses. The CPU fetches the `next` pointer, which contains the address of the next node in RAM. Because of alignment, the compiler often adds padding so the pointer sits on a 4-byte boundary.

---

#### 6. Important interview questions and answers
**Q: How do you detect a loop in a linked list?**
```c
bool has_loop(Node_t *head) {
    Node_t *slow = head, *fast = head;
    while (fast && fast->next) {
        slow = slow->next;
        fast = fast->next->next;
        if (slow == fast) return true;
    }
    return false;
}
```
This is Floyd’s Cycle-Finding Algorithm. We use two pointers moving at different speeds; if they ever meet, a loop exists.

---

#### 7. Common beginner mistakes
**Incorrect: Dereferencing NULL during traversal**
```c
void print_list_bad(Node_t *head) {
    while (head->next != NULL) { /* CRASH: if head is NULL */
        printf("%d", head->data);
        head = head->next;
    }
}
```
**Correct: Safe NULL check**
```c
void print_list_good(Node_t *head) {
    while (head != NULL) { /* SAFE: handles empty list */
        printf("%d", head->data);
        head = head->next;
    }
}
```

---

#### 8. Sample Syntax and Access Pattern (MANDATORY)
```c
/* 1. Definition */
typedef struct Node {
    int val;
    struct Node *next;
} Node_t;

/* 2. Static Pool Allocation (Safe for Embedded) */
Node_t pool[3];
Node_t *head = &pool[0];

/* 3. Linkage */
pool[0].next = &pool[1];
pool[1].next = &pool[2];
pool[2].next = NULL;

/* 4. Access Pattern */
int first = head->val;
int second = head->next->val;

/* 5. Passing to function */
void traverse(Node_t *n) {
    while(n) { n = n->next; }
}
```

---

#### 9. Create Markdown file content for `pattern_library`
```markdown
# Pattern: Linked List Management

### Implementation
```c
typedef struct Node {
    uint32_t id;
    struct Node *next;
} Node_t;

void insert_at_head(Node_t **head, Node_t *new_node) {
    if (new_node) {
        new_node->next = *head;
        *head = new_node;
    }
}
```

### Technical Notes
1. **Dynamic Memory:** In safety-critical embedded, avoid `malloc()`. Use a pre-allocated array (pool) of nodes.
2. **Indirection:** Traversal is O(N). Each jump to `next` may cause a cache miss.
3. **Termination:** Always ensure the last node's `next` is `NULL`.

### RTOS Example
```c
typedef struct {
    uint32_t timeout;
    struct Timer *next;
} Timer_t;
```
Used to manage soft-timers by keeping them in a sorted linked list based on the earliest expiration time.
```

---

#### 10. Mental Model / Brain Tattoo
**The Scavenger Hunt**
```c
Node_t *clue = &first_clue;
next_location = clue->next;
```
A linked list is like a **Scavenger Hunt**. You don't know where all the prizes are at the start. You only know where the **First Clue** is. Each clue tells you exactly where to find the **next** address (`clue->next`). If a clue is missing (`NULL`), the hunt is over.