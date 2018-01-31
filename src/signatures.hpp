#ifndef __thodd_signatures_hpp__
#  define __thodd_signatures_hpp__

#  include <tuple>
#  include "structure.hpp"

/// //////////// ///
/// couche regex ///
/// //////////// ///


auto is_pure_rx (auto begin, auto end) -> std::tuple<decltype(begin), lexem::type_> ; // x t
auto is_impure_rx (auto begin, auto end) -> std::tuple<decltype(begin), lexem::type_> ; // x t
auto is_lambda_rx (auto begin, auto end) -> std::tuple<decltype(begin), lexem::type_> ; // x t
auto is_pod_rx (auto begin, auto end) -> std::tuple<decltype(begin), lexem::type_> ; // x t
auto is_return_rx (auto begin, auto end) -> std::tuple<decltype(begin), lexem::type_> ; // x t
auto is_identifier_rx (auto begin, auto end) -> std::tuple<decltype(begin), lexem::type_> ; // x t
auto is_identifiers_rx (auto begin, auto end) -> std::tuple<decltype(begin), lexem::type_> ; // x 
auto is_lbracket_rx (auto begin, auto end) -> std::tuple<decltype(begin), lexem::type_> ; // x t
auto is_rbracket_rx (auto begin, auto end) -> std::tuple<decltype(begin), lexem::type_> ; // x t
auto is_lbrace_rx (auto begin, auto end) -> std::tuple<decltype(begin), lexem::type_> ; // x t
auto is_rbrace_rx (auto begin, auto end) -> std::tuple<decltype(begin), lexem::type_> ; // x t
auto is_colon_rx (auto begin, auto end) -> std::tuple<decltype(begin), lexem::type_> ; // x t
auto is_semi_colon_rx (auto begin, auto end) -> std::tuple<decltype(begin), lexem::type_> ; // x t
auto is_comma_rx (auto begin, auto end) -> std::tuple<decltype(begin), lexem::type_> ; // x t
auto is_alias_rx (auto begin, auto end) -> std::tuple<decltype(begin), lexem::type_> ; // x t
auto is_strengh_rx (auto begin, auto end) -> std::tuple<decltype(begin), lexem::type_> ; // x t
auto is_weak_rx (auto begin, auto end) -> std::tuple<decltype(begin), lexem::type_> ; // x t
auto is_number_rx (auto begin, auto end) -> std::tuple<decltype(begin), lexem::type_> ; // x t
auto is_ignored_rx (auto begin, auto end) -> std::tuple<decltype(begin), lexem::type_> ; // x t


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
auto next_expression (auto begin, auto end) -> decltype(begin) ; // x

bool has_const_instruction (auto begin, auto end) ; // x t
auto next_const_instruction (auto begin, auto end) -> decltype(begin) ; // x t

bool has_return_instruction (auto begin, auto end) ; // x t
auto next_return_instruction (auto begin, auto end) -> decltype(begin) ; // x t

bool has_pod_member (auto begin, auto end) ; // x t
auto next_pod_member (auto begin, auto end) -> decltype(begin) ; // x t

bool has_pod_declaration (auto begin, auto end) ; // x t
auto next_pod_declaration (auto begin, auto end) -> decltype(begin) ; // x t

bool has_function_declaration (auto begin, auto end) ; // x
auto next_function_declaration (auto begin, auto end) -> decltype(begin) ; // x

bool has_thodd (auto begin, auto end) ; // x

#endif