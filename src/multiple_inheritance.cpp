#include <iostream>

// Probelm 1

struct Base { //NOLINT
    int a;

    virtual ~Base() = default;
};

struct D1 : Base {};

struct D2 : Base {};

struct D3 : D1, D2 {};

namespace {
    Base const *D1BaseToD2Base(Base const *base) {
        return static_cast<const D2*>(dynamic_cast<const D3*>(
                    dynamic_cast<const D1*>(base)
                    ));
    }
}

// Probelm 1

//Probelm 2

struct Unit {
    explicit Unit(size_t id) //NOLINT
        : id_(id) {}

    [[nodiscard]] size_t id() const { return this->id_; }

private:
    size_t id_;
};

struct Animal: virtual Unit {
    // name хранит название животного
    // "bear" для медведя
    // "pig" для свиньи
    Animal(const std::string &name, size_t id) //NOLINT
        : Unit(id), name_(name) {
#ifdef EBUG
            std::cout << __PRETTY_FUNCTION__ << "\n";
#endif
        }

    [[nodiscard]] std::string const& name() const { return name_; }
private:
    std::string name_;
};

struct Man: virtual Unit {
    explicit Man(size_t id) //NOLINT
        : Unit(id) {
#ifdef EBUG
            std::cout << __PRETTY_FUNCTION__ << "\n";
#endif
        }
};

struct Bear: Animal {
    explicit Bear(size_t id) //NOLINT
        : Unit(id), Animal("bear", id) {
#ifdef EBUG
            std::cout << __PRETTY_FUNCTION__ << "\n";
#endif
        }
};

struct Pig: Animal {
    explicit Pig(size_t id) //NOLINT
        : Unit(id), Animal("pig", id) {
#ifdef EBUG
            std::cout << __PRETTY_FUNCTION__ << "\n";
#endif
        }
};

struct ManBearPig: Man, Bear, Pig {
    ManBearPig(size_t id) //NOLINT
        : Unit(id), Man(id), Bear(id), Pig(id) {
#ifdef EBUG
            std::cout << __PRETTY_FUNCTION__ << "\n";
#endif
        }
};

//Probelm 2

namespace {
    void problem1() {
        D3 obj;

        obj.D1::a = 5; //NOLINT
        obj.D2::a = 10; //NOLINT

        Base const *ptr = D1BaseToD2Base(static_cast<D1*>(&obj));

        std::cout << &obj << "\n";
        std::cout << ptr << "\n";
    }

    void problem2() {
        const ManBearPig unit(0);

        std::cout << unit.id() << "\n";
    }
}

int main(int /*argc*/, char * /*argv*/ []) {
    problem1();

    problem2();

    return 0;
}
