#include <algorithm>
#include <iostream>
#include <list>

template<class It>
size_t max_increasing_len(It p, It q) {
    if (p == q) return 0;

    size_t max = 1, ret = 1;
    auto previous = p;

    for (auto i = p; i != q; ++i) {
        if (*i > *previous) {
            max++;
        } else {
            ret = std::max(ret, max);
            max = 1;
        }

        previous = i;
    }

    return std::max(ret, max);
}

int main(int /*argc*/, char * /*argv*/ []) {
    std::list<int> const l1 = {7,8,9,4,5,6,1,2,3,4};
    size_t len1 = max_increasing_len(l1.begin(), l1.end());

    std::list<int> const l2 = {-3,-2,-1,0,0,1,2,3,4,5};
    size_t len2 = max_increasing_len(l2.begin(), l2.end());

    std::cout << len1 << std::endl;

    std::cout << len2 << std::endl;

    return 0;
}
