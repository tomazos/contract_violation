#pragma once

#include <cstddef>
#include <exception>
#include <source_location>

namespace std {
inline namespace P2853 {

enum class contract_kind { empty, precondition, incondition, postcondition };
enum class contract_resolution { abort_program };

class contract_violation : public std::exception {
 public:
  contract_violation();

  contract_violation(const contract_violation &) noexcept;
  contract_violation(contract_violation &&) noexcept;
  ~contract_violation();
  contract_violation &operator=(const contract_violation &) noexcept;
  contract_violation &operator=(contract_violation &&) noexcept;

  const char *what() const noexcept override;

  std::contract_kind kind() const noexcept;

  // Returns either the source code/text of the contract expression
  // or the empty string if it is not available.
  const char *source_code() const noexcept;

  const std::source_location &source_location() const noexcept;

 private:
  static constexpr size_t size = 512;
  alignas(std::max_align_t) mutable char storage[size];

  static constexpr struct __noinit_t {
  } __noinit = {};
  contract_violation(__noinit_t) {}
  friend struct __contract_violation_builder;
};

}  // namespace P2853
}  // namespace std

std::contract_resolution handle_contract_violation(
    const std::contract_violation &);
