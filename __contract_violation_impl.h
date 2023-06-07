#pragma once

#include <cstring>
#include <mutex>

#include "contract_violation.h"

namespace std {
inline namespace P2853 {

namespace detail {

struct contract_violation_impl {
  contract_violation_impl(std::contract_kind kind, const char* source_code,
                          const std::source_location& source_location) noexcept
      : kind(kind),
        source_code(source_code),
        source_location(source_location) {}
  std::contract_kind kind;
  const char* source_code;
  std::source_location source_location;
  bool what_generated = false;
  std::mutex mu;

  contract_violation_impl(const contract_violation_impl& that)
      : kind(that.kind),
        source_code(that.source_code),
        source_location(that.source_location),
        what_generated(false),
        mu() {}

  contract_violation_impl(contract_violation_impl&& that)
      : kind(std::move(that.kind)),
        source_code(std::move(that.source_code)),
        source_location(std::move(that.source_location)),
        what_generated(false),
        mu() {}

  contract_violation_impl& operator=(const contract_violation_impl& that) {
    kind = that.kind;
    source_code = that.source_code;
    source_location = that.source_location;
    what_generated = false;
    return *this;
  }

  contract_violation_impl& operator=(contract_violation_impl&& that) {
    kind = std::move(that.kind);
    source_code = std::move(that.source_code);
    source_location = std::move(that.source_location);
    what_generated = false;
    return *this;
  }
};

}  // namespace detail

struct __contract_violation_builder {
 public:
  static_assert(sizeof(detail::contract_violation_impl) + 1 <=
                std::contract_violation::size);

  static void memberwise_construct(
      std::contract_violation* v, std::contract_kind kind,
      const char* source_code, const std::source_location& source_location) {
    new (v->storage)
        detail::contract_violation_impl(kind, source_code, source_location);
  }

  static void default_construct(std::contract_violation* v) {
    memberwise_construct(v, std::contract_kind::empty, "",
                         std::source_location());
  }

  std::contract_violation cv = {std::contract_violation::__noinit};

  __contract_violation_builder(std::contract_kind kind, const char* source_code,
                               const std::source_location& source_location) {
    memberwise_construct(&cv, kind, source_code, source_location);
  }
};

}  // namespace P2853
}  // namespace std
