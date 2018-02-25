int x[10];

int bar(int a, int b, int c, ...);
int bar(int x, int y, int z, ...);

int foo(int a, void *b)
{
    int c[10];
}

int *x[10];			/* conflicting types for 'x' */
