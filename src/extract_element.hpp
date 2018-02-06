#ifndef __thodd_extract_element_hpp__
#  define __thodd_extract_element_hpp__

#  include "structure.hpp"
#  include "has_element.hpp"

string extract_string (auto begin, auto end) {
  return string{(*begin).data} ;
}

identifier extract_identifier (auto begin, auto end) {
  return identifier{(*begin).data} ;
}

number extract_number (auto begin, auto end) {
  return number{(*begin).data} ;
}

function_call extract_function_call (auto begin, auto end) {
  auto cursor = begin ; 
  auto && name = extract_identifier(cursor, end) ;
  cursor = next_lbracket(next_identifier(cursor, end), end) ;
  std::vector<expression> args ;
  
  while (has_expression(cursor, end)) {
    args.push_back(extract_expression(cursor, end)) ;
    cursor = next_expression(cursor, end) ;
    
    if (has_comma(cursor, end))
      cursor = next_comma(cursor, end) ;
    else break ;
  }
  
  return function_call{name, args} ;
}

parameter extract_parameter (auto begin, auto end) {
  return parameter{
    extract_identifier(begin, end), 
    extract_identifier(next_colon(next_identifier(begin, end), end), end)} ; 
}

lambda extract_lambda (auto begin, auto end) {
  auto cursor = begin ;
  cursor = next_lbracket(next_lambda_kw(cursor, end), end) ;
  std::vector<parameter> parameters ;

  while (has_parameter(cursor, end)) {
    parameters.push_back(extract_parameter(cursor, end)) ;
    cursor = next_parameter(cursor, end) ;

    if (has_comma(cursor, end)) 
      cursor = next_comma(cursor, end) ;
    else break ;
  }

  cursor = next_colon(next_rbracket(cursor, end), end) ;

  auto && type = extract_identifier(cursor, end) ;
  cursor = next_identifier(cursor, end) ;
  cursor = next_lbrace(cursor, end) ;

  std::vector<const_instruction> consts ;

  while (has_const_instruction(cursor, end)) {
    consts.push_back(extract_const_instruction(cursor, end)) ;
    cursor = next_const_instruction(cursor, end) ;
  }

  auto && return_ = extract_return_instruction(cursor, end) ;
  
  return  // TODO generate unique name 
  lambda{identifier{}, type, parameters, consts, return_} ;
}

expression extract_expression (auto begin, auto end) {
  if (has_function_call(begin, end))
    return expression{expression::type_::function_call, std::vector<lexem>(begin, next_function_call(begin, end))} ;
  else if (has_lambda(begin, end))
    return expression{expression::type_::lambda, std::vector<lexem>(begin, next_lambda(begin, end))} ;
  else if (has_number(begin, end))
    return expression{expression::type_::number, std::vector<lexem>(begin, next_number(begin, end))} ;
  else if (has_identifier(begin, end))
    return expression{expression::type_::lambda, std::vector<lexem>(begin, next_identifier(begin, end))} ;
  else if (has_string(begin, end))
    return expression{expression::type_::lambda, std::vector<lexem>(begin, next_string(begin, end))} ;
  return expression{expression::type_::unknown} ;
}

const_instruction extract_const_instruction (auto begin, auto end) {
  auto && name  = extract_identifier(begin, end) ;
  auto && type  = extract_identifier(next_colon(next_identifier(begin, end), end), end) ;
  auto && value = extract_expression(next_identifier(next_colon(next_identifier(begin, end), end), end), end) ;
  return const_instruction{name, type, value} ; 
}

return_instruction extract_return_instruction (auto begin, auto end) {
  return return_instruction{extract_expression(next_return_kw(begin, end), end)} ;
}

member extract_member (auto begin, auto end) {
  auto && name = extract_identifier(begin, end) ;
  auto && type = extract_identifier(next_colon(next_identifier(begin, end), end), end) ;
  return member{name, type} ; 
}

pod extract_pod (auto begin, auto end) {
  auto cursor = next_pod_kw(begin, end) ;
  auto && name = extract_identifier(cursor, end) ;
  cursor = next_lbrace(next_identifier(cursor, end), end) ;
  std::vector<member> members ;
  
  while (has_member(cursor, end)) {
    members.push_back(extract_member(cursor, end)) ;
    cursor = next_member(cursor, end) ;
  }

  return pod{name, members} ;
}

function extract_function (auto begin, auto end) {
  auto cursor = begin ;
  auto && name = extract_identifier(cursor, end) ;
  cursor = next_lbracket(next_identifier(cursor, end), end) ;
  std::vector<parameter> parameters ;

  while (has_parameter(cursor, end)) {
    parameters.push_back(extract_parameter(cursor, end)) ;
    cursor = next_parameter(cursor, end) ;

    if (has_comma(cursor, end)) 
      cursor = next_comma(cursor, end) ;
    else break ;
  }

  cursor = next_colon(next_rbracket(cursor, end), end) ;

  auto && type = extract_identifier(cursor, end) ;
  cursor = next_identifier(cursor, end) ;
  cursor = next_lbrace(cursor, end) ;

  std::vector<const_instruction> consts ;

  while (has_const_instruction(cursor, end)) {
    consts.push_back(extract_const_instruction(cursor, end)) ;
    cursor = next_const_instruction(cursor, end) ;
  }

  auto && return_ = extract_return_instruction(cursor, end) ;
  
  return 
  function{name, type, parameters, consts, return_} ;
}

thodd extract_thodd (std::string_view filename, auto begin, auto end) {
  std::vector<pod> pods ;
  std::vector<function> functions ;
  auto cursor = begin ;
  bool has_something = true ;

  while (has_something) {
    if (has_pod(cursor, end)) {
      pods.push_back(extract_pod(cursor, end)) ;
      cursor = next_pod(cursor, end) ;
    } else if (has_function(cursor, end)) {
      functions.push_back(cursor, end) ;
      cursor = next_function(cursor, end) ;
    } else has_something = false ; 
  }
  
  return thodd{filename, pods, functions} ;
}


#endif 