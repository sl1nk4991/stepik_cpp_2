#include <functional>
#include <iostream>
#include <iterator>
#include <future>
#include <list>
#include <thread>
#include <unistd.h>
#include <vector>
#include <numeric>

template <typename It, typename F1, typename F2>
auto map_reduce(It p, It q, F1 f1, F2 f2, size_t threads) ->
    decltype(f2(f1(*p), f1(*p))) {
    using value_type = decltype(f2(f1(*p), f1(*p)));

    auto size = std::distance(p, q);
    std::vector<std::future<value_type>> resv(threads);

    auto cur = p;
    for (size_t i = 0; i < threads; i++) {
        auto part_size = size / threads + (i < size % threads ? 1 : 0);
        auto next = std::next(cur, part_size);

        resv.at(i) = std::async(std::launch::async, [cur, next, f1, f2]() {
            return std::accumulate(cur, next, value_type{},
                [&](value_type acc, const typename std::iterator_traits<It>::value_type& val) {
                    return f2(acc, f1(val));
                });
        });

        cur = next;
    }

    value_type result = value_type{};
    for (auto& fut : resv) {
        result = f2(result, fut.get());
    }

    return result;
}

template <typename It, typename F1, typename F2>
auto map_reduce_thread(It p, It q, F1 f1, F2 f2, size_t threads) ->
    decltype(f2(f1(*p), f1(*p))) {
    using value_type = decltype(f2(f1(*p), f1(*p)));

    auto size = std::distance(p, q);
    std::vector<value_type> results(threads);
    std::vector<std::thread> thread_pool;

    auto cur = p;
    for (size_t i = 0; i < threads; i++) {
        auto part_size = size / threads + (i < size % threads ? 1 : 0);
        auto next = std::next(cur, part_size);

        thread_pool.emplace_back([cur, next, f1, f2, &results, i]() {
            results[i] = std::accumulate(cur, next, value_type{},
                [&](value_type acc, const typename std::iterator_traits<It>::value_type& val) {
                    return f2(acc, f1(val));
                });
        });

        cur = next;
    }

    for (auto& t : thread_pool) {
        if (t.joinable()) {
            t.join();
        }
    }

    value_type result = value_type{};
    for (const auto& partial_result : results) {
        result = f2(result, partial_result);
    }

    return result;
}

int main(int /*argc*/, char * /*argv*/ []) {
    std::list<int> l = {1,2,3,4,5,6,7,8,9,10};

    {
        auto res = map_reduce(l.begin(), l.end(), [](int i){return i;},
                std::plus<int>(), 3);
        std::cout << res << std::endl;
    }

    std::cout << std::endl;

    {
        auto res = map_reduce(l.begin(), l.end(),
                [](int i){return i % 2 == 0;}, std::logical_or<bool>(), 4);
        std::cout << res << std::endl;
    }

    std::cout << std::endl;

    {
        auto res = map_reduce_thread(l.begin(), l.end(), [](int i){return i;},
                std::plus<int>(), 3);
        std::cout << res << std::endl;
    }

    std::cout << std::endl;

    {
        auto res = map_reduce_thread(l.begin(), l.end(),
                [](int i){return i % 2 == 0;}, std::logical_or<bool>(), 4);
        std::cout << res << std::endl;
    }

    return 0;
}
