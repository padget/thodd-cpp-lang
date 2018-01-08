#include <iostream>
#include <string>
#include <regex>
#include <algorithm>
#include <list>
#include <optional>
#include <type_traits>
#include <functional>

namespace std {
  inline auto 
  make_empty_optional (auto && v) {
    return optional<std::decay_t<decltype(v)>>() ;
  }

  inline auto 
  if_exists (auto && opt, auto then) {
    if (std::forward<decltype(opt)>(opt).has_value()) 
      then(std::forward<decltype(opt)>(opt).value()) ;
  }

  inline auto 
  if_exists (auto && opt, auto fthen, auto felse) {
    if (std::forward<decltype(opt)>(opt).has_value()) 
      fthen(std::forward<decltype(opt)>(opt).value()) ;
    else felse() ;
  }

  inline auto 
  if_not_exists (auto && opt, auto fthen) {
    if (!opt.has_value()) fthen() ; 
  }
}


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

template <typename iterator_t, typename unit_t> 
struct extracted {
  iterator_t last ;
  std::optional<unit_t> unit ;
} ;

inline auto 
make_extracted (auto last, auto && unit, bool exists = true) {
  return extracted<decltype(last), std::decay_t<decltype(unit)>> 
  { last, exists ? std::make_optional (std::forward<decltype(unit)>(unit)) : 
                   std::make_empty_optional(std::forward<decltype(unit)>(unit)) } ;
}

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

inline auto 
error (bool mandatory, std::string const & message) {
  return [&] {
    if (mandatory)
      std::cout << "error : " << message << std::endl ;
  } ;
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
    std:if_exists(
      next_ids(cursor, end, ids), 
      [&] (auto && local_cursor) {
        can_continue = true ;
        cursor = local_cursor ;
        std::if_exists(
          next_ids(cursor, end, sep_ids), 
          [&] (auto && sep_cursor) {
            can_continue = true ; 
            cursor = sep_cursor ;
          }, 
          [&] { can_continue = false ; }) ;
      }) ;
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

struct identifier {
  lexem ident ;
} ;

inline extracted<auto, identifier> 
next_identifier2 (auto begin, auto const end) {
  auto const identifier_ids = { thodd::identifier } ;
  
  if (auto && ident_opt = next_ids (begin, end, identifier_ids) ;
      ident_opt.has_value()) {
    return make_extracted (ident_opt.value(), identifier { *ident_opt.value() }) ;
  } else {
    return make_extracted (begin, identifier {}, false) ;
  }
}

struct number {
  lexem num ;
} ;

inline std::optional<auto>
next_number (auto begin, auto end) {
  auto const number_ids = { thodd::number } ;
  return next_ids(begin, end, number_ids) ;
}

inline extracted<auto, number> 
next_number2 (auto begin, auto const end) {
  auto const number_ids = { thodd::number } ;
  
  if (auto && number_opt = next_ids (begin, end, number_ids) ;
      number_opt.has_value()) 
    return make_extracted (number_opt.value(), number { *number_opt.value() }) ;
  else 
    return make_extracted (begin, number {}, false) ;
}

inline std::optional<auto>
next_identifier_or_number(auto begin, auto end) {
  auto && res = next_identifier(begin, end) ;
  return res.has_value() ? res : next_number(begin, end) ;
}

struct parameter {

} ;


inline extracted<auto, parameter> 
next_param(auto begin, auto const end, bool mandatory = true) {
  auto && function_call_ext = next_function_call2 (begin, end, false) ;
      
  if (!function_call_ext.unit.has_value()) {    
    auto && ident_ext = next_identifier2 (begin, end) ;

    if (!ident_ext.unit.has_value()) {
      auto && number_ext = next_number2 (begin, end) ;

      if (!number_ext.unit.has_value()) {
        error (mandatory, "un paramêtre de fonction valide (identifiant, nomber, appel de fonction) est attendu") () ;
        return make_extracted(begin, parameter{}, false) ;
      } else {
        // interpretation number ;
        std::cout << "je suis un number" << std::endl ;
        return make_extracted(number_ext.last, parameter{}) ;
      }
    } else {
      // interpretation identifiant ;
      std::cout << "je suis un identifiant" << std::endl ;
      return make_extracted(ident_ext.last, parameter{}) ;
    }
  } else {
    // interpretation function_call ;
    std::cout << "je suis un appel de fonction" << std::endl ;
    return make_extracted(function_call_ext.last, parameter{}) ;
  }
}

struct function_call {
  identifier name ;
  std::vector<parameter> params ;
} ;


inline extracted<auto, function_call> 
next_function_call2 (auto begin, auto const end, bool mandatory = true) {
  auto && ident_ext = next_identifier2(begin, end) ;
  
  if (!ident_ext.unit.has_value()) { 
    error(mandatory, "identifiant attendu") () ;
    return make_extracted(begin, function_call{}, false) ;  
  } else {
    auto const lbracket_ids = { thodd::lbracket } ;
    auto && lbracket_opt = next_ids(ident_ext.last, end, lbracket_ids) ;
    
    if (!lbracket_opt.has_value()) { 
      error (mandatory, "'(' attendu") () ;
      return make_extracted(begin, function_call{}, false) ;
    } else { // est ce qu'il y a des parametres ?  
      auto const rbracket_ids = { thodd::lbracket } ;
      auto && rbracket_opt = next_ids(lbracket_opt.value(), end, rbracket_ids) ;
      
      if (!rbracket_opt.has_value()) { // apparement oui il y a des parametres.
        std::vector<parameter> params ;
        bool has_parameter = true ;
        auto params_cursor = lbracket_opt.value() ;

        while (has_parameter) {
          auto && param = next_param (params_cursor, end) ;
          
          if (param.unit.has_value()) { 
            params.push_back(param.unit.value()) ;
            auto const comma_ids = { thodd::comma } ;
            auto && comma_opt = next_ids(param.last, end, comma_ids) ;
            
            if (comma_opt.has_value()) 
              params_cursor = comma_opt.value() ;
            else // il n'y a pas de virgule donc plus de parametre
              has_parameter = false ; 
          } else { // une erreur est survenue donc pas de parametre detecté
            has_parameter = false ;
          }
        } // il n'y a plus de parametre. 

        // est ce qu'il y a une parenthèse fermante ?
        auto && final_rbracket_opt = next_ids(params_cursor, end, rbracket_ids) ;
        if (!final_rbracket_opt.has_value()) { // aie ! malformation de l'appel de fonction
          error (mandatory, "Une parenthèse fermante est attendue") () ;
          return make_extracted (begin, function_call{}) ;
        } else // il y a une parenthèse fermante, donc tout s'est bien passé. 
          return make_extracted(final_rbracket_opt.value(), function_call{ident_ext.unit.value(), params}) ;
      } else { // parenthèse fermante detectée donc pas de parametre.
        return make_extracted(rbracket_opt.value(), function_call{ident_ext.unit.value()}) ;
      } 
    } 
  }
}

inline auto
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

inline std::optional<std::string>
interpret_expresion (auto begin, auto end) {
  // ici begin et end sont des iterator sur des lexem => (*begin).id/str est valide
  if (begin != end) {
    if (auto && function_call_end = next_function_call (begin, end) ; 
        function_call_end.has_value()) {
      // ici on sait qu'on affaire à un appel de fonction. 
      // Il faut donc savoir s'il y a des paramêtres ou non.
      
    }
  }
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
  
  auto && add_call = next_function_call2 (lexems_filtered.begin(), lexems_filtered.end()) ;
  //std::cout << add_call.unit.value().params.size() << std::endl ;
}