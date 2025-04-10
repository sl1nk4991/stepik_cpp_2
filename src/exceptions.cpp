#include <cmath>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

class bad_from_string : public std::exception {
public:
    bad_from_string(const std::string &s) : msg(s) {}
    const char *what() const noexcept override {
        return msg.c_str();
    }
private:
    std::string msg;
};

template<class T>
T from_string(std::string const& s) {

    T t;
    std::istringstream is(s);

    is >> std::noskipws >> t;

    if (is.fail())
        throw bad_from_string("conversion error");

    is.get();

    if(is.good())
        throw bad_from_string("conversion error");

    return t;
}

int main(int /*argc*/, char * /*argv*/ []) {
    using namespace std;

#if 1
    #define _CATCH() catch (exception const& e) {cout<<"catch std::exception: "<< e.what();} catch (...) {cout<<"catch unknown";}

    vector<string> strings{ "123", "12.3", "", " ", "abc", " 123", "123 ", "12 3", "-1", "a", "A"};

    for (auto& str : strings) {

        cout << endl << "from_string(\'" << str << "\'):";

        try {cout<< endl  << "<string>: "; cout << from_string<string>(str);} _CATCH()

        try {cout<< endl  << "<double>: "; cout << from_string<double>(str);} _CATCH()

        try {cout<< endl  << "<int>   : "; cout << from_string<int>(str);} _CATCH()

        try {cout<< endl  << "<char>  : "; cout << from_string<char>(str);} _CATCH()

        cout << endl;

    }
#endif

    return 0;
}
