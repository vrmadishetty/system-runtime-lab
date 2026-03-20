volatile int global_counter = 0;

int add_numbers(int a, int b)
{
    int result = a + b;
    return result;
}

void modify_memory(int *ptr)
{
    *ptr = *ptr + 10;
}

int main()
{
    int x = 5, y = 20;

    int sum = add_numbers(x, y);
    modify_memory(&x);

    global_counter++;
    return sum;
}