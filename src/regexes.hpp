#ifndef __thodd_regexes_hpp__
#  define __thodd_regexes_hpp__

#  include <regex>
#  include <tuple>
#  include <string_view>
#  include <functional>

#  include "signatures.hpp"
#  include "structure.hpp"
#  include "start_with.hpp"


auto next_for(auto cursor, auto end, std::string_view searched) {
   return start_with(
      cursor, end, 
      searched.begin(), searched.end(), 
      std::equal_to{}) ?  std::next(cursor, searched.size()) : cursor  ;
}

auto is_pure_rx (auto begin, auto end) -> std::tuple<decltype(begin), lexem::type_> {
  auto cursor = next_for(begin, end, "pure") ;
  return std::make_tuple(cursor, cursor != begin ? lexem::type_::pure_kw : lexem::type_::error) ;
}

auto is_impure_rx (auto begin, auto end) -> std::tuple<decltype(begin), lexem::type_> {
  auto cursor = next_for(begin, end, "impure") ;
  return std::make_tuple(cursor, cursor != begin ? lexem::type_::impure_kw : lexem::type_::error) ;
}

auto is_lambda_rx (auto begin, auto end) -> std::tuple<decltype(begin), lexem::type_> {
  auto cursor = next_for(begin, end, "lambda") ;
  return std::make_tuple(cursor, cursor != begin ? lexem::type_::lambda_kw : lexem::type_::error) ;
}

auto is_pod_rx (auto begin, auto end) -> std::tuple<decltype(begin), lexem::type_> {
  auto cursor = next_for(begin, end, "pod") ;
  return std::make_tuple(cursor, cursor != begin ? lexem::type_::pod_kw : lexem::type_::error) ;
}

auto is_return_rx (auto begin, auto end) -> std::tuple<decltype(begin), lexem::type_> {
  auto cursor = next_for(begin, end, "return") ;
  return std::make_tuple(cursor, cursor != begin ? lexem::type_::return_kw : lexem::type_::error) ;
}

auto is_identifier_rx (auto begin, auto end) -> std::tuple<decltype(begin), lexem::type_> {
  auto cursor = begin ;
  
  while (('a' <= *cursor && *cursor <= 'z') || *cursor == '_')
    cursor = std::next(cursor) ;

  return std::make_tuple(cursor, cursor != begin ? lexem::type_::identifier : lexem::type_::error) ;
}

auto is_identifiers_rx (auto begin, auto end) -> std::tuple<decltype(begin), lexem::type_> {
  auto cursor = begin ;
  
  while (('a' <= *cursor && *cursor <= 'z') || *cursor == '_')
    cursor = std::next(cursor) ;

  if (cursor == begin)
    return std::make_tuple(begin, lexem::type_::error) ;

  while (*cursor == '.') {
    auto save = cursor ;
    cursor = std::next(cursor) ;

    while (('a' <= *cursor && *cursor <= 'z') || *cursor == '_')
      cursor = std::next(cursor) ;

    if (cursor == save)
      return std::make_tuple(begin, lexem::type_::error) ;
  }

  return std::make_tuple(cursor, cursor != begin ? lexem::type_::identifiers : lexem::type_::error) ;
}

auto is_lbracket_rx (auto begin, auto end) -> std::tuple<decltype(begin), lexem::type_> {
  auto cursor = next_for(begin, end, "(") ;
  return std::make_tuple(cursor, cursor != begin ? lexem::type_::lbracket : lexem::type_::error) ;
}

auto is_rbracket_rx (auto begin, auto end) -> std::tuple<decltype(begin), lexem::type_> {
  auto cursor = next_for(begin, end, ")") ;
  return std::make_tuple(cursor, cursor != begin ? lexem::type_::rbracket : lexem::type_::error) ;
}

auto is_lbrace_rx (auto begin, auto end) -> std::tuple<decltype(begin), lexem::type_> {
  auto cursor = next_for(begin, end, "{") ;
  return std::make_tuple(cursor, cursor != begin ? lexem::type_::lbrace : lexem::type_::error) ;
}

auto is_rbrace_rx (auto begin, auto end) -> std::tuple<decltype(begin), lexem::type_> {
  auto cursor = next_for(begin, end, "}") ;
  return std::make_tuple(cursor, cursor != begin ? lexem::type_::rbrace : lexem::type_::error) ;
}

