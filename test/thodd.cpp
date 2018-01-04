#include <iostream>
#include <string>
#include <regex>
#include <algorithm>
#include <list>
#include <optional>
#include <type_traits>
#include <functional>

enum class thodd : int {
  identifier, 
  lbrace, rbrace, 
  lbracket, rbracket,
  lsbracket, rsbracket,
  colon, semi_colon, 
  point, comma, ignored 
} ;

struct rx {
  std::regex reg ; 
  thodd id ;
} ;

struct lexem {
  std::string str ;
  thodd id ; 
} ;

inline auto
make_thodd_lexems (auto begin, auto const end, auto const & ... regexs) {
  std::smatch matched ; 
  std::list<lexem> lexems ;
  auto const rxs = { regexs ... } ;

  while (std::not_equal_to{}(begin, end)) {
    std::vector<lexem> matchs ;
    
    std::transform (
      rxs.begin(), rxs.end(), std::back_inserter(matchs), 
      [&] (auto && rx) { 
        std::regex_search(begin, end, matched, rx.reg) ; 
        return lexem { matched.str(), rx.id } ; 
      }) ;

    auto max = std::max_element (
      matchs.cbegin(), matchs.cend(), 
      [] (auto const & l, auto const & r) { 
        return l.str.size() < r.str.size() ; 
      }) ;
    
    std::advance (begin, (*max).str.size() == 0 ? 1 : (*max).str.size()) ;
    
    if ((*max).str.size() != 0)
      lexems.push_back(*max) ;
  } 

  return lexems ;
}

inline void 
filter_lexems (auto const & lexems, auto & lexems_filtered) {
  std::copy_if(
    lexems.cbegin(), lexems.cend(), 
    std::back_inserter(lexems_filtered), 
    [] (auto const & lexem) { return lexem.id != thodd::ignored ; }) ;
}

namespace std {
  inline auto 
  make_empty_optional (auto && v) {
    return optional<std::decay_t<decltype(v)>>() ;
  }
}

inline auto const 
to_ids (auto begin, size_t nb) {
  std::vector<thodd> ids ;
  std::transform (
    begin, std::next(begin, nb), 
    std::back_inserter(ids), 
    [] (auto lexem) { return lexem.id ; }) ;

  return ids ; 
} ;

inline bool
ids_equal(auto begin, auto const & ids) {
  auto mapped_ids = to_ids(begin, ids.size()) ;
  return 
  std::equal(
    mapped_ids.begin(), mapped_ids.end(), 
    ids.begin(), ids.end()) ;
}


inline auto 
while_with_separator (auto begin, auto ids, auto sep_ids) {
  auto cursor = begin ;
  bool can_continue = true ;
  
  do {
    if (can_continue = ids_equal(cursor, ids)) {
      cursor = std::next(cursor, ids.size()) ;
      can_continue = ids_equal(cursor, sep_ids) ? 
                      (cursor = std::next(cursor), true) : false ;
    }
  } while (can_continue) ;

  return std::make_optional(cursor) ;
}

inline auto
is_signature (auto begin) {
  auto const begin_signature_ids = { thodd::identifier, thodd::lbracket } ;
  auto const param_ids           = { thodd::identifier, thodd::colon, thodd::identifier } ;
  auto const param_separator_ids = { thodd::comma } ;
  auto const end_signature_ids   = { thodd::rbracket, thodd::colon, thodd::identifier, thodd::semi_colon } ;
  
  auto cursor = begin ; 

  if (ids_equal(cursor, begin_signature_ids)) 
    cursor = std::next(cursor, begin_signature_ids.size()) ;
  else 
    return std::make_empty_optional(cursor) ;

  cursor = while_with_separator(cursor, param_ids, param_separator_ids).value_or(cursor) ;

  return 
  ids_equal(cursor, end_signature_ids) ? 
    std::make_optional(std::next(cursor, end_signature_ids.size())) : 
    std::make_empty_optional(cursor) ; 
} ;

int main(int argc, char** argv) {
  auto stream = std::string ("__add (a: int, b: int): int  ; ");
  
  // Bibliothèque des regex.
  auto const identifier_rx = rx { std::regex("^[a-z_]+"), thodd::identifier } ;
  auto const lbracket_rx = rx { std::regex ("^\\("), thodd::lbracket } ;
  auto const rbracket_rx = rx { std::regex ("^\\)"), thodd::rbracket } ;
  auto const colon_rx = rx { std::regex ("^:"), thodd::colon } ;
  auto const semi_colon_rx = rx { std::regex ("^;"), thodd::semi_colon } ;
  auto const comma_rx = rx { std::regex ("^,"), thodd::comma } ;
  auto const ignored_rx = rx { std::regex ("^ +"), thodd::ignored } ;

  // Constitution du stream de lexem.
  auto && lexems = 
    make_thodd_lexems(
      stream.cbegin(), stream.cend(), 
      identifier_rx, lbracket_rx, rbracket_rx, 
      colon_rx, semi_colon_rx, comma_rx, ignored_rx) ;  

  std::for_each(
    lexems.cbegin(), lexems.cend(), 
    [] (auto const & lexem) { std::cout << (int) lexem.id << '-' ; }) ;

  std::cout << std::endl ;

  std::list<lexem> lexems_filtered ; 
  filter_lexems(lexems, lexems_filtered) ;

  std::for_each(
    lexems_filtered.cbegin(), lexems_filtered.cend(), 
    [] (auto const & lexem) { std::cout << (int) lexem.id << '-' ; }) ;

  std::cout << std::endl ;
  
  std::cout << std::boolalpha << is_signature (lexems_filtered.begin()).has_value() 
            << std::endl ;
}