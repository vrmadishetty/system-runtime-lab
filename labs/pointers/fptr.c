#include <stdio.h>

void my_func1(int val)
{
    printf("my_func1 -> val: %d\n", val);
}

void my_func2(int val)
{
    printf("my_func2 -> val: %d\n", val);
}

typedef void (*func_ptr)(int val);

int main()
{
    func_ptr fp;
    fp = &my_func1;
    fp(200);

    fp = my_func2;
    fp(100);

    return 0;
}