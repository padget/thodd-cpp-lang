#ifndef __thodd_regexes_hpp__
#  define __thodd_regexes_hpp__

#  include <regex>
#  include <string_view>
#  include <functional>

#  include "structure.hpp"
#  include "start_with.hpp"

struct rx {
  std::regex reg ; 
  lexem::type_ type ;
} ;

rx make_rx (std::regex const & reg, lexem::type_ type) ;
std::vector<rx> thodd_rxs () ;

auto is_pure_rx (auto begin, auto end) -> decltype(begin) {
  std::string_view pure = "pure" ;
  return start_with(begin, end, pure.begin(), pure.end(), std::equal_to{}) ?  std::next(begin, pure.size()) : begin  ;
}

auto is_impure_rx (auto begin, auto end) -> decltype(begin) {
  std::string_view impure = "impure" ;
  return start_with(begin, end, impure.begin(), impure.end(), std::equal_to{}) ?  std::next(begin, impure.size()) : begin  ;
}

auto is_lambda_rx (auto begin, auto end) -> decltype(begin) {
  std::string_view lambda = "lambda" ;
  return start_with(begin, end, lambda.begin(), lambda.end(), std::equal_to{}) ?  std::next(begin, lambda.size()) : begin  ;
}

auto is_pod_rx (auto begin, auto end) -> decltype(begin) {
  std::string_view pod = "pod" ;
  return start_with(begin, end, pod.begin(), pod.end(), std::equal_to{}) ?  std::next(begin, pod.size()) : begin  ;
}

auto is_return_rx (auto begin, auto end) -> decltype(begin) {
  auto cursor = begin ;

  return cursor ;
}

auto is_identifier_rx (auto begin, auto end) -> decltype(begin) {
  auto cursor = begin ;

  return cursor ;
}

auto is_identifiers_rx (auto begin, auto end) -> decltype(begin) {
  auto cursor = begin ;

  return cursor ;
}

auto is_lbracket_rx (auto begin, auto end) -> decltype(begin) {
  auto cursor = begin ;

  return cursor ;
}

auto is_rbracket_rx (auto begin, auto end) -> decltype(begin) {
  auto cursor = begin ;

  return cursor ;
}

auto is_lbrace_rx (auto begin, auto end) -> decltype(begin) {
  auto cursor = begin ;

  return cursor ;
}

auto is_rbrace_rx (auto begin, auto end) -> decltype(begin) {
  auto cursor = begin ;

  return cursor ;
}

auto is_colon_rx (auto begin, auto end) -> decltype(begin) {
  auto cursor = begin ;

  return cursor ;
}

auto is_semi_colon_rx (auto begin, auto end) -> decltype(begin) {
  auto cursor = begin ;

  return cursor ;
}

auto is_comma_rx (auto begin, auto end) -> decltype(begin) {
  auto cursor = begin ;

  return cursor ;
}

auto is_alias_rx (auto begin, auto end) -> decltype(begin) {
  auto cursor = begin ;

  return cursor ;
}

auto is_strengh_rx (auto begin, auto end) -> decltype(begin) {
  auto cursor = begin ;

  return cursor ;
}

auto is_weak_rx (auto begin, auto end) -> decltype(begin) {
  auto cursor = begin ;

  return cursor ;
}

auto is_number_rx (auto begin, auto end) -> decltype(begin) {
  auto cursor = begin ;

  return cursor ;
}

auto is_ignored_rx (auto begin, auto end) -> decltype(begin) {
  auto cursor = begin ;

  return cursor ;
}

#endif