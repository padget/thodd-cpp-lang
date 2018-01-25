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
bool has_rbracket (auto begin, auto end) ; // x
bool has_lbrace (auto begin, auto end) ; // x
bool has_rbrace (auto begin, auto end) ; // x
bool has_return_kw (auto begin, auto end) ; // x
bool has_colon (auto begin, auto end) ; // x
bool has_comma (auto begin, auto end) ; // x
bool has_pod_kw (auto begin, auto end) ; // x
bool has_lambda_kw (auto begin, auto end) ; // x 
bool has_semi_colon (auto begin, auto end) ; // x

bool has_string (auto begin, auto end) ; // x
bool has_identifier (auto begin, auto end) ; // x
bool has_number (auto begin, auto end) ; // x

bool has_function_call (auto begin, auto end) ; //
bool has_lambda (auto begin, auto end) ; //
bool has_expression (auto begin, auto end) ; //

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



#endif