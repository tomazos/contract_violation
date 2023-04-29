#pragma once

#include <cstddef>
#include <exception>
#include <source_location>

namespace std {
inline namespace P2854 {

enum class contract_kind { empty, precondition, incondition, postcondition };
enum class contract_resolution { abort_program };

class contract_violation : public std::exception {
 public:
  contract_violation();

  // Precond: source_code shall point to a NTBS of static storage duration
  // Precond: contract_kind shall be exactly one of precondition, incondition or
  // postcondition
  contract_violation(std::contract_kind contract_kind, const char *source_code,
                     const std::source_location &source_location);
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
};

}  // namespace P2854
}  // namespace std

std::contract_resolution handle_contract_violation(
    const std::contract_violation &);
