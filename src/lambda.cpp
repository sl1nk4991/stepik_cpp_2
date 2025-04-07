#include <functional>
#include <iostream>
#include <ostream>

template<typename F>
void for_each_int(int * p, int * q, F f) {
    for ( ; p != q; ++p )
        f(*p);
}

template<typename F>
int *find_if(int * p, int * q, F f) {
    for ( ; p != q; ++p )
        if (f(*p))
            return p;
    return q;
}

template<size_t N>
void print_array(std::ostream &os, int (&arr)[N]) {
    for (auto i: arr) {
        os << i << " ";
    }
    os << std::endl;
}

int main() {
    {
        auto square_fun = [](int &n) -> void { n = n * n; };

        int m[10] = {1,2,3,4,5,6,7,8,9,10};

        print_array(std::cout, m);

        for_each_int(m, m + 10, square_fun);

        print_array(std::cout, m);
    }

    {
        std::function<std::function<bool(int)>(int*, int*)> gen_finder =
            [](int *p, int *q) -> std::function<bool(int)> {
                return [ p, q](int n) -> bool {
                    int *ptr = p;
                    for ( ; ptr != q; ptr++)
                        if (n == *ptr)
                            return true;
                    return false;
                };
            };

        int primes[5] = {2,3,5,7,11};

        int m[10] = {0,0,1,1,4,6,7,8,9,10};

        print_array(std::cout, primes);

        print_array(std::cout, m);

        auto finder = gen_finder(primes, primes + 5);

        int *first_prime = find_if(m, m + 10, finder);

        std::cout << *first_prime << std::endl;
    }

    return 0;
}
