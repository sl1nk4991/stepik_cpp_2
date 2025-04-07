#include <algorithm>
#include <cstddef>
#include <initializer_list>
#include <iostream>
#include <iterator>
#include <ostream>

template<class T>
struct Array
{
    // все объявленные ниже методы уже реализованы
    explicit Array(size_t size = 0)
        : size_(size), data_(new T[this->size_]) {
#ifdef EBUG
            std::cout << __PRETTY_FUNCTION__ << std::endl;
#endif
        }

    Array(std::initializer_list<T> list)
        : size_(list.size()), data_(new T[this->size_]) {
            std::copy(list.begin(), list.end(), this->data_);

#ifdef EBUG
            std::cout << __PRETTY_FUNCTION__ << std::endl;
#endif
    }

    Array(const Array &other)
        : size_(other.size_), data_(new T[this->size_]) {
            std::copy(other.data_, other.data_ + this->size_, this->data_);

#ifdef EBUG
            std::cout << __PRETTY_FUNCTION__ << std::endl;
#endif
        }

    Array &operator=(const Array &other) {
        if (this != &other) {
            Array<T> temp{other};
            std::swap(this->size_, temp.size_);
            std::swap(this->data_, temp.data_);
        }

#ifdef EBUG
            std::cout << __PRETTY_FUNCTION__ << std::endl;
#endif

        return *this;
    }

    // реализуйте перемещающий конструктор
    Array(Array &&other) {
        std::swap(this->size_, other.size_);
        std::swap(this->data_, other.data_);

#ifdef EBUG
            std::cout << __PRETTY_FUNCTION__ << std::endl;
#endif
    }

    // реализуйте перемещающий оператор присваивания
    Array &operator=(Array &&other) {
        std::swap(this->size_, other.size_);
        std::swap(this->data_, other.data_);

#ifdef EBUG
            std::cout << __PRETTY_FUNCTION__ << std::endl;
#endif

        return *this;
    }

    ~Array() {
        delete [] this->data_;

        this->size_ = 0;
        this->data_ = nullptr;

#ifdef EBUG
            std::cout << __PRETTY_FUNCTION__ << std::endl;
#endif
    }

    size_t size() const {
        return this->size_;
    }

    T &         operator[](size_t i) {
        return this->data_[i];
    }

    T const&    operator[](size_t i) const {
        return this->data_[i];
    }

    const T *begin() const {
        return this->data_;
    }

    const T *end() const {
        return this->data_ + this->size_;
    }

private:
    size_t  size_ = 0;
    T *     data_ = nullptr;
};

template<typename T,
    typename = decltype(std::begin(std::declval<T>())),
    typename = decltype(std::end(std::declval<T>()))>
void print_obj(std::ostream& os, const T &obj, const std::string &name = "") {
    os << name << ": ";
    for (const auto &i: obj) {
        os << i << " ";
    }
    os << std::endl;
}

Array<int> move() {
    return Array<int>{1, 2, 3};
}

int main(int /*argc*/, char * /*argv*/ []) {
    Array<int> obj = Array<int>({5, 10, 15});

    print_obj(std::cout, obj, "obj");

    Array copy{std::move(obj)};

    print_obj(std::cout, obj, "obj");
    print_obj(std::cout, copy, "copy");

    Array<int> empty;

    print_obj(std::cout, empty, "empty");

    empty = move();

    print_obj(std::cout, empty, "empty");

    empty = std::move(empty); //NOLINT

    print_obj(std::cout, empty, "empty");

    return 0;
}
