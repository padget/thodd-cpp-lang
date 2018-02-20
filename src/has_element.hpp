#ifndef __thodd_has_element_hpp__
#  define __thodd_has_element_hpp__

#  include <vector>
#  include <functional>
#  include <type_traits>

#  include "start_with.hpp"
#  include "structure.hpp"
#  include "signatures.hpp"



bool has_next (auto begin, auto end, std::vector<lexem::type_> ids) {
  return start_with(
    begin, end, ids.begin(), ids.end(), 
    [] (auto const & lexem, auto const & type) { return lexem.type == type ; }) ;
}

bool has_lbracket (auto begin, auto end) {
  return has_next(begin, end, {lexem::type_::lbracket}) ;
} 

auto next_lbracket (auto begin, auto end) -> decltype(begin) {
  return std::next(begin) ;
}

bool has_rbracket (auto begin, auto end) {
  return has_next(begin, end, {lexem::type_::rbracket}) ;
} 

auto next_rbracket (auto begin, auto end) -> decltype(begin) {
  return std::next(begin) ;
}

bool has_lbrace (auto begin, auto end) {
  return has_next(begin, end, {lexem::type_::lbrace}) ;
} 

auto next_lbrace (auto begin, auto end) -> decltype(begin) {
  return std::next(begin) ;
}

bool has_rbrace (auto begin, auto end) {
  return has_next(begin, end, {lexem::type_::rbrace}) ;
} 

auto next_rbrace (auto begin, auto end) -> decltype(begin) {
  return std::next(begin) ;
}

bool has_return_kw (auto begin, auto end) {
  return has_next(begin, end, {lexem::type_::return_kw}) ;
}

auto next_return_kw (auto begin, auto end) -> decltype(begin) {
  return std::next(begin) ;
}

bool has_colon (auto begin, auto end) {
  return has_next(begin, end, {lexem::type_::colon}) ;
}

auto next_colon (auto begin, auto end) -> decltype(begin) {
  return std::next(begin) ;
}

bool has_point (auto begin, auto end) {
  return has_next(begin, end, {lexem::type_::point}) ;
}

auto next_point (auto begin, auto end) -> decltype(begin) {
  return std::next(begin) ;
}

bool has_comma (auto begin, auto end) {
  return has_next(begin, end, {lexem::type_::comma}) ;
}

auto next_comma (auto begin, auto end) -> decltype(begin) {
  return std::next(begin) ;
}

bool has_pod_kw (auto begin, auto end) {
  return has_next(begin, end, {lexem::type_::pod_kw}) ;
}

auto next_pod_kw (auto begin, auto end) -> decltype(begin) {
  return std::next(begin) ;
}

bool has_semi_colon (auto begin, auto end) {
  return has_next(begin, end, {lexem::type_::semi_colon}) ;
}

auto next_semi_colon (auto begin, auto end) -> decltype(begin) {
  return std::next(begin) ;
}

bool has_string (auto begin, auto end) {
  return has_next(begin, end, {lexem::type_::string}) ;
}

auto next_string (auto begin, auto end) -> decltype(begin) {
  return std::next(begin) ;
}

bool has_identifier (auto begin, auto end) {
  return has_next(begin, end, {lexem::type_::identifier}) ;
}

auto next_identifier (auto begin, auto end) -> decltype(begin) {
  return std::next(begin) ;
}

bool has_access (auto begin, auto end) {
  auto cursor = begin ;
  bool has_point_again = false ;
  size_t nb_members = 0u ;

  while (has_identifier(cursor, end)) {
    cursor = next_identifier(cursor, end) ;
    ++ nb_members ;

    if (has_point_again = has_point(cursor, end)) 
      cursor = next_point(cursor, end) ;
    else
      break ;
  }

  return nb_members >= 1 && !has_point_again && cursor != begin ;
}

auto next_access (auto begin, auto end) -> decltype(begin) {
  auto cursor = begin ;

  while (has_identifier(cursor, end)) {
    cursor = next_identifier(cursor, end) ;
    
    if (has_point(cursor, end)) 
      cursor = next_point(cursor, end) ;
    else break ;
  }

  return cursor ;
}

bool has_number (auto begin, auto end) {
  return has_next(begin, end, {lexem::type_::number}) ;
}

auto next_number (auto begin, auto end) -> decltype(begin) {
  return std::next(begin) ;
}

bool has_function_call (auto begin, auto end) {
  auto cursor = begin ; 
  
  if (has_identifier(cursor, end) && 
      has_lbracket(next_identifier(cursor, end), end)) 
    cursor = next_lbracket(next_identifier(cursor, end), end) ;
  else 
    return false ;

  bool has_comma_again = false ;

  while (has_expression(cursor, end)) {
    cursor = next_expression(cursor, end) ;
   
    if (has_comma_again = has_comma(cursor, end)) 
      cursor = next_comma(cursor, end) ;
    else
      break ;
  }

  return !has_comma_again && has_rbracket(cursor, end) ;
}

auto next_function_call (auto begin, auto end) -> decltype(begin) {
  auto cursor = begin ; 
  cursor = next_lbracket(next_identifier(cursor, end), end) ;

  while (has_expression(cursor, end))
    if (has_comma(cursor = next_expression(cursor, end), end)) 
      cursor = next_comma(cursor, end) ;
    else 
      break ;

  return next_rbracket(cursor, end) ;
}

bool has_parameter (auto begin, auto end) {
  return has_identifier(begin, end) && 
    has_colon(next_identifier(begin, end), end) &&
    has_identifier(next_colon(next_identifier(begin, end), end), end) ;
}

