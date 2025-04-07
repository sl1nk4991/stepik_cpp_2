#include <functional>
#include <iostream>
#include <map>
#include <stdexcept>
#include <typeindex>
#include <utility>

// базовый класс фигуры (полиморфный)
struct Shape {
    virtual void square() = 0;
    virtual ~Shape() = default;
};

// прямоугольник
struct Rectangle : Shape {
    void square() override {}
};

// треугольник
struct Triangle : Shape {
    void square() override {}
};

// функция для проверки пересечения двух прямоугольников
bool is_intersect_r_r(Shape * /*a*/, Shape * /*b*/) {
    return true;
}

// функция для проверки пересечения прямоугольника и треугольника
bool is_intersect_r_t(Shape *a, Shape *b) {
    std::type_index type_a = typeid(*a);
    std::type_index type_b = typeid(*b);

    std::cout << __PRETTY_FUNCTION__ << std::endl;

    std::cout << "Shape *a = " << type_a.name() <<
        ", Shape *b = " << type_b.name() << std::endl;

    return type_a != type_b ? true : false;
}

// Base - базовый класс иерархии
// Result - тип возвращаемого значения мультиметода
// Commutative - флаг, который показывает, что
// мультиметод коммутативный (т.е. f(x,y) = f(y,x)).
template<typename Base, typename Result, bool Commutative>
struct Multimethod2
{
    using F = std::function<Result(Base*, Base*)>;
    // устанавливает реализацию мультиметода
    // для типов t1 и t2 заданных через typeid
    // f - это функция или функциональный объект
    // принимающий два указателя на Base
    // и возвращающий значение типа Result
    void addImpl(std::type_index t1, std::type_index t2, const F &f) {
        if (!this->map.insert({{t1, t2}, f}).second) {
            throw std::logic_error("functor not added");
        }
    }

    // проверяет, есть ли реализация мультиметода
    // для типов объектов a и b
    bool hasImpl(Base *a, Base *b) const {
        // возвращает true, если реализация есть
        // если операция коммутативная, то нужно
        // проверить есть ли реализация для b и а
        return this->isImpl(a, b) || (Commutative && this->isImpl(b, a));
    }

    // Применяет мультиметод к объектам
    // по указателям a и b
    Result call(Base *a, Base *b) const {
        // возвращает результат применения реализации
        // мультиметода к a и b
        auto getResult = [this](Base *a, Base *b) -> Result {
            return this->map.find({typeid(*a), typeid(*b)})->second(a, b);
        };

        return this->isImpl(a, b) ? getResult(a, b) : getResult(b, a);
    }

private:
    std::map<std::pair<std::type_index, std::type_index>, F> map;

    bool isImpl(Base *a, Base *b) const {
        return this->map.count({typeid(*a), typeid(*b)});
    };
};

int main(int /*argc*/, char * /*argv*/ []) {
    // мультиметод для наследников Shape
    // возращающий bool и являющийся коммутативным
    Multimethod2<Shape, bool, true> is_intersect;

    // добавляем реализацию мультиметода для двух прямоугольников
    is_intersect.addImpl(typeid(Rectangle), typeid(Rectangle), &is_intersect_r_r);

    // добавляем реализацию мультиметода для прямоугольника и треугольника
    is_intersect.addImpl(typeid(Rectangle), typeid(Triangle), &is_intersect_r_t);

    // создаём две фигуры
    Shape *s1 = new Triangle();
    Shape *s2 = new Rectangle();

    // проверяем, что реализация для s1 и s2 есть
    if (is_intersect.hasImpl(s1, s2)) {
        // вызывается функция is_intersect_r_t(s2, s1)
        bool res = is_intersect.call(s1, s2);

        // Замечание: is_intersect_r_t ожидает,
        // что первым аргументом будет прямоугольник
        // а вторым треугольник, а здесь аргументы
        // передаются в обратном порядке.
        // ваша реализация должна самостоятельно
        // об этом позаботиться

        std::cout << res << std::endl;
    } else {
        std::cout << "No implementation" << std::endl;
    }

    delete s2;
    delete s1;

    return 0;
}
