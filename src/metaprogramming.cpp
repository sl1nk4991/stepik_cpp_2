#include <cstddef>
#include <iostream>
#include <ostream>
#include <tuple>
#include <type_traits>
#include <utility>

//Fib
template <int N>
struct Fib {
    static const int value = Fib<N - 1>::value + Fib<N - 2>::value;
};

template <>
struct Fib<0> {
    static const int value = 0;
};

template <>
struct Fib<1> {
    static const int value = 1;
};
//Fib

//IntList
template <int... args>
struct IntList;

template<int N, int... Args>
struct IntList<N, Args...> {
    constexpr static int Head = N;
    using Tail = IntList<Args...>;
};

template <>
struct IntList<> { };
//IntList

//Length
template <typename IL>
struct Length {
    static const size_t value = 1 + Length<typename IL::Tail>::value;
};

template <>
struct Length<IntList<>> {
    static const size_t value = 0;
};
//Length

//IntCons
template <int N, typename IL>
struct IntCons;

template <int N, int... Args>
struct IntCons<N, IntList<Args...>> {
    using type = IntList<N, Args...>;
};
//IntCons

//Concat
template <typename IL1, typename IL2>
struct Concat;

template <int... Is1, int... Is2>
struct Concat<IntList<Is1...>, IntList<Is2...>> {
    using type = IntList<Is1..., Is2...>;
};
//Concat

//Generate
template <int N, int S = 0>
struct Generate {
    using type = typename IntCons<S,
          typename Generate<N - 1, S + 1>::type>::type;
};

template <int S>
struct Generate<0, S> {
    using type = IntList<>;
};
//Generate

//IndexSequence
template <size_t... Ns> struct IndexSequence { };

template <size_t... Ns> struct MakeIndexSequenceImpl;

template <size_t I, size_t... Ns>
struct MakeIndexSequenceImpl<I, Ns...> {
    using type = typename MakeIndexSequenceImpl<I - 1, I - 1, Ns...>::type;
};

template <size_t... Ns>
struct MakeIndexSequenceImpl<0, Ns...> {
    using type = IndexSequence<Ns...>;
};

template <size_t N>
using MakeIndexSequence = typename MakeIndexSequenceImpl<N>::type;
//IndexSequence

//Apply
template <typename F, typename Tuple, size_t... Idx>
auto apply_impl(F&& f, Tuple&& args, IndexSequence<Idx...>) ->
decltype(std::forward<F>(f)(std::get<Idx>(std::forward<Tuple>(args))...)) {
    return std::forward<F>(f)(std::get<Idx>(std::forward<Tuple>(args))...);
}

template <typename F, typename Tuple>
auto apply(F&& f, Tuple&& args) ->
decltype (apply_impl(f, args, MakeIndexSequence<std::tuple_size<
    typename std::remove_reference<Tuple>::type>::value>())) {
    return apply_impl(f, args, MakeIndexSequence<std::tuple_size<
            typename std::remove_reference<Tuple>::type>::value>());
}
//Apply

//Zip
template <int a, int b>
struct Plus {
    static const int value = a + b;
};

template <int a, int b>
struct Minus {
    static const int value = a - b;
};

template <typename IL1, typename IL2, template <int, int> class F>
struct Zip {
    using type = typename IntCons<F<IL1::Head, IL2::Head>::value,
          typename Zip<typename IL1::Tail, typename IL2::Tail, F>::type>::type;
};

template <template <int, int> class F>
struct Zip<IntList<>, IntList<>, F> {
    using type = IntList<>;
};
//Zip

//Quantity
template<int m = 0, int kg = 0, int s = 0, int A = 0, int K = 0, int mol = 0,
    int cd = 0>
using Dimension = IntList<m, kg, s, A, K, mol, cd>;

template <typename IL>
class Quantity {
    double value_ = 0;
public:
    using dimension = IL;

    Quantity() = default;
    explicit Quantity(double value)
        : value_(value) {}

    Quantity(Quantity&& other) = default;
    Quantity(const Quantity& other) = default;

    Quantity& operator=(Quantity&& other) = default;
    Quantity& operator=(const Quantity& other) = default;

    Quantity& operator+=(const Quantity& rhs) {
        this->value_ += rhs.value_;
        return *this;
    }

    Quantity& operator-=(const Quantity& rhs) {
        this->value_ -= rhs.value_;
        return *this;
    }

    Quantity& operator*=(double rhs) {
        this->value_ *= rhs;
        return *this;
    }

    Quantity& operator/=(double rhs) {
        this->value_ /= rhs;
        return *this;
    }

