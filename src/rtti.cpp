#include <iostream>
#include <typeinfo>

struct Expression {
    virtual double evaluate() const = 0;
    virtual ~Expression() = default;
};

struct Number: Expression {
    Number(double number = 0)
        :number_(number) {}

    ~Number() = default;

    double evaluate() const {
        return this->number_;
    };

private:
    double number_;
};

struct BinaryOperation: Expression {
    double evaluate() const {
        return 0;
    }
};

bool check_equals(Expression const *left, Expression const *right) {
    return typeid(*left) == typeid(*right);
}

template<class T>
bool isSameObject(const T *p, const T *q) {
    return dynamic_cast<const void*>(p) == dynamic_cast<const void*>(q);
}

int main(int /*argc*/, char * /*argv*/ []) {
    Expression *e1 = new Number(10);
    Expression *e2 = new Number(20);
    Expression *e3 = new BinaryOperation();

    std::cout << isSameObject(e1, e2) << std::endl;
    std::cout << isSameObject(e1, e3) << std::endl;
    std::cout << isSameObject(e1, e1) << std::endl;

    delete e1;
    delete e2;
    delete e3;

    return 0;
}
