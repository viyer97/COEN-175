void y, bar(int x);		/* 'y' has type void */

void foo(int a, int *b)
{
    b[a] = 1;
    x[a] = 1;			/* 'x' undeclared */
}

void bar(int x, ...) { }	/* conflicting types for 'bar' */
