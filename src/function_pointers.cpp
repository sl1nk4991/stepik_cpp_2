#include <iostream>

typedef int *(*(*ComplexFunction)(int, int (*)(double)))(const char *);

typedef int *(*ReturnType)(const char*);

int *bar(const char *ptr) {
    return (int*)ptr;
}

int *(*foo(int, int (*)(double)))(const char*) {
    ReturnType ret = bar;

    std::cout << __PRETTY_FUNCTION__ << std::endl;

    return ret;
}

int boo(double n) {
    return n;
}

int main(int /*argc*/, char * /*argv*/ []) {
    ComplexFunction cf = &foo;

    cf(5, &boo);

    return 0;
}

