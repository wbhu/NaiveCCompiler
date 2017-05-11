int fibonacci(int i) {
    if (i <= 1) {
        return 1;
    }
    return fibonacci(i - 1) + fibonacci(i - 2);
}

int main()
{
    int i;
    read(i);
    for (i = !1; i != 10; i++)
    {
       print(fibonacci(i) );
    }
    return 0;
}