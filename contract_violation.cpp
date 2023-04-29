#include "contract_violation.h"

#include <cstring>

namespace std {
inline namespace P2854 {

namespace detail {

struct contract_violation_impl {
  contract_violation_impl(std::contract_kind kind, const char *source_code,
                          const std::source_location &source_location) noexcept
      : kind(kind),
        source_code(source_code),
        source_location(source_location) {}
  std::contract_kind kind;
  const char *source_code;
  std::source_location source_location;
  bool what_generated = false;
};

}  // namespace detail

contract_violation::contract_violation()
    : contract_violation(std::contract_kind::empty, "",
                         std::source_location()) {}

contract_violation::contract_violation(
    std::contract_kind kind, const char *source_code,
    const std::source_location &source_location) {
  static_assert(sizeof(detail::contract_violation_impl) + 1 <= size);
  new (storage)
      detail::contract_violation_impl(kind, source_code, source_location);
}

contract_violation::contract_violation(
    const contract_violation &that) noexcept {
  new (storage) detail::contract_violation_impl(
      *((detail::contract_violation_impl *)that.storage));
}

contract_violation::contract_violation(contract_violation &&that) noexcept {
  new (storage) detail::contract_violation_impl(
      std::move(*((detail::contract_violation_impl *)that.storage)));
}

contract_violation::~contract_violation() {
  ((detail::contract_violation_impl *)storage)->~contract_violation_impl();
}

contract_violation &contract_violation::operator=(
    const contract_violation &that) noexcept {
  *((detail::contract_violation_impl *)storage) =
      *((detail::contract_violation_impl *)that.storage);
  return *this;
}

contract_violation &contract_violation::operator=(
    contract_violation &&that) noexcept {
  *((detail::contract_violation_impl *)storage) =
      std::move(*((detail::contract_violation_impl *)that.storage));
  return *this;
}

const char *contract_violation::what() const noexcept {
  char *what_msg = storage + sizeof(detail::contract_violation_impl);
  bool &what_generated =
      ((detail::contract_violation_impl *)storage)->what_generated;
  if (what_generated) return what_msg;
  char *what_cursor = what_msg;
  char *what_end = storage + size;
  auto write_bytes = [&](const char *in, size_t len) {
    const size_t available = what_end - what_cursor;
    if (available < len) len = available;
    std::memcpy(what_cursor, in, len);
    what_cursor += len;
  };
  auto write_str = [&](const char *in) { write_bytes(in, std::strlen(in)); };
  auto write_num = [&](int num) {
    if (num == 0) {
      write_bytes("0", 1);
      return;
    }
    size_t ndigits = 0;
    int remainder = num;
    while (remainder > 0) {
      ndigits++;
      remainder /= 10;
    }
    const size_t available = what_end - what_cursor;
    if (ndigits > available) return;
    remainder = num;
    for (size_t i = 0; i < ndigits; i++) {
      what_cursor[ndigits - i - 1] = char(int('0') + (remainder % 10));
      remainder /= 10;
    }
    what_cursor += ndigits;
  };

  std::source_location location = source_location();

  write_str(location.file_name());
  write_str(":");
  write_num(location.line());
  write_str(":");
  write_num(location.column());
  write_str(":");
  write_str(location.function_name());
  write_str(": ");

  switch (kind()) {
    case std::contract_kind::precondition:
      write_str("precondition violation: ");
      break;
    case std::contract_kind::incondition:
      write_str("incondition violation:  ");
      break;
    case std::contract_kind::postcondition:
      write_str("postcondition violation: ");
      break;
    default:
      write_str("empty violation: ");
      break;
  }
  write_str(source_code());
  what_generated = true;
  return what_msg;
}

std::contract_kind contract_violation::kind() const noexcept {
  return ((detail::contract_violation_impl *)storage)->kind;
}

const char *contract_violation::source_code() const noexcept {
  return ((detail::contract_violation_impl *)storage)->source_code;
}

const std::source_location &contract_violation::source_location()
    const noexcept {
  return ((detail::contract_violation_impl *)storage)->source_location;
}

}  // namespace P2854
}  // namespace std