auto next_parameter (auto begin, auto end) -> decltype(begin) {
  return next_identifier(next_colon(next_identifier(begin, end), end), end) ;
} 

bool has_expression (auto begin, auto end) {
  return has_function_call (begin, end) || 
    has_access(begin, end) ||
    has_identifier(begin, end) ||
    has_number(begin, end) ;
}

auto next_expression (auto begin, auto end) -> decltype(begin) {
  if (has_function_call(begin, end)) return next_function_call(begin, end) ;
  if (has_access(begin, end)) return next_access(begin, end) ;
  if (has_identifier(begin, end)) return next_identifier(begin, end) ;
  if (has_number(begin, end)) return next_number(begin, end) ;
  return begin ;
}

bool has_const_instruction (auto begin, auto end) {
  return has_identifier(begin, end) &&
    has_colon(next_identifier(begin, end), end) &&
    has_identifier(next_colon(next_identifier(begin, end), end), end) && 
    has_expression(next_identifier(next_colon(next_identifier(begin, end), end), end), end) && 
    has_semi_colon(next_expression(next_identifier(next_colon(next_identifier(begin, end), end), end), end), end) ;
}

auto next_const_instruction (auto begin, auto end) -> decltype(begin) {
  return next_semi_colon(next_expression(next_identifier(next_colon(next_identifier(begin, end), end), end), end), end) ; 
}

bool has_return_instruction (auto begin, auto end) {
  return has_return_kw(begin, end) &&
    has_expression(next_return_kw(begin, end), end) &&
    has_semi_colon(next_expression(next_return_kw(begin, end), end), end) ;
}

auto next_return_instruction (auto begin, auto end) -> decltype(begin) {
  return next_semi_colon(next_expression(next_return_kw(begin, end), end), end) ; 
}

bool has_member (auto begin, auto end) {
  return has_identifier(begin, end) && 
    has_colon(next_identifier(begin, end), end) && 
    has_identifier(next_colon(next_identifier(begin, end), end), end) && 
    has_semi_colon(next_identifier(next_colon(next_identifier(begin, end), end), end), end) ;
}

auto next_member (auto begin, auto end) -> decltype(begin) {
  return next_semi_colon(next_identifier(next_colon(next_identifier(begin, end), end), end), end) ;
}

bool has_pod (auto begin, auto end) {
  auto cursor = begin ; 
  
  if (!has_pod_kw(cursor, end))
    return false ;

  cursor = next_pod_kw(cursor, end) ;

  if (!has_identifier(cursor, end))
    return false ;

  cursor = next_identifier(cursor, end) ;

  if (!has_lbrace(cursor, end))
    return false ;

  cursor = next_lbrace(cursor, end) ;

  while (has_member(cursor, end)) 
    cursor = next_member(cursor, end) ;

  return has_rbrace(cursor, end) ;
}

auto next_pod (auto begin, auto end) -> decltype(begin) {
  auto cursor = next_lbrace(next_identifier(next_pod_kw(begin, end), end), end) ;
  
  while (has_member(cursor, end))
    cursor = next_member(cursor, end) ;

  return next_rbrace(cursor, end) ;
}

bool has_function (auto begin, auto end) {
  auto cursor = begin ;

  if (!has_identifier(cursor, end)) 
    return false ;
  
  cursor = next_identifier(cursor, end) ;
  
  if (!has_lbracket(cursor, end))
    return false ;

  cursor = next_lbracket(cursor, end) ;
  bool has_comma_again = false ;

  while (has_parameter(cursor, end)) {
    cursor = next_parameter(cursor, end) ;

    if (has_comma_again = has_comma(cursor, end))
      cursor = next_comma(cursor, end) ;
    else break ;
  }

  if (has_comma_again) 
    return false ;

  if (!has_rbracket(cursor, end))
    return false ;
  
  cursor = next_rbracket(cursor, end) ;
  
  if (!has_colon(cursor, end) || !has_identifier(next_colon(cursor, end), end))
    return false ;

  cursor = next_identifier(next_colon(cursor, end), end) ;  

  if (!has_lbrace(cursor, end))
    return false ;

  cursor = next_lbrace(cursor, end) ;
  
  while (has_const_instruction(cursor, end))
    cursor = next_const_instruction(cursor, end) ;

  if (!has_return_instruction(cursor, end)) 
    return false ; 

  cursor = next_return_instruction(cursor, end) ;

  if (!has_rbrace(cursor, end))
    return false ;
  
  return true ;
}

auto next_function (auto begin, auto end) -> decltype(begin) {
  auto cursor = begin ;
  cursor = next_lbracket(next_identifier(cursor, end), end) ;
  
  while (has_parameter(cursor, end))
    if (!has_comma(cursor = next_parameter(cursor, end), end)) break ;
    else cursor = next_comma(cursor, end) ;

  cursor = next_lbrace(next_identifier(next_colon(next_rbracket(cursor, end), end), end), end) ;

  while (has_const_instruction(cursor, end))
    cursor = next_const_instruction(cursor, end) ;

  cursor = next_return_instruction(cursor, end) ;

  return next_rbrace(cursor, end) ;
}

bool has_thodd (auto begin, auto end) {
  auto cursor = begin ;
  bool has_declaration = true ;
  bool has_declaration_malformed = false ;

  while (has_declaration && !has_declaration_malformed)
    if (has_pod(cursor, end))
      cursor = next_pod(cursor, end) ;
    else if (has_function(cursor, end))
      cursor = next_function(cursor, end) ;
    else 
      has_declaration = false ;

  return cursor == end ;
}





#endif