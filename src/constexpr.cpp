#include <iostream>
#include <ostream>

// определение структуры Point уже подключено
struct Point
{
    Point() = default;

    constexpr Point(double x, double y)
        : x(x), y(y) {}

    friend std::ostream &operator<<(std::ostream &os, const Point &p) {
        return os << "(" << p.x << " " << p.y << ")";
    }

    double x = 0;
    double y = 0;
};

// сложение
constexpr Point operator+(const Point &p1, const Point &p2) {
    return Point(p1.x + p2.x, p1.y + p2.y);
}

// вычитание
constexpr Point operator-(const Point &p1, const Point &p2) {
    return Point(p1.x - p2.x, p1.y - p2.y);
}

// скалярное произведение
constexpr double operator*(const Point &p1, const Point &p2) {
    return p1.x * p2.x + p1.y * p2.y;
}

#if 0
int main(int /*argc*/, char * /*argv*/ []) {
    Point p1 = {1, 0};
    Point p2 = {0, 1};

    std::cout << p1 + p2 << std::endl;
    std::cout << p1 - p2 << std::endl;
    std::cout << p1 * p2 << std::endl;

    return 0;
}
#endif