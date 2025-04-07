#include <iostream>
#include <string>
#include <utility>

template<typename F, typename... Args>
auto apply(F f, Args&&... args) -> decltype(f(std::forward<Args>(args)...)) {
    return f(std::forward<Args>(args)...);
}

int main(int /*argc*/, char * /*argv*/ []) {
    auto fun = [](std::string a, const std::string &b) { return a += b; };

    std::string s("world!");

    s = apply(fun, std::string("Hello, "), s);

    std::cout << s << std::endl;

    return 0;
}
