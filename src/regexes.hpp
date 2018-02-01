#ifndef __thodd_regexes_hpp__
#  define __thodd_regexes_hpp__

#  include <regex>
#  include <tuple>
#  include <string_view>
#  include <functional>

#  include "signatures.hpp"
#  include "structure.hpp"
#  include "start_with.hpp"


auto search_for(auto cursor, auto end, std::string_view searched, lexem::type_ type) {
  auto is_start_with = start_with(cursor, end, searched.begin(), searched.end(), std::equal_to{}) ;
  return std::make_tuple(
    is_start_with ? std::next(cursor, searched.size()) : cursor, 
    is_start_with ? type : lexem::type_::error) ;
}

auto is_pure_rx (auto begin, auto end) -> std::tuple<decltype(begin), lexem::type_> {
  return search_for(begin, end, "pure", lexem::type_::pure_kw) ;
}

auto is_impure_rx (auto begin, auto end) -> std::tuple<decltype(begin), lexem::type_> {
  return search_for(begin, end, "impure", lexem::type_::impure_kw ) ;
}

auto is_lambda_rx (auto begin, auto end) -> std::tuple<decltype(begin), lexem::type_> {
  return search_for(begin, end, "lambda", lexem::type_::lambda_kw) ;
}

auto is_pod_rx (auto begin, auto end) -> std::tuple<decltype(begin), lexem::type_> {
  return search_for(begin, end, "pod", lexem::type_::pod_kw) ;
}

auto is_return_rx (auto begin, auto end) -> std::tuple<decltype(begin), lexem::type_> {
  return search_for(begin, end, "return", lexem::type_::return_kw) ;
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
  return search_for(begin, end, "(", lexem::type_::lbracket) ;
}

auto is_rbracket_rx (auto begin, auto end) -> std::tuple<decltype(begin), lexem::type_> {
  return search_for(begin, end, ")", lexem::type_::rbracket) ;
}

auto is_lbrace_rx (auto begin, auto end) -> std::tuple<decltype(begin), lexem::type_> {
  return search_for(begin, end, "{", lexem::type_::lbrace) ;
}

auto is_rbrace_rx (auto begin, auto end) -> std::tuple<decltype(begin), lexem::type_> {
  return search_for(begin, end, "}", lexem::type_::rbrace) ;
}

auto is_colon_rx (auto begin, auto end) -> std::tuple<decltype(begin), lexem::type_> {
  return search_for(begin, end, ":", lexem::type_::colon) ;
}

auto is_semi_colon_rx (auto begin, auto end) -> std::tuple<decltype(begin), lexem::type_> {
  return search_for(begin, end, ";", lexem::type_::semi_colon) ;
}

auto is_comma_rx (auto begin, auto end) -> std::tuple<decltype(begin), lexem::type_> {
  return search_for(begin, end, ",", lexem::type_::comma) ;
}

auto is_alias_rx (auto begin, auto end) -> std::tuple<decltype(begin), lexem::type_> {
  return search_for(begin, end, "#", lexem::type_::alias) ;
}

auto is_strengh_rx (auto begin, auto end) -> std::tuple<decltype(begin), lexem::type_> {
  return search_for(begin, end, "[@]", lexem::type_::strengh) ;
}

auto is_weak_rx (auto begin, auto end) -> std::tuple<decltype(begin), lexem::type_> {
  return search_for(begin, end, "@", lexem::type_::weak) ;
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