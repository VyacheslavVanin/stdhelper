#pragma once
#include <type_traits>

/// // Usage example:
///
/// HAS_BINARY_OPERATOR_DEFINE(has_eq, ==);
/// HAS_BINARY_OPERATOR_DEFINE(has_plus, +);
///
/// struct foo {
///     foo operator+(const foo& ) const {return {};}
/// };
///
/// int main () {
///     std::cout << has_eq<int>() << "\n";
///     std::cout << has_eq<foo>() << "\n";
///     std::cout << has_plus<int>() << "\n";
///     std::cout << has_plus<foo>() << "\n";
/// }
///

#define HAS_BINARY_OPERATOR_DEFINE(func_name, OP)                              \
    namespace _impl {                                                          \
    struct func_name##_test {                                                  \
        template <typename T>                                                  \
        static decltype(std::declval<T>() OP std::declval<T>(),                \
                        std::true_type())                                      \
        f(int);                                                                \
        template <typename T>                                                  \
        static std::false_type f(...);                                         \
    };                                                                         \
    template <typename T>                                                      \
    struct func_name##_struct : decltype(func_name##_test::template f<T>(0)) { \
    };                                                                         \
    template <typename T>                                                      \
    constexpr func_name##_struct<T> func_name()                                \
    {                                                                          \
        return func_name##_struct<T>{};                                        \
    }                                                                          \
    }                                                                          \
    using _impl::func_name;

namespace vvv {
HAS_BINARY_OPERATOR_DEFINE(has_eq, ==);
HAS_BINARY_OPERATOR_DEFINE(has_binary_plus, +);
HAS_BINARY_OPERATOR_DEFINE(has_binary_minus, -);
HAS_BINARY_OPERATOR_DEFINE(has_mul, *);
HAS_BINARY_OPERATOR_DEFINE(has_div, /);
HAS_BINARY_OPERATOR_DEFINE(has_mod, %);
HAS_BINARY_OPERATOR_DEFINE(has_gte, >=);
HAS_BINARY_OPERATOR_DEFINE(has_lte, <=);
HAS_BINARY_OPERATOR_DEFINE(has_gt, >);
HAS_BINARY_OPERATOR_DEFINE(has_lt, <);
HAS_BINARY_OPERATOR_DEFINE(has_and, &);
HAS_BINARY_OPERATOR_DEFINE(has_or, |);
HAS_BINARY_OPERATOR_DEFINE(has_logic_and, &&);
HAS_BINARY_OPERATOR_DEFINE(has_logic_or, ||);
HAS_BINARY_OPERATOR_DEFINE(has_xor, ^);
} // namespace vvv
