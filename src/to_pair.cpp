#include <cstddef>
#include <iostream>
#include <tuple>
#include <utility>

#if 1
template<size_t N1, size_t N2, typename... Args>
auto to_pair(const std::tuple<Args...> &tup)
#else
template<size_t N1, size_t N2, typename T>
auto to_pair(const T &tup)
#endif
#if __cplusplus < 201103L
#error "C++ version is lower then 11 (201103)"
#elif __cplusplus == 201103L
#if 1
    -> decltype(std::make_pair(std::get<N1>(tup), std::get<N2>(tup)))
#else
    -> std::pair<
        decltype(std::get<N1>(tup)),
        decltype(std::get<N2>(tup))
    >
#endif
#endif
{
    return std::make_pair(std::get<N1>(tup), std::get<N2>(tup));
}

int main(int /*argc*/, char * /*argv*/ []) {
    auto t = std::make_tuple(0, 3.5, "hello");

    std::pair<double, char const *> p = to_pair<1, 2>(t);

#if 0
    std::pair<double, char const *> p2 = to_pair<1, 2>((double)5);
    auto t2 = std::make_tuple(0, 3.5);
    auto p2 = to_pair<1, 2>(t2);
#endif

    std::cout << p.first << std::endl;

    std::cout << p.second << std::endl;

    return 0;
}

