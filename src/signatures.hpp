#ifndef __thodd_signatures_hpp__
#  define __thodd_signatures_hpp__

#  include <tuple>

/// //////////////// ///
/// couche structure ///
/// //////////////// ///


struct lexem {
  enum class type_ : int {
    identifier, identifiers, 
    number, string, 
    lbrace, rbrace, 
    lbracket, rbracket,
    lsbracket, rsbracket,
    colon, semi_colon, 
    point, comma, ignored, 
    pure_kw, impure_kw, pod_kw,
    lambda_kw, error,
    weak, strengh, alias, 
    switch_kw, case_kw, return_kw, 
    iterate_kw, iterate_if_kw
  } ;

  type_ type ;
  std::string data ;
} ;

struct expression ;
struct number ; 
struct identifier ;
struct lambda_expression ;
struct string ;
struct function_call ;
struct parameter ;
struct pod_declaration ;
struct pod_member ;
struct const_instruction ;
struct return_instruction ;
struct function_declaration ;
struct declaration ;
struct thodd ;


/// //////////// ///
/// couche regex ///
/// //////////// ///


auto search_for_pure_rx (auto begin, auto end) -> std::tuple<decltype(begin), lexem::type_> ; // x t
auto search_for_impure_rx (auto begin, auto end) -> std::tuple<decltype(begin), lexem::type_> ; // x t
auto search_for_lambda_rx (auto begin, auto end) -> std::tuple<decltype(begin), lexem::type_> ; // x t
auto search_for_pod_rx (auto begin, auto end) -> std::tuple<decltype(begin), lexem::type_> ; // x t
auto search_for_return_rx (auto begin, auto end) -> std::tuple<decltype(begin), lexem::type_> ; // x t
auto search_for_identifier_rx (auto begin, auto end) -> std::tuple<decltype(begin), lexem::type_> ; // x t
auto search_for_identifiers_rx (auto begin, auto end) -> std::tuple<decltype(begin), lexem::type_> ; // x 
auto search_for_lbracket_rx (auto begin, auto end) -> std::tuple<decltype(begin), lexem::type_> ; // x t
auto search_for_rbracket_rx (auto begin, auto end) -> std::tuple<decltype(begin), lexem::type_> ; // x t
auto search_for_lbrace_rx (auto begin, auto end) -> std::tuple<decltype(begin), lexem::type_> ; // x t
auto search_for_rbrace_rx (auto begin, auto end) -> std::tuple<decltype(begin), lexem::type_> ; // x t
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


bool has_lbracket (auto begin, auto end) ; // x t
auto next_lbracket (auto begin, auto end) -> decltype(begin) ; // x t

bool has_rbracket (auto begin, auto end) ; // x t
auto next_rbracket (auto begin, auto end) -> decltype(begin) ; // x t

bool has_lbrace (auto begin, auto end) ; // x t
auto next_lbrace (auto begin, auto end) -> decltype(begin) ; // x t

bool has_rbrace (auto begin, auto end) ; // x t
auto next_rbrace (auto begin, auto end) -> decltype(begin) ; // x t

bool has_return_kw (auto begin, auto end) ; // x t
auto next_return_kw (auto begin, auto end) -> decltype(begin) ; // x t

bool has_colon (auto begin, auto end) ; // x t
auto next_colon (auto begin, auto end) -> decltype(begin) ; // x t

bool has_comma (auto begin, auto end) ; // x t
auto next_comma (auto begin, auto end) -> decltype(begin) ; // x t

bool has_pod_kw (auto begin, auto end) ; // x t
auto next_pod_kw (auto begin, auto end) -> decltype(begin) ; // x t

bool has_lambda_kw (auto begin, auto end) ; // x t
auto next_lambda_kw (auto begin, auto end) -> decltype(begin) ; // x t 

bool has_semi_colon (auto begin, auto end) ; // x t
auto next_semi_colon (auto begin, auto end) -> decltype(begin) ; // x t

bool has_string (auto begin, auto end) ; // x t
auto next_string (auto begin, auto end) -> decltype(begin) ; // x t

bool has_identifier (auto begin, auto end) ; // x t
auto next_identifier (auto begin, auto end) -> decltype(begin) ; // x t

bool has_number (auto begin, auto end) ; // x t
auto next_number (auto begin, auto end) -> decltype(begin) ; // x t

bool has_function_call_expression (auto begin, auto end) ; // x t
auto next_function_call_expression (auto begin, auto end) -> decltype(begin) ; // x t

bool has_parameter (auto begin, auto end) ; // x t
auto next_parameter (auto begin, auto end) -> decltype(begin) ; // x t

bool has_lambda_expression (auto begin, auto end) ; // x t
auto next_lambda_expression (auto begin, auto end) -> decltype(begin) ; // x t

bool has_expression (auto begin, auto end) ; // x t
auto next_expression (auto begin, auto end) -> decltype(begin) ; // x t

bool has_const_instruction (auto begin, auto end) ; // x t
auto next_const_instruction (auto begin, auto end) -> decltype(begin) ; // x t

bool has_return_instruction (auto begin, auto end) ; // x t
auto next_return_instruction (auto begin, auto end) -> decltype(begin) ; // x t

bool has_pod_member (auto begin, auto end) ; // x t
auto next_pod_member (auto begin, auto end) -> decltype(begin) ; // x t

bool has_pod_declaration (auto begin, auto end) ; // x t
auto next_pod_declaration (auto begin, auto end) -> decltype(begin) ; // x t

bool has_function_declaration (auto begin, auto end) ; // x t 
auto next_function_declaration (auto begin, auto end) -> decltype(begin) ; // x t

bool has_thodd (auto begin, auto end) ; // x t


/// //////////// ///
/// couche objet ///
/// //////////// ///


string extract_string (auto begin, auto end) ; // x
identifier extract_identifier (auto begin, auto end) ;// x
number extract_number (auto begin, auto end) ;
function_call extract_function_call_expression (auto begin, auto end) ;
parameter extract_parameter (auto begin, auto end) ;
lambda_expression extract_lambda_expression (auto begin, auto end) ;
expression extract_expression (auto begin, auto end) ;
const_instruction extract_const_instruction (auto begin, auto end) ;
return_instruction extract_return_instruction (auto begin, auto end) ;
pod_member extract_pod_member (auto begin, auto end) ;
pod_declaration extract_pod_declaration (auto begin, auto end) ;
function_declaration extract_function_declaration (auto begin, auto end) ;
thodd extract_thodd (auto begin, auto end) ;

#endif