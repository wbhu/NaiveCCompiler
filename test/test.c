int fibonacci(int i) {
    if (i <= 1) {
        return 1;
    }
    return fibonacci(i-1) + fibonacci(i-2);
}

int main()
{
    int i;
    i = -3;
    while (i <= 10) {
        i = i + 1;
    }
    return 0;