#ifndef UTIL_STRONG_INT_H_
#define UTIL_STRONG_INT_H_

// Copied from
// https://github.com/tensorflow/tensorflow/blob/master/tensorflow/core/lib/gtl/int_type.h

#include <ostream>
#include <type_traits>

namespace util {

#define DEFINE_STRONG_INT_TYPE(int_type_name, value_type) \
  struct int_type_name##_tag_ {};                         \
  using int_type_name = ::util::StrongInt<value_type, int_type_name##_tag_>;

template <typename _ValueType, typename UniqueName>
class StrongInt {
  static_assert(std::is_integral_v<_ValueType>, "invalid integer type");

 public:
  using ValueType = _ValueType;

  constexpr StrongInt() : value_(0) {}
  constexpr explicit StrongInt(ValueType value) : value_(value) {}

  constexpr ValueType value() const { return value_; }

  template <typename ValType>
  constexpr ValType value() const {
    return static_cast<ValType>(value_);
  }

  // prefix ++
  StrongInt<ValueType, UniqueName>& operator++() {
    ++value_;
    return *this;
  }
  // postfix ++
  const StrongInt<ValueType, UniqueName> operator++(int) {
    StrongInt<ValueType, UniqueName> temp(*this);
    ++value_;
    return temp;
  }
  // prefix --
  StrongInt<ValueType, UniqueName>& operator--() {
    --value_;
    return *this;
  }
  // postfix --
  const StrongInt<ValueType, UniqueName> operator--(int) {
    StrongInt<ValueType, UniqueName> temp(*this);
    --value_;
    return temp;
  }

  constexpr bool operator!() const { return value_ == 0; }
  constexpr const StrongInt<ValueType, UniqueName> operator+() const {
    return StrongInt<ValueType, UniqueName>(value_);
  }
  constexpr const StrongInt<ValueType, UniqueName> operator-() const {
    return StrongInt<ValueType, UniqueName>(-value_);
  }
  constexpr const StrongInt<ValueType, UniqueName> operator~() const {
    return StrongInt<ValueType, UniqueName>(~value_);
  }

#define STRONG_INT_ASSIGNMENT_OP(op)                                   \
  StrongInt<ValueType, UniqueName>& operator op(                       \
      const StrongInt<ValueType, UniqueName>& arg_value) {             \
    value_ op arg_value.value();                                       \
    return *this;                                                      \
  }                                                                    \
  StrongInt<ValueType, UniqueName>& operator op(ValueType arg_value) { \
    value_ op arg_value;                                               \
    return *this;                                                      \
  }
  STRONG_INT_ASSIGNMENT_OP(+=)
  STRONG_INT_ASSIGNMENT_OP(-=)
  STRONG_INT_ASSIGNMENT_OP(*=)
  STRONG_INT_ASSIGNMENT_OP(/=)
  STRONG_INT_ASSIGNMENT_OP(<<=)
  STRONG_INT_ASSIGNMENT_OP(>>=)
  STRONG_INT_ASSIGNMENT_OP(%=)
#undef STRONG_INT_ASSIGNMENT_OP

  StrongInt<ValueType, UniqueName>& operator=(ValueType arg_value) {
    value_ = arg_value;
    return *this;
  }

 private:
  ValueType value_;
} __attribute__((packed));

template <typename ValueType, typename UniqueName>
std::ostream& operator<<(std::ostream& os,  // NOLINT
                         StrongInt<ValueType, UniqueName> arg) {
  return os << arg.value();
}

#define STRONG_INT_ARITHMETIC_OP(op)                                       \
  template <typename ValueType, typename UniqueName>                       \
  static inline constexpr StrongInt<ValueType, UniqueName> operator op(    \
      StrongInt<ValueType, UniqueName> id_1,                               \
      StrongInt<ValueType, UniqueName> id_2) {                             \
    return StrongInt<ValueType, UniqueName>(id_1.value() op id_2.value()); \
  }                                                                        \
  template <typename ValueType, typename UniqueName>                       \
  static inline constexpr StrongInt<ValueType, UniqueName> operator op(    \
      StrongInt<ValueType, UniqueName> id,                                 \
      typename StrongInt<ValueType, UniqueName>::ValueType arg_val) {      \
    return StrongInt<ValueType, UniqueName>(id.value() op arg_val);        \
  }                                                                        \
  template <typename ValueType, typename UniqueName>                       \
  static inline constexpr StrongInt<ValueType, UniqueName> operator op(    \
      typename StrongInt<ValueType, UniqueName>::ValueType arg_val,        \
      StrongInt<ValueType, UniqueName> id) {                               \
    return StrongInt<ValueType, UniqueName>(arg_val op id.value());        \
  }
STRONG_INT_ARITHMETIC_OP(+)
STRONG_INT_ARITHMETIC_OP(-)
STRONG_INT_ARITHMETIC_OP(*)
STRONG_INT_ARITHMETIC_OP(/)
STRONG_INT_ARITHMETIC_OP(<<)
STRONG_INT_ARITHMETIC_OP(>>)
STRONG_INT_ARITHMETIC_OP(%)
#undef STRONG_INT_ARITHMETIC_OP

#define STRONG_INT_COMPARISON_OP(op)                              \
  template <typename ValueType, typename UniqueName>              \
  static inline constexpr bool operator op(                       \
      StrongInt<ValueType, UniqueName> id_1,                      \
      StrongInt<ValueType, UniqueName> id_2) {                    \
    return id_1.value() op id_2.value();                          \
  }                                                               \
  template <typename ValueType, typename UniqueName>              \
  static inline constexpr bool operator op(                       \
      StrongInt<ValueType, UniqueName> id,                        \
      typename StrongInt<ValueType, UniqueName>::ValueType val) { \
    return id.value() op val;                                     \
  }                                                               \
  template <typename ValueType, typename UniqueName>              \
  static inline constexpr bool operator op(                       \
      typename StrongInt<ValueType, UniqueName>::ValueType val,   \
      StrongInt<ValueType, UniqueName> id) {                      \
    return val op id.value();                                     \
  }
STRONG_INT_COMPARISON_OP(==)
STRONG_INT_COMPARISON_OP(!=)
STRONG_INT_COMPARISON_OP(<)
STRONG_INT_COMPARISON_OP(<=)
STRONG_INT_COMPARISON_OP(>)
STRONG_INT_COMPARISON_OP(>=)
#undef STRONG_INT_COMPARISON_OP

}  // namespace util

#endif  // UTIL_STRONG_INT_H_