auto is_colon_rx (auto begin, auto end) -> std::tuple<decltype(begin), lexem::type_> {
  auto cursor = next_for(begin, end, ":") ;
  return std::make_tuple(cursor, cursor != begin ? lexem::type_::colon : lexem::type_::error) ;
}

auto is_semi_colon_rx (auto begin, auto end) -> std::tuple<decltype(begin), lexem::type_> {
  auto cursor = next_for(begin, end, ";") ;
  return std::make_tuple(cursor, cursor != begin ? lexem::type_::semi_colon : lexem::type_::error) ;
}

auto is_comma_rx (auto begin, auto end) -> std::tuple<decltype(begin), lexem::type_> {
  auto cursor = next_for(begin, end, ",") ;
  return std::make_tuple(cursor, cursor != begin ? lexem::type_::comma : lexem::type_::error) ;
}

auto is_alias_rx (auto begin, auto end) -> std::tuple<decltype(begin), lexem::type_> {
  auto cursor = next_for(begin, end, "#") ;
  return std::make_tuple(cursor, cursor != begin ? lexem::type_::alias : lexem::type_::error) ;
}

auto is_strengh_rx (auto begin, auto end) -> std::tuple<decltype(begin), lexem::type_> {
  auto cursor = next_for(begin, end, "[@]") ;
  return std::make_tuple(cursor, cursor != begin ? lexem::type_::strengh : lexem::type_::error) ;
}

auto is_weak_rx (auto begin, auto end) -> std::tuple<decltype(begin), lexem::type_> {
  auto cursor = next_for(begin, end, "@") ;
  return std::make_tuple(cursor, cursor != begin ? lexem::type_::weak : lexem::type_::error) ;
}

auto is_number_rx (auto begin, auto end) -> std::tuple<decltype(begin), lexem::type_> {
  auto cursor = begin ;
  
  while (('0' <= *cursor && *cursor <= '9'))
    cursor = std::next(cursor) ;

  if (cursor == begin)
    return std::make_tuple(begin, lexem::type_::error) ;

  if (*cursor == '.') {
    auto save = cursor ;
    cursor = std::next(cursor) ;

    while (('0' <= *cursor && *cursor <= '9'))
      cursor = std::next(cursor) ;

    if (cursor == save)
      return std::make_tuple(begin, lexem::type_::error) ;
  }

  return std::make_tuple(cursor, cursor != begin ? lexem::type_::number : lexem::type_::error) ;
}

auto is_ignored_rx (auto begin, auto end) -> std::tuple<decltype(begin), lexem::type_> {
  auto cursor = begin ;
  
  while (*cursor == ' ' || *cursor == '\t' || *cursor == '\n')
    cursor = std::next(cursor) ;

  return std::make_tuple(cursor, lexem::type_::ignored) ;
}


auto
thodd_rxs (auto) {
  return std::make_tuple (
    [] (auto begin, auto end) { return is_pure_rx(begin, end) ;}, 
    [] (auto begin, auto end) { return is_impure_rx(begin, end) ;}, 
    [] (auto begin, auto end) { return is_lambda_rx(begin, end) ;}, 
    [] (auto begin, auto end) { return is_pod_rx(begin, end) ;}, 
    [] (auto begin, auto end) { return is_return_rx(begin, end) ;}, 
    [] (auto begin, auto end) { return is_identifier_rx(begin, end) ;}, 
    [] (auto begin, auto end) { return is_identifiers_rx(begin, end) ;},
    [] (auto begin, auto end) { return is_lbracket_rx(begin, end) ;}, 
    [] (auto begin, auto end) { return is_rbracket_rx(begin, end) ;}, 
    [] (auto begin, auto end) { return is_lbrace_rx(begin, end) ;},
    [] (auto begin, auto end) { return is_rbrace_rx(begin, end) ;}, 
    [] (auto begin, auto end) { return is_colon_rx(begin, end) ;}, 
    [] (auto begin, auto end) { return is_semi_colon_rx(begin, end) ;},
    [] (auto begin, auto end) { return is_comma_rx(begin, end) ;}, 
    [] (auto begin, auto end) { return is_alias_rx(begin, end) ;}, 
    [] (auto begin, auto end) { return is_strengh_rx(begin, end) ;}, 
    [] (auto begin, auto end) { return is_number_rx(begin, end) ;}, 
    [] (auto begin, auto end) { return is_ignored_rx(begin, end) ;} );
} 


#endif