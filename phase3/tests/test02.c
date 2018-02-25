int x, *y, z;

void foo(void) {
    int **x, y, *z;
}

void foo(void) {		/* redefinition of 'foo' */
    int **x, *y, z;
}
