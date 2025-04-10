#include <algorithm>
#include <array>
#include <iostream>
#include <iterator>
#include <vector>

// алгоритм должен работать с forward итераторами
// и возвращать итератор, который потом будет передан
// в метод erase соответствующего контейнера
template<class FwdIt>
FwdIt remove_nth(FwdIt p, FwdIt q, size_t n) {
    return std::remove_if(p, q,
        [&n](typename FwdIt::reference) { return !n--; });
}

template<class Iterator>
size_t count_permutations(Iterator p, Iterator q) {
    using value_type = typename std::iterator_traits<Iterator>::value_type;

    std::vector<value_type> v(p, q);

    std::sort(v.begin(), v.end());

    size_t count = 0;

    do {
        if (std::adjacent_find(v.begin(), v.end()) == v.end()) {
            count++;
        }
    } while(std::next_permutation(v.begin(), v.end()));

    return count;
}

int main(int /*argc*/, char * /*argv*/ []) {
    {
        std::vector<int> v = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

        std::for_each(v.begin(), v.end(),
                [](const int &n) {std::cout << n << ' ';});
        std::cout << std::endl;

        v.erase(remove_nth(v.begin(), v.end(), 0), v.end());

        std::for_each(v.begin(), v.end(),
                [](const int &n) {std::cout << n << ' ';});
        std::cout << std::endl;
    }

    {
        std::vector<int> v = {};

        std::for_each(v.begin(), v.end(),
                [](const int &n) {std::cout << n << ' ';});
        std::cout << std::endl;

        v.erase(remove_nth(v.begin(), v.end(), 20), v.end());

        std::for_each(v.begin(), v.end(),
                [](const int &n) {std::cout << n << ' ';});
        std::cout << std::endl;
    }

    {
        std::vector<int> v = { 0,1 };

        std::for_each(v.begin(), v.end(),
                [](const int &n) {std::cout << n << ' ';});
        std::cout << std::endl;

        v.erase(remove_nth(v.begin(), v.end(), 100), v.end());

        std::for_each(v.begin(), v.end(),
                [](const int &n) {std::cout << n << ' ';});
        std::cout << std::endl;
    }

    {
        std::array<int, 3> a1 = {1,2,3};
        size_t c1 = count_permutations(a1.begin(), a1.end()); // 6

        std::cout << c1 << std::endl;

        std::array<int, 5> a2 = {1,2,3,4,4};
        size_t c2 = count_permutations(a2.begin(), a2.end()); // 36
        std::cout << c2 << std::endl;
    }

    return 0;
}
