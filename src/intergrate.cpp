#include <iostream>
#include <cmath>
#include <pthread.h>

struct Task {
    double (*f) (double);

    double start;
    double end;
    double eps;
    double * result;
};

void * thread_fun(void * data) {
    Task & t = *static_cast<Task *>(data);

    double res = 0;
    for (double x = t.start; x < t.end; x += t.eps)
        res += t.f(x + t.eps / 2) * t.eps;

    *t.result = res;
    return 0;
}

double function(double x) {
    return exp(x * sin(x));
}

int main(int /*argc*/, char * /*argv*/ []) {
    int const THREAD_COUNT = 4;

    double results[THREAD_COUNT] = {};

    Task tasks[THREAD_COUNT] = {};

    double const start = -5;
    double const end   =  5;

    double const taskLen = (end - start) / THREAD_COUNT;
    for (int i = 0; i != THREAD_COUNT; ++i) {
        tasks[i].f      = &function;
        tasks[i].start  = start + taskLen * i;
        tasks[i].end    = tasks[i].start + taskLen;
        tasks[i].eps    = 1e-9;
        tasks[i].result = &results[i];
    }

    pthread_t threads[THREAD_COUNT];

    for (size_t i = 0; i != THREAD_COUNT; ++i)
        pthread_create(&threads[i], 0, &thread_fun, &tasks[i]);

    for (size_t i = 0; i != THREAD_COUNT; ++i)
        pthread_join(threads[i], 0);

    double res = 0;
    for (size_t i = 0; i != THREAD_COUNT; ++i)
        res += results[i];

    std::cout << "Result: " << res << std::endl;

    return 0;
}

