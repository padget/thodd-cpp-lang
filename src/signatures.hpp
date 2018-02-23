#ifndef __thodd_signatures_hpp__
#  define __thodd_signatures_hpp__

/// /////////////////// ///
/// couche structure so ///
/// /////////////////// ///

struct lexem {
  enum class type_ : int {
    identifier, 
    number, string, 
    lbrace, rbrace, 
    lbracket, rbracket,
    lsbracket, rsbracket,
    colon, semi_colon, 
    point, comma, ignored, 
    pure_kw, impure_kw, pod_kw, error,
    weak, strengh, alias, 
    switch_kw, case_kw, return_kw, 
    iterate_kw, iterate_if_kw
  } ;

  type_ type ;
  std::string data ;
} ;


/// //////////// ///
/// couche regex ///
/// //////////// ///


auto search_for_pure_rx (auto begin, auto end) -> std::tuple<decltype(begin), lexem::type_> ; // x t
auto search_for_impure_rx (auto begin, auto end) -> std::tuple<decltype(begin), lexem::type_> ; // x t
auto search_for_pod_rx (auto begin, auto end) -> std::tuple<decltype(begin), lexem::type_> ; // x t
auto search_for_return_rx (auto begin, auto end) -> std::tuple<decltype(begin), lexem::type_> ; // x t
auto search_for_identifier_rx (auto begin, auto end) -> std::tuple<decltype(begin), lexem::type_> ; // x t
auto search_for_lbracket_rx (auto begin, auto end) -> std::tuple<decltype(begin), lexem::type_> ; // x t
auto search_for_rbracket_rx (auto begin, auto end) -> std::tuple<decltype(begin), lexem::type_> ; // x t
auto search_for_lbrace_rx (auto begin, auto end) -> std::tuple<decltype(begin), lexem::type_> ; // x t
auto search_for_rbrace_rx (auto begin, auto end) -> std::tuple<decltype(begin), lexem::type_> ; // x t
auto search_for_point_rx (auto begin, auto end) -> std::tuple<decltype(begin), lexem::type_> ; // x 
auto search_for_colon_rx (auto begin, auto end) -> std::tuple<decltype(begin), lexem::type_> ; // x t
auto search_for_semi_colon_rx (auto begin, auto end) -> std::tuple<decltype(begin), lexem::type_> ; // x t
auto search_for_comma_rx (auto begin, auto end) -> std::tuple<decltype(begin), lexem::type_> ; // x t
auto search_for_alias_rx (auto begin, auto end) -> std::tuple<decltype(begin), lexem::type_> ; // x t
auto search_for_strengh_rx (auto begin, auto end) -> std::tuple<decltype(begin), lexem::type_> ; // x t
auto search_for_weak_rx (auto begin, auto end) -> std::tuple<decltype(begin), lexem::type_> ; // x t
auto search_for_number_rx (auto begin, auto end) -> std::tuple<decltype(begin), lexem::type_> ; // x t
auto search_for_ignored_rx (auto begin, auto end) -> std::tuple<decltype(begin), lexem::type_> ; // x t


/// //////////// ///
/// couche lexem ///
/// //////////// ///





/// //////////// ///
/// couche objet ///
/// //////////// ///

struct expression ;
struct number ; 
struct identifier ;
struct string ;
struct function_call ;
struct parameter ;
struct pod ;
struct member ;
struct const_instruction ;
struct return_instruction ;
struct function ;
struct declaration ;
struct thodd ;





#endif