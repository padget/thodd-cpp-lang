#include <iostream>
#include <string>
#include <regex>
#include <algorithm>
#include <list>
#include <optional>
#include <type_traits>
#include <functional>

enum class thodd : int {
  identifier, number,
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

namespace std {
  inline bool
  start_with (auto begin, auto const end, auto sbegin, auto send, auto comparator) {
    while (begin != end && sbegin != send 
           && comparator(*begin, *sbegin)) {
      ++ begin ; ++ sbegin ;   
    }

    return 
    sbegin == send ;
  }

  inline bool
  start_with (auto begin, auto const end, auto sbegin, auto send) {
    return start_with(begin, end, sbegin, send, std::equal_to {}) ;
  }
}

inline std::optional<auto>
next_ids(auto begin, auto end, auto const & ids) {
  return 
  std::start_with(
    begin, end, ids.begin(), ids.end(), 
    [] (auto const & lexem, auto const & id) {return lexem.id == id ;}) ? 
  std::make_optional(std::next(begin, std::size(ids))) :
  std::make_empty_optional(begin) ;
} 




inline std::optional<auto>
while_with_separator (auto begin, auto end, auto ids, auto sep_ids) {
  auto cursor = begin ;
  bool can_continue = true ;
  
  do 
    if (auto && next = next_ids(cursor, end, ids) ; 
        can_continue = next.has_value()) {
      cursor = next.value() ;
      auto && next_sep = next_ids(cursor, end, sep_ids) ; 
      can_continue = next_sep.has_value() ? (cursor = next_sep.value(), true) : false ;
    }
  while (can_continue) ;

  return std::make_optional(cursor) ;
}

inline std::optional<auto>
next_signature (auto begin, auto end) {
  auto const begin_signature_ids = { thodd::identifier, thodd::lbracket } ;
  auto const param_ids           = { thodd::identifier, thodd::colon, thodd::identifier } ;
  auto const param_separator_ids = { thodd::comma } ;
  auto const end_signature_ids   = { thodd::rbracket, thodd::colon, thodd::identifier, thodd::semi_colon } ;
  
  auto cursor = begin ; 

  if (auto && next_begin = next_ids(cursor, end, begin_signature_ids) ; 
      next_begin.has_value()) 
    cursor = next_begin.value() ;
  else 
    return std::make_empty_optional(cursor) ;

  cursor = while_with_separator(cursor, end, param_ids, param_separator_ids).value_or(cursor) ;

  return 
  next_ids(cursor, end, end_signature_ids) ;
} ;

inline std::optional<auto>
next_identifier (auto begin, auto end) {
  auto const identifier_ids = { thodd::identifier } ;
  return next_ids(begin, end, identifier_ids) ;
}

inline std::optional<auto>
next_number (auto begin, auto end) {
  auto const number_ids = { thodd::number } ;
  return next_ids(begin, end, number_ids) ;
}

inline std::optional<auto>
next_identifier_or_number(auto begin, auto end) {
  auto && res = next_identifier(begin, end) ;
  return res.has_value() ? res : next_number(begin, end) ;
}

inline std::optional<auto>
next_function_call (auto begin, auto end) {
  auto const begin_function_call_ids = { thodd::identifier, thodd::lbracket } ;
  auto const param_separator_ids = { thodd::comma } ; 
  auto const end_function_call_ids = { thodd::rbracket } ;

  auto cursor = begin ; 
  
  if (auto && next_begin = next_ids (cursor, end, begin_function_call_ids) ; 
      next_begin.has_value()) 
    cursor = next_begin.value() ;
  else 
    return std::make_empty_optional(cursor) ;

  bool has_parameter = false ;

  do {
    if (auto && expression = next_function_call(cursor, end) ; 
        expression.has_value()) {
      has_parameter = true ;
      cursor = expression.value() ;
    } else if (auto && parameter = next_identifier_or_number(cursor, end) ; 
               parameter.has_value()) {
      has_parameter = true ;
      cursor = parameter.value() ;
    }
    
    if (has_parameter) {
      auto && next_separator = next_ids(cursor, end, param_separator_ids) ;
      has_parameter = next_separator.has_value() ? (cursor = next_separator.value(), true) : false ;
    }
  } while (has_parameter) ;
  
  return next_ids(cursor, end, end_function_call_ids) ;
}

inline std::optional<auto>
next_expression (auto begin, auto const end) {
  return next_function_call(begin, end) ;
}

inline std::optional<auto>
next_instruction (auto begin, auto const end) {
  auto const end_instruction_ids = { thodd::semi_colon } ;

  if (auto && expression = next_expression (begin, end); 
      expression.has_value())  
    return next_ids(expression.value(), end, end_instruction_ids) ;
  else 
    return std::make_empty_optional(begin) ;
}

int main(int argc, char** argv) {
  auto stream = std::string ("__add (a: int, b: int): int  ; ");
  
  // Bibliothèque des regex.
  auto const identifier_rx = rx { std::regex("^[a-z_]+"), thodd::identifier } ;
  auto const lbracket_rx = rx { std::regex ("^\\("), thodd::lbracket } ;
  auto const rbracket_rx = rx { std::regex ("^\\)"), thodd::rbracket } ;
  auto const colon_rx = rx { std::regex ("^:"), thodd::colon } ;
  auto const semi_colon_rx = rx { std::regex ("^;"), thodd::semi_colon } ;
  auto const comma_rx = rx { std::regex ("^,"), thodd::comma } ;
  auto const number_rx = rx { std::regex ("^[0-9]+(\\.[0-9]+){0,1}"), thodd::number } ;
  auto const ignored_rx = rx { std::regex ("^[ \\n\\t]+"), thodd::ignored } ;

  std::cout << "ignored id : " << (int) thodd::ignored << std::endl ;

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
  
  std::cout << std::boolalpha 
            << next_signature (lexems_filtered.begin(), lexems_filtered.end()).has_value() 
            << std::endl ;

  stream = std::string ("__add ( __add (1,2), 2,5,6,3) ");

  // Constitution du stream de lexem.
  auto && lexems_fcall = 
    make_thodd_lexems(
      stream.cbegin(), stream.cend(), 
      identifier_rx, lbracket_rx, rbracket_rx, 
      comma_rx, ignored_rx, number_rx) ;  
  
  std::for_each(
    lexems_fcall.cbegin(), lexems_fcall.cend(), 
    [] (auto const & lexem) { std::cout << (int) lexem.id << '-' ; }) ;
  std::cout << std::endl ;
  
  lexems_filtered.clear() ;
  filter_lexems (lexems_fcall, lexems_filtered) ;
  
  std::for_each(
    lexems_filtered.cbegin(), lexems_filtered.cend(), 
    [] (auto const & lexem) { std::cout << (int) lexem.id << '-' ; }) ;
  std::cout << std::endl ;

  std::cout << std::boolalpha
            << next_function_call(lexems_filtered.begin(), lexems_filtered.end()).has_value() 
            << std::endl ;

  stream += ";" ;
  std::cout << stream << std::endl ;
  auto && lexems_instruction = 
    make_thodd_lexems(
      stream.cbegin(), stream.cend(), 
      identifier_rx, lbracket_rx, rbracket_rx, 
      comma_rx, ignored_rx, number_rx, semi_colon_rx) ;  
  
  std::for_each(
    lexems_instruction.cbegin(), lexems_instruction.cend(), 
    [] (auto const & lexem) { std::cout << (int) lexem.id << '-' ; }) ;
  std::cout << std::endl ;
  
  lexems_filtered.clear() ;
  filter_lexems (lexems_instruction, lexems_filtered) ;
  
  std::for_each(
    lexems_filtered.cbegin(), lexems_filtered.cend(), 
    [] (auto const & lexem) { std::cout << (int) lexem.id << '-' ; }) ;
  std::cout << std::endl ;

  std::cout << std::boolalpha
            << next_instruction(lexems_filtered.begin(), lexems_filtered.end()).has_value() 
            << std::endl ;
}