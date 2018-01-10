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
  point, comma, ignored, 
  pure_kw, impure_kw
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
  enum class type_ : size_t {
    fcall, number, identifier
  } ;

  type_ type ;
  std::vector<lexem> data ;
} ;

inline extracted<auto, parameter> 
next_param(auto begin, auto const end, bool mandatory = true) {
  auto && function_call_ext = next_function_call (begin, end, false) ;
  std::vector<lexem> pdata ;
  auto last = begin ; 
  parameter::type_ ptype ;

  if (!function_call_ext.unit.has_value()) {    
    auto && ident_ext = next_identifier (begin, end) ;

    if (!ident_ext.unit.has_value()) {
      auto && number_ext = next_number (begin, end) ;

      if (!number_ext.unit.has_value()) {
        error (mandatory, "un paramêtre de fonction valide (identifiant, nomber, appel de fonction) est attendu") () ;
        return make_extracted(begin, parameter{}, false) ;
      } else // interpretation number ;
        (last = number_ext.last, ptype = parameter::type_::number) ;
    } else // interpretation identifiant ;
      (last = ident_ext.last, ptype = parameter::type_::identifier) ;
  } else // interpretation function_call ;
    (last = function_call_ext.last, ptype = parameter::type_::fcall) ;

  std::copy(begin, last, std::back_inserter(pdata)) ;
  return make_extracted(last, parameter{ptype, pdata}) ;
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


struct argument {
  identifier name ;
  identifier type ;
} ;

inline extracted<auto, argument> 
next_argument (auto begin, auto const end, bool mandatory = true) {
  auto && name_ext = next_identifier (begin, end) ;

  if (!name_ext.unit.has_value()) {
    error (mandatory, "identifiant attendu") () ;
    return make_extracted(begin, argument{}, false) ;
  } else {
    auto const colon_ids = { thodd::colon } ;
    auto && colon_opt = next_ids (name_ext.last, end, colon_ids) ;

    if (!colon_opt.has_value()) {
       error (mandatory, "':' attendu") () ;
       return make_extracted(begin, argument{}, false) ;
    } else {
      auto && type_ext = next_identifier(colon_opt.value(), end) ;
      
      if (!type_ext.unit.has_value()) {
        error (mandatory, "identifiant attendu") () ;
        return make_extracted(begin, argument{}, false) ;
      } else 
        return make_extracted (type_ext.last, argument { name_ext.unit.value(), type_ext.unit.value() }) ;
    }
  }
}

struct function_signature {
  enum class purity_ { pure, impure } ;

  identifier name ;
  identifier type ;
  std::vector<argument> args ;
  purity_ purity ;
} ;

inline extracted<auto, function_signature> 
next_function_signature (auto begin, auto const end, bool mandatory = true) {
  auto && name_ext = next_identifier (begin, end) ;

  if (!name_ext.unit.has_value()) {
    error (mandatory, "identifiant attendu") () ;
    return make_extracted(begin, function_signature{}, false) ;
  } else {
    auto const lbracket_ids = { thodd::lbracket } ;
    auto && lbracket_opt = next_ids(name_ext.last, end, lbracket_ids) ;
    
    if (!lbracket_opt.has_value()) { 
      error (mandatory, "'(' attendu") () ;
      return make_extracted(begin, function_signature{}, false) ;
    } else {
      auto const rbracket_ids = { thodd::rbracket } ;
      auto && rbracket_opt = next_ids(lbracket_opt.value(), end, rbracket_ids) ;
      
      if (!rbracket_opt.has_value()) { // apparement oui il y a des parametres.
        std::vector<argument> args ;
        bool has_argument = true ;
        auto args_cursor = lbracket_opt.value() ;

        while (has_argument) {
          auto && arg = next_argument (args_cursor, end, mandatory) ;
          
          if (arg.unit.has_value()) { 
            args.push_back(arg.unit.value()) ;
            args_cursor = arg.last ;
            auto const comma_ids = { thodd::comma } ;
            auto && comma_opt = next_ids(arg.last, end, comma_ids) ;
            
            if (comma_opt.has_value()) 
              args_cursor = comma_opt.value() ;
            else // il n'y a pas de virgule donc plus de parametre
              has_argument = false ;
          } else { // une erreur est survenue donc pas de parametre detecté
            error(mandatory, "la détection d'argument à echouer") () ;
            has_argument = false ;
          }
        } // il n'y a plus de parametre. 

        // est ce qu'il y a une parenthèse fermante ?
        auto && final_rbracket_opt = next_ids(args_cursor, end, rbracket_ids) ;
        
        if (!final_rbracket_opt.has_value()) { // aie ! malformation de l'appel de fonction
          error (mandatory, "')' est attendue") () ;
          return make_extracted (begin, function_signature{}) ;
        } else  {
          auto const colon_ids = { thodd::colon } ;
          auto && colon_opt = next_ids (final_rbracket_opt.value(), end, colon_ids) ;

          if (!colon_opt.has_value()) {
            error (mandatory, "':' attendu") () ;
            return make_extracted(begin, function_signature{}, false) ;
          } else {
            auto && type_ext = next_identifier(colon_opt.value(), end) ;
            
            if (!type_ext.unit.has_value()) {
              error (mandatory, "identifiant attendu") () ;
              return make_extracted(begin, function_signature{}, false) ;
            } else 
              return make_extracted (type_ext.last, function_signature { name_ext.unit.value(), type_ext.unit.value(), args }) ;
          }
        }
      } else {
        auto const colon_ids = { thodd::colon } ;
        auto && colon_opt = next_ids (name_ext.last, end, colon_ids) ;

        if (!colon_opt.has_value()) {
          error (mandatory, "':' attendu") () ;
          return make_extracted(begin, function_signature{}, false) ;
        } else {
          auto && type_ext = next_identifier(colon_opt.value(), end) ;
          
          if (!type_ext.unit.has_value()) {
            error (mandatory, "identifiant attendu") () ;
            return make_extracted(begin, function_signature{}, false) ;
          } else 
            return make_extracted (type_ext.last, function_signature { name_ext.unit.value(), type_ext.unit.value() }) ;
        }
      }
    } 
  }
}

struct instruction {
  enum class type_ { return_fcall, return_number, return_identifier } ;

  type_ type ;
  std::vector<lexem> data ;
} ;

struct function_definition {
  function_signature sig ;
  std::vector<instruction> insts; 
} ;

size_t depth = 0u ;

inline std::string indent () {
  return std::string (2 * depth, ' ') ;
}

namespace cpp {
  inline std::string const
  transpile_function_call (function_call const & fcall) ;

  inline std::string const
  transpile_number (number const & fnum) {
    return fnum.num.str ;
  }

  inline std::string const 
  transpile_identifier (identifier const & fident) {
    return fident.ident.str ;
  }

  inline std::string const
  transpile_parameter (parameter const & fparam) {  
    switch (fparam.type) {
      case parameter::type_::number : 
        return 
        transpile_number(
          next_number(
            fparam.data.begin(), 
            fparam.data.end()).unit.value()) ; 
      case parameter::type_::identifier : 
        return 
        transpile_identifier(
          next_identifier(
            fparam.data.begin(), 
            fparam.data.end()).unit.value()) ; 
      case parameter::type_::fcall : 
        return
        transpile_function_call(
          next_function_call(
            fparam.data.begin(), 
            fparam.data.end()).unit.value()) ; 
    }
  }

  inline std::string const  
  transpile_function_call (function_call const & fcall) {
    std::string cpp = transpile_identifier(fcall.name) + "(" ;
    
    std::for_each(
      fcall.params.begin(), fcall.params.end(), 
      [&cpp] (auto const & param) {
        cpp += transpile_parameter (param) + ", " ;
      }) ;

    cpp.pop_back() ; cpp.pop_back() ;  
    cpp += ")" ;

    return cpp ;
  }

  inline std::string const 
  transpile_argument(argument const & arg) {
    return 
    transpile_identifier(arg.type) + " " + 
    transpile_identifier(arg.name) ;
  }

  inline std::string const 
  transpile_function_signature (function_signature const & fsignature) {
    std::string cpp = transpile_identifier(fsignature.name) + "(" ;
    
    std::for_each(
      fsignature.args.begin(), fsignature.args.end(), 
      [&cpp] (auto const & arg) {
        cpp += transpile_argument (arg) + ", " ;
      }) ;

    cpp.pop_back() ; cpp.pop_back() ;  
    cpp += ") : " + transpile_identifier(fsignature.type) ;

    return cpp ;  
  }

  inline std::string const
  transpile_instruction (instruction const & inst) {
    switch (inst.type) {
      case instruction::type_::return_fcall : 
        return 
        transpile_function_call(
          next_function_call(
            inst.data.begin(), 
            inst.data.end()).unit.value()) + " ;" ; 
      case instruction::type_::return_number : 
        return 
        transpile_number(
          next_number(
            inst.data.begin(), 
            inst.data.end()).unit.value()) + " ;" ; 
      case instruction::type_::return_identifier :
        return
        transpile_identifier(
          next_identifier(
            inst.data.begin(), 
            inst.data.end()).unit.value()) + " ;" ;  
    }
  }

  inline std::string const 
  transpile_function_definition (function_definition const & fdef) {
    return 
    transpile_function_signature(fdef.sig) + " {\n" +
    transpile_// instruction
  }
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
  auto const pure_rx = rx { std::regex ("^pure"), thodd::pure_kw } ;
  auto const impure_rx = rx { std::regex ("^impure"), thodd::impure_kw } ;

  auto const stream = std::string ("__add (a     : int  , b  :int):int");

  std::cout << stream << std::endl ;

  auto && lexems_instruction = 
    make_thodd_lexems(
      stream.cbegin(), stream.cend(),
      pure_rx, impure_rx, 
      identifier_rx, lbracket_rx, rbracket_rx, colon_rx,
      comma_rx, ignored_rx, number_rx, semi_colon_rx) ;  
  
  std::vector<lexem> lexems_filtered ;
  filter_lexems (lexems_instruction, lexems_filtered) ;
    // TODO faire la purité d'une signature de fonction.
  std::cout << 
    cpp::transpile_function_signature(
    next_function_signature (
      lexems_filtered.begin(), 
      lexems_filtered.end()).unit.value()) << std::endl ;
}