#ifndef __thodd_signatures_hpp__
#  define __thodd_signatures_hpp__

#  include <tuple>

/// /////////////////// ///
/// couche structure so ///
/// /////////////////// ///


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

bool has_function_call (auto begin, auto end) ; // x t
auto next_function_call (auto begin, auto end) -> decltype(begin) ; // x t

bool has_parameter (auto begin, auto end) ; // x t
auto next_parameter (auto begin, auto end) -> decltype(begin) ; // x t

bool has_lambda (auto begin, auto end) ; // x t
auto next_lambda (auto begin, auto end) -> decltype(begin) ; // x t

bool has_expression (auto begin, auto end) ; // x t
auto next_expression (auto begin, auto end) -> decltype(begin) ; // x t

bool has_const_instruction (auto begin, auto end) ; // x t
auto next_const_instruction (auto begin, auto end) -> decltype(begin) ; // x t

bool has_return_instruction (auto begin, auto end) ; // x t
auto next_return_instruction (auto begin, auto end) -> decltype(begin) ; // x t

bool has_member (auto begin, auto end) ; // x t
auto next_member (auto begin, auto end) -> decltype(begin) ; // x t

bool has_pod (auto begin, auto end) ; // x t
auto next_pod (auto begin, auto end) -> decltype(begin) ; // x t

bool has_function (auto begin, auto end) ; // x t 
auto next_function (auto begin, auto end) -> decltype(begin) ; // x t

bool has_thodd (auto begin, auto end) ; // x t


/// //////////// ///
/// couche objet ///
/// //////////// ///

struct expression ;
struct number ; 
struct identifier ;
struct lambda ;
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


string extract_string (auto begin, auto end) ; // x t
identifier extract_identifier (auto begin, auto end) ; // x t
number extract_number (auto begin, auto end) ; // x t
function_call extract_function_call (auto begin, auto end) ; // x
parameter extract_parameter (auto begin, auto end) ; // x
lambda extract_lambda (auto begin, auto end) ; // x
expression extract_expression (auto begin, auto end) ; // x
const_instruction extract_const_instruction (auto begin, auto end) ; // x
return_instruction extract_return_instruction (auto begin, auto end) ; // x
member extract_member (auto begin, auto end) ; // x
pod extract_pod (auto begin, auto end) ; // x
function extract_function (auto begin, auto end) ; // x
thodd extract_thodd (auto begin, auto end) ; // x


#endif