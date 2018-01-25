#ifndef __thodd_has_element_hpp__
#  define __thodd_has_element_hpp__

#  include <vector>
#  include <functional>
#  include <type_traits>

# include "structure.hpp"

bool start_with (auto begin, auto end, 
                 auto sbegin, auto send, 
                 auto comparator) {
  while (begin != end && sbegin != send && comparator(*begin, *sbegin)) {
    ++ begin ; ++ sbegin ;   
  }

  return sbegin == send ;
}

bool has_next (auto begin, auto end, std::vector<lexem::type_> const ids) {
  return start_with(
    begin, end, ids.begin(), ids.end(), 
    [] (auto const & lexem, auto const & type) { return lexem.type == type ; }) ;
}

bool has_next (auto begin, auto end, lexem::type_ const id) {
  return has_next (begin, end,  {id}) ;
}

bool has_lbracket (auto begin, auto end) ; // x
auto next_lbracket (auto begin, auto end) ; //
bool has_rbracket (auto begin, auto end) ; // x
auto next_lbracket (auto begin, auto end) ; //
bool has_lbrace (auto begin, auto end) ; // x
auto next_lbracket (auto begin, auto end) ; //
bool has_rbrace (auto begin, auto end) ; // x
auto next_lbracket (auto begin, auto end) ; //
bool has_return_kw (auto begin, auto end) ; // x
auto next_lbracket (auto begin, auto end) ; //
bool has_colon (auto begin, auto end) ; // x
auto next_lbracket (auto begin, auto end) ; //
bool has_comma (auto begin, auto end) ; // x
auto next_lbracket (auto begin, auto end) ; //
bool has_pod_kw (auto begin, auto end) ; // x
auto next_lbracket (auto begin, auto end) ; //
bool has_lambda_kw (auto begin, auto end) ; // x
auto next_lbracket (auto begin, auto end) ; // 
bool has_semi_colon (auto begin, auto end) ; // x
auto next_lbracket (auto begin, auto end) ; //

bool has_string (auto begin, auto end) ; // x
auto next_lbracket (auto begin, auto end) ; //
bool has_identifier (auto begin, auto end) ; // x
auto next_lbracket (auto begin, auto end) ; //
bool has_number (auto begin, auto end) ; // x
auto next_lbracket (auto begin, auto end) ; //

bool has_function_call_expression (auto begin, auto end) ; //
auto next_lbracket (auto begin, auto end) ; //
bool has_lambda_expression (auto begin, auto end) ; //
auto next_lbracket (auto begin, auto end) ; //
bool has_expression (auto begin, auto end) ; // x
auto next_lbracket (auto begin, auto end) ; //

bool has_lbracket (auto begin, auto end) {
  return has_next(begin, end, lexem::type_::lbracket) ;
} 

bool has_rbracket (auto begin, auto end) {
  return has_next(begin, end, lexem::type_::rbracket) ;
} 

bool has_lbrace (auto begin, auto end) {
  return has_next(begin, end, lexem::type_::lbrace) ;
} 

bool has_rbrace (auto begin, auto end) {
  return has_next(begin, end, lexem::type_::rbrace) ;
} 

bool has_return_kw (auto begin, auto end) {
  return has_next(begin, end, lexem::type_::return_kw) ;
}

bool has_lambda_kw (auto begin, auto end) {
  return has_next(begin, end, lexem::type_::lambda_kw) ;
}

bool has_colon (auto begin, auto end) {
  return has_next(begin, end, lexem::type_::colon) ;
}

bool has_comma (auto begin, auto end) {
  return has_next(begin, end, lexem::type_::comma) ;
}

bool has_pod_kw (auto begin, auto end) {
  return has_next(begin, end, lexem::type_::pod_kw) ;
}

bool has_semi_colon (auto begin, auto end) {
  return has_next(begin, end, lexem::type_::semi_colon) ;
}

bool has_string (auto begin, auto end) {
  return has_next(begin, end, lexem::type_::string) ;
}

bool has_identifier (auto begin, auto end) {
  return has_next(begin, end, lexem::type_::identifier) ;
}

bool has_number (auto begin, auto end) {
  return has_next(begin, end, lexem::type_::number) ;
}

bool has_function_call_expression (auto begin, auto end) {
  auto cursor = begin ; 
  if (has_identifier(begin, end) && has_lbracket(std::next(cursor))) cursor = std::next(cursor, 2u) ;
  else return false ;
  
  while (has_expression(cursor, end)) {
    cursor = next_expression(cursor, end) ; // TODO faire next_expression ;
    if (has_comma(cursor)) cursor = std::next(cursor) ;
    else break ;
  }
  
  if (has_rbracket(cursor, end)) cursor = std::next(cursor) ;
  else return false ;

  return true ;

}

bool has_expression (auto begin, auto end) {
  return has_function_call_expression (begin, end) || 
    has_lambda_expression(begin, end) ||
    has_identifier(begin, end) ||
    has_number(begin, end) ;
}

#endif