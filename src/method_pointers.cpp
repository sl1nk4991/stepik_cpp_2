#include <iostream>
#include <string>
template<typename T1, typename T2>
bool compare(const T1& a, const T1& b, T2 (T1::*mptr)() const) {
    return (a.*mptr)() < (b.*mptr)();
}

int main(int /*argc*/, char * /*argv*/ []) {
    std::string s1{"String"};
    std::string s2{"Longet string"};

    std::cout << compare(s1, s2, &std::string::size) << std::endl;
    std::cout << compare(s1, s1, &std::string::size) << std::endl;

    return 0;
}
