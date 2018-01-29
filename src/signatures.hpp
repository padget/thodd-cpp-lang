#ifndef __thodd_signatures_hpp__
#  define __thodd_signatures_hpp__

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

bool has_pod_member (auto begin, auto end) ; // x
auto next_pod_member (auto begin, auto end) -> decltype(begin) ; // x

bool has_pod_declaration (auto begin, auto end) ; // x
auto next_pod_declaration (auto begin, auto end) -> decltype(begin) ; // x

bool has_function_declaration (auto begin, auto end) ;
auto next_function_declaration (auto begin, auto end) -> decltype(begin) ;

bool has_thodd (auto begin, auto end) ; //
auto next_thodd (auto begin, auto end) -> decltype(begin) ; // 

#endif