    double value() const {
        return this->value_;
    }

    double& value() {
        return this->value_;
    }

    operator double() const {
        return this->value_;
    }

    operator double&() {
        return this->value_;
    }
};

template <typename IL>
const Quantity<IL> operator+(const Quantity<IL>& lhs, const Quantity<IL>& rhs) {
    Quantity<IL> result(lhs);

    return result += rhs;
}

template <typename IL>
const Quantity<IL> operator-(const Quantity<IL>& lhs, const Quantity<IL>& rhs) {
    Quantity<IL> result(lhs);

    return result -= rhs;
}

template <typename IL>
const Quantity<IL> operator*(const Quantity<IL>& lhs, double rhs) {
    Quantity<IL> result(lhs);

    return result *= rhs;
}

template <typename IL>
const Quantity<IL> operator*(double lhs, const Quantity<IL>& rhs) {
    Quantity<IL> result(rhs);

    return result *= lhs;
}

template <typename IL>
const Quantity<IL> operator/(const Quantity<IL>& lhs, double rhs) {
    Quantity<IL> result(lhs);

    return result /= rhs;
}

template <typename IL1, typename IL2>
const Quantity<typename Zip<IL1, IL2, Plus>::type>
operator*(const Quantity<IL1>& lhs, const Quantity<IL2>& rhs) {
    using type = Quantity<typename Zip<IL1, IL2, Plus>::type>;

    return type(lhs.value() * rhs.value());
}



template <typename IL1, typename IL2>
const Quantity<typename Zip<IL1, IL2, Minus>::type>
operator/(const Quantity<IL1>& lhs, const Quantity<IL2>& rhs) {
    using type = Quantity<typename Zip<IL1, IL2, Minus>::type>;

    return type(lhs.value() / rhs.value());
}

using NumberQ = Quantity<Dimension<>>;

template <typename IL>
const Quantity<typename Zip<NumberQ::dimension, IL, Minus>::type> operator/(double lhs, const Quantity<IL>& rhs) {
    Quantity<typename Zip<NumberQ::dimension, IL, Minus>::type> result(rhs);

    return result /= lhs;
}

using NumberQ   = Quantity<Dimension<>>;          // число без размерности
using LengthQ   = Quantity<Dimension<1>>;          // метры
using MassQ     = Quantity<Dimension<0, 1>>;       // килограммы
using TimeQ     = Quantity<Dimension<0, 0, 1>>;    // секунды
using FreqQ     = Quantity<Dimension<0, 0, -1>>;    // секунды ^ -1
using VelocityQ = Quantity<Dimension<1, 0, -1>>;   // метры в секунду
using AccelQ    = Quantity<Dimension<1, 0, -2>>;   // ускорение, метры в секунду в квадрате
using ForceQ    = Quantity<Dimension<1, 1, -2>>;   // сила в ньютонах
//Quantity

template <typename T>
void check() {
    std::cout << __PRETTY_FUNCTION__ << std::endl;
}

int main() {
    std::cout << Fib<10>::value << std::endl;

    using primes = IntList<2, 3, 5, 7, 11, 13>;

    constexpr size_t len = Length<primes>::value;

    constexpr int head = primes::Head;

    using old_primes = primes::Tail;

    std::cout << len << std::endl;

    std::cout << head << std::endl;

    std::cout << old_primes::Head << std::endl;

    using new_primes = IntCons<2, primes>::type;

    std::cout << new_primes::Head << std::endl;

    using L3 = Generate<5>::type;

    check<L3>();

    auto f = [](int x, double y, double z) { return x + y + z; };
    auto t = std::make_tuple(30, 5.0, 1.6);
    auto res = apply(f, t);

    std::cout << res << std::endl;

    using L1 = IntList<1, 2, 3, 4, 5>;
    using L2 = IntList<1, 3, 7, 7, 2>;

    using Sum = Zip<L1, L2, Plus>::type;

    check<Sum>();

    {
        LengthQ   l{30000};      // 30 км
        TimeQ     t{10 * 60};    // 10 минут
        // вычисление скорости
        VelocityQ v = l / t;     // результат типа VelocityQ, 50 м/с

        std::cout << v << std::endl;
        check<typeof(v)>();

        AccelQ    a{9.8};        // ускорение свободного падения
        MassQ     m{80};         // 80 кг
        // сила притяжения, которая действует на тело массой 80 кг
        ForceQ    f = m * a;

        std::cout << f << std::endl;
        check<typeof(f)>();

        FreqQ hz = (double)1 / t;
        std::cout << hz << std::endl;
        check<typeof(hz)>();
    }

    return 0;
}
