#include "contract_violation.h"

#include <cassert>
#include <iostream>
#include <string>

#include "__contract_violation_impl.h"

int main() {
  // default construction
  std::contract_violation v0;
  assert(v0.kind() == std::contract_kind::empty);

  std::source_location source_location = std::source_location::current();

  // memberwise construction (private implementation)
  std::__contract_violation_builder v1(std::contract_kind::precondition,
                                       "x + y > 2", source_location);
  assert(v1.cv.kind() == std::contract_kind::precondition);
  assert(std::string(v1.cv.source_code()) == "x + y > 2");
  assert(v1.cv.source_location().line() == source_location.line());

  // copy construction
  std::contract_violation v2(v1.cv);
  assert(v2.kind() == std::contract_kind::precondition);
  assert(std::string(v2.source_code()) == "x + y > 2");
  assert(v2.source_location().line() == source_location.line());

  // move construction
  std::contract_violation v3(std::move(v2));
  assert(v3.kind() == std::contract_kind::precondition);
  assert(std::string(v3.source_code()) == "x + y > 2");
  assert(v3.source_location().line() == source_location.line());

  // copy assignment
  std::contract_violation v4;
  v4 = v3;
  assert(v4.kind() == std::contract_kind::precondition);
  assert(std::string(v4.source_code()) == "x + y > 2");
  assert(v4.source_location().line() == source_location.line());

  // move assignment
  std::contract_violation v5;
  v5 = std::move(v4);
  assert(v5.kind() == std::contract_kind::precondition);
  assert(std::string(v5.source_code()) == "x + y > 2");
  assert(v5.source_location().line() == source_location.line());

  // .what()
  std::cout << v5.what() << std::endl;
}
