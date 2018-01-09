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

struct identifier {
  lexem ident ;
} ;

inline extracted<auto, identifier> 
next_identifier (auto begin, auto const end) {
  auto const identifier_ids = { thodd::identifier } ;
  
  if (auto && ident_opt = next_ids (begin, end, identifier_ids) ;
      ident_opt.has_value()) {
    return make_extracted (ident_opt.value(), identifier { *begin }) ;
  } else {
    return make_extracted (begin, identifier {}, false) ;
  }
}

struct number {
  lexem num ;
} ;

inline extracted<auto, number> 
next_number (auto begin, auto const end) {
  auto const number_ids = { thodd::number } ;
  
  if (auto && number_opt = next_ids (begin, end, number_ids) ;
      number_opt.has_value()) 
    return make_extracted (number_opt.value(), number { *begin }) ;
  else 
    return make_extracted (begin, number {}, false) ;
}

struct parameter {
  std::vector<lexem> data ;
} ;

inline extracted<auto, parameter> 
next_param(auto begin, auto const end, bool mandatory = true) {
  auto && function_call_ext = next_function_call (begin, end, false) ;
  std::vector<lexem> pdata ;
  auto last = begin ; 

  if (!function_call_ext.unit.has_value()) {    
    auto && ident_ext = next_identifier (begin, end) ;

    if (!ident_ext.unit.has_value()) {
      auto && number_ext = next_number (begin, end) ;

      if (!number_ext.unit.has_value()) {
        error (mandatory, "un paramêtre de fonction valide (identifiant, nomber, appel de fonction) est attendu") () ;
        return make_extracted(begin, parameter{}, false) ;
      } else // interpretation number ;
        last = number_ext.last ;
    } else // interpretation identifiant ;
      last = ident_ext.last ;
  } else // interpretation function_call ;
    last = function_call_ext.last ;

  std::copy(begin, last, std::back_inserter(pdata)) ;
  return make_extracted(last, parameter{pdata}) ;
}

struct function_call {
  identifier name ;
  std::vector<parameter> params ;
} ;


inline extracted<auto, function_call> 
next_function_call (auto begin, auto const end, bool mandatory = true) {
  auto && ident_ext = next_identifier(begin, end) ;
  
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
      auto const rbracket_ids = { thodd::rbracket } ;
      auto && rbracket_opt = next_ids(lbracket_opt.value(), end, rbracket_ids) ;
      
      if (!rbracket_opt.has_value()) { // apparement oui il y a des parametres.
        std::vector<parameter> params ;
        bool has_parameter = true ;
        auto params_cursor = lbracket_opt.value() ;

        while (has_parameter) {
          auto && param = next_param (params_cursor, end) ;
          
          if (param.unit.has_value()) { 
            params.push_back(param.unit.value()) ;
            params_cursor = param.last ;
            auto const comma_ids = { thodd::comma } ;
            auto && comma_opt = next_ids(param.last, end, comma_ids) ;
            
            if (comma_opt.has_value()) 
              params_cursor = comma_opt.value() ;
            else // il n'y a pas de virgule donc plus de parametre
              has_parameter = false ;
          } else { // une erreur est survenue donc pas de parametre detecté
            error(mandatory, "la détection de paramètre à echouer") () ;
            has_parameter = false ;
          }
        } // il n'y a plus de parametre. 

        // est ce qu'il y a une parenthèse fermante ?
        auto && final_rbracket_opt = next_ids(params_cursor, end, rbracket_ids) ;
        if (!final_rbracket_opt.has_value()) { // aie ! malformation de l'appel de fonction
          error (mandatory, "')' est attendue") () ;
          return make_extracted (begin, function_call{}) ;
        } else // il y a une parenthèse fermante, donc tout s'est bien passé. 
          return make_extracted(final_rbracket_opt.value(), function_call{ident_ext.unit.value(), params}) ;
      } else { // parenthèse fermante detectée donc pas de parametre.
        return make_extracted(rbracket_opt.value(), function_call{ident_ext.unit.value()}) ;
      } 
    } 
  }
}

inline void
read_parameter (parameter const & fparam) {
  std::cout << '-' ;
  std::for_each (
    fparam.data.begin(), fparam.data.end(), 
    [] (auto const & part) {
      std::cout << part.str ;
    }) ;

  std::cout << '-' << std::endl ;
}

inline void 
read_function_call (function_call const & fcall) {
  std::cout << fcall.name.ident.str << std::endl ;
  std::for_each(
    fcall.params.begin(), fcall.params.end(), 
    [](auto const & param) {
      read_parameter (param) ;
    }) ;
}



int main(int argc, char** argv) {  
  // Bibliothèque des regex.
  auto const identifier_rx = rx { std::regex("^[a-z_]+"), thodd::identifier } ;
  auto const lbracket_rx = rx { std::regex ("^\\("), thodd::lbracket } ;
  auto const rbracket_rx = rx { std::regex ("^\\)"), thodd::rbracket } ;
  auto const colon_rx = rx { std::regex ("^:"), thodd::colon } ;
  auto const semi_colon_rx = rx { std::regex ("^;"), thodd::semi_colon } ;
  auto const comma_rx = rx { std::regex ("^,"), thodd::comma } ;
  auto const number_rx = rx { std::regex ("^[0-9]+(\\.[0-9]+){0,1}"), thodd::number } ;
  auto const ignored_rx = rx { std::regex ("^[ \\n\\t]+"), thodd::ignored } ;

  auto const stream = std::string ("__add ( __add (1,2,3,4,5,6), 98,5,__add(5, 65),3) ");

  std::cout << stream << std::endl ;

  auto && lexems_instruction = 
    make_thodd_lexems(
      stream.cbegin(), stream.cend(), 
      identifier_rx, lbracket_rx, rbracket_rx, 
      comma_rx, ignored_rx, number_rx, semi_colon_rx) ;  
  
  std::vector<lexem> lexems_filtered ;
  filter_lexems (lexems_instruction, lexems_filtered) ;
    
  auto && add_call_ext = next_function_call (lexems_filtered.begin(), lexems_filtered.end()) ;
  std::cout << add_call_ext.unit.value().params.size() << std::endl ;
  read_function_call(add_call_ext.unit.value()) ;
}