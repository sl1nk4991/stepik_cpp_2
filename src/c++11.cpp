#include <cstddef>
#include <cstring>
#include <iostream>
#include <optional>
#include <ostream>
#include <string>
#include <typeinfo>

class Class {
public:
    int *foo(int n, double d) {
        int *ptr = reinterpret_cast<int*>(new char[sizeof(int) +
                sizeof(double)]);

        *ptr = n;
        *reinterpret_cast<double*>(ptr + 1) = d;

        return ptr;
    }

    static int *bar(int n, double d) {
        int *ptr = reinterpret_cast<int*>(new char[sizeof(int) +
                sizeof(double)]);

        *ptr = n;
        *reinterpret_cast<double*>(ptr + 1) = d;

        return ptr;
    }
};

using Foo = int * (Class::*)(int, double);
using Bar = int *(*)(int, double);

struct String final {
    String(const char* str = "", std::optional<size_t> len = std::nullopt)
        : len_(len.value_or(strlen(str))), str_(new char[this->len_]) {
            strncpy(this->str_, str, this->len_);
        }

    ~String() {
        delete [] this->str_;
        this->str_ = nullptr;
        this->len_ = 0;
    }

    explicit operator const char *() const {
        return this->str_;
    }
private:
    size_t len_ = 0;
    char *str_ = nullptr;
};

template<typename T1, typename T2>
auto Plus(T1 x, T2 y) //-> decltype(x + y)
{
    return x + y;
}

void print_values(std::ostream&) {}

template<typename T>
void print_values(std::ostream& os, T value) {
    os << typeid(value).name() << ": " << value << std::endl;
}

template<typename T, typename... Args>
void print_values(std::ostream& os, T value, Args... args) {
    os << typeid(value).name() << ": " << value << std::endl;
    print_values(os, args...);
}

int main() {
    {
        Foo ptr = &Class::foo;
        Bar bar = &Class::bar;
    }

    {
        String s("hello world!");

        //delete s;
        //if (s) {}
        //const char *p1 = s;
        const char *p2 = (const char *)s;
        const char *p3 = static_cast<const char*>(s);
        //const char *s2 = s + 4;
    }

    {
        std::string s = "Compiler";

        size_t sz = s.size();

        auto c1 = s[0];

        decltype(s[0]) c2 = s[7];

        decltype(s[0] + s[7]) c3 = s[1];

        decltype(s)   sa = s;

        decltype((s)) sb = s;

        c1 = 'K';
        c2 = '!';
    }

    print_values(std::cout, 0, 3.5, "Hello");

    print_values(std::cout);

    return 0;
}
