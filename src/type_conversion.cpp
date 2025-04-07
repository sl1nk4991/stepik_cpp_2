
int main(int /*argc*/, char * /*argv*/ []) {
    int a = 27;
    const int b = 412;
    int * pa = &a;

    const int &c = a;
    int d = const_cast<int&>(b);
    const int *p1 = pa;
    int *const *p2 = &pa;
    const int **p3 = const_cast<const int**>(&pa);
    const int *const *p4 = &pa;

    return 0;
}
