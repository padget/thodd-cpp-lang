#ifndef __thodd_extract_element_hpp__
#  define __thodd_extract_element_hpp__

#  include <vector>
#  include <functional>
#  include <type_traits>

bool
start_with (auto begin, auto const end, auto sbegin, auto send, auto comparator) {
  while (begin != end && sbegin != send 
          && comparator(*begin, *sbegin)) {
    ++ begin ; ++ sbegin ;   
  }

  return sbegin == send ;
}

template <
  typename extract_t>
struct extracted {
  using lexems_it = typename std::vector<lexem>::iterator ;

  extract_t ext ;
  lexems_it last ;
} ;

auto make_extracted (auto last, auto const & ext) {
  return extracted <std::decay_t<decltype(ext)>> {ext, last} ;
}

/**
 * Fonction de test générique
 */ 
bool has_next (auto begin, auto end, std::vector<lexem::type_> const ids) {
  return start_with(
    begin, end, ids.begin(), ids.end(), 
    [] (auto const & lexem, auto const & type) { return lexem.type == type ; }) ;
}

/** 
 * Identifier
 */

bool has_identifier (auto begin, auto end) {
  return has_next(begin, end, {lexem::type_::identifier}) ;
}

auto next_identifier(auto begin, auto end) {
  return std::next(begin) ;
}

extracted<identifier> 
extract_identifier (auto begin, auto end) {
  return make_extracted(next_identifier(begin, end), identifier{(*begin).data}) ; 
}

/**
 * Number
 */
bool has_number (auto begin, auto end) {
  return has_next(begin, end, {lexem::type_::number}) ;
}

auto next_number(auto begin, auto end) {
  return std::next(begin) ;
}

extracted<number>
extract_number (auto begin, auto end) {
  return make_extracted(next_number(begin, end), number{(*begin).data}) ;
}

/**
 * expression
 */

bool has_function_call (auto begin, auto end) ;
auto next_function_call (auto begin, auto end) -> decltype(begin) ;
extracted<function_call> extract_function_call (auto begin, auto end) ;

bool has_expression (auto begin, auto end) {
  return has_function_call (begin, end) ||
    has_number(begin, end) ||
    has_identifier(begin, end) ;
}

auto next_expression (auto begin, auto end) {
  auto cursor = begin ;

  if (has_function_call(begin, end)) {
    cursor = next_function_call(begin, end) ;
  } else if (has_identifier(begin, end))
    cursor = next_identifier(begin, end) ;
  else if (has_number(begin, end)) 
    cursor = next_number(begin, end) ;

  return cursor ;
}
 
extracted<expression> 
extract_expression (auto begin, auto const end) {
  expression::type_ type ;

  if (has_function_call(begin, end))
    type = expression::type_::function_call ;
  else if (has_number(begin, end))
    type = expression::type_::number ;
  else if (has_identifier(begin, end))
    type = expression::type_::identifier ;
  
  auto const last = next_expression(begin, end) ;

  return make_extracted(last, expression{type, copy_lexems(begin, last)}) ;
}

/**
 * Function call
 */

bool has_function_call (auto begin, auto const end) {
  auto cursor = begin ;
  
  if (!has_identifier(cursor, end))
    return false ;

  cursor = std::next(cursor) ;

  if (!has_next(cursor, end, {lexem::type_::lbracket}))
    return false ;

  cursor = std::next(cursor) ;

  while (has_expression(cursor, end)) {
    cursor = next_expression(cursor, end) ;
    
    if (has_next(cursor, end, {lexem::type_::comma})) 
      cursor = std::next(cursor) ;
    else 
      break ;
  }
    
  if (!has_next(cursor, end, {lexem::type_::rbracket}))
    return false ;

  return true ;
}

auto next_function_call (auto begin, auto end) -> decltype(begin) {
  auto cursor = std::next(next_identifier(begin, end)) ;

  while (has_expression(cursor, end)) {
    cursor = next_expression(cursor, end) ;
    
    if (has_next(cursor, end, {lexem::type_::comma})) 
      cursor = std::next(cursor) ;
    else 
      break ;
  }

  return std::next(cursor) ;
}

extracted<function_call> 
extract_function_call (auto begin, auto end) {
  auto && fname_ext = extract_identifier(begin, end) ;
  auto cursor = fname_ext.last ;
  std::vector<expression> expressions ;

  while (has_expression(cursor, end)) {
    auto && expr_ext = extract_expression(cursor, end) ;
    expressions.push_back(expr_ext.ext) ;
    cursor = expr_ext.last ;
  }
    
  return make_extracted(
    std::next(cursor, 2u), 
    function_call{fname_ext.ext, expressions}) ;  
}

/**
 * If statement
 */

bool has_instruction (auto begin, auto end) ;
auto next_instruction (auto begin, auto end) -> decltype(begin) ;
extracted<instruction> extract_instruction (auto begin, auto end) ;

bool has_if_statement (auto begin, auto end) {
  auto cursor = begin ;

  if (!has_next(begin, end, {lexem::type_::if_kw})) return false ;
  cursor = std::next(cursor) ;
  if (!has_next(cursor, end, {lexem::type_::lbracket})) return false ;
  cursor = std::next(cursor) ;
  if (!has_expression(cursor, end)) return false ;
  cursor = next_expression(cursor, end) ;
  if (!has_next(cursor, end, {lexem::type_::rbracket})) return false ;
  cursor = std::next(cursor) ;
  if (!has_next(cursor, end, {lexem::type_::lbrace})) return false ;
  cursor = std::next(cursor) ;
  
  while (has_instruction(cursor, end)) 
    cursor = next_instruction (cursor, end) ;  

  if (!has_next(cursor, end, {lexem::type_::rbrace}))
    return false ;

  return true ;
}

auto next_if_statement (auto begin, auto end) {
  auto cursor = std::next(next_expression(std::next(begin, 2u), end), 2u) ;

  while (has_instruction(cursor, end)) 
    cursor = next_instruction(cursor, end) ;

  return std::next(cursor) ;
}

extracted<if_statement>
extract_if_statement (auto begin, auto end) {
  auto && test_ext = extract_expression(std::next(begin, 2u), end) ;
  auto cursor = test_ext.last ;
  std::vector<instruction> instructions ;

  while (has_instruction(cursor, end)) {
    auto && inst_ext = extract_instruction(cursor, end) ;
    instructions.push_back(inst_ext.ext) ;
    cursor = inst_ext.last ;
  }

  return make_extracted(
    std::next(cursor, 2u), 
    if_statement{test_ext.ext, instructions}) ; 
}

/**
 * While statement
 */

bool has_while_statement (auto begin, auto end) {
  auto cursor = begin ;
  
  if (!has_next(begin, end, {lexem::type_::while_kw})) return false ;
  cursor = std::next(cursor) ;
  if (!has_next(cursor, end, {lexem::type_::lbracket})) return false ;
  cursor = std::next(cursor) ;
  if (!has_expression(cursor, end)) return false ;
  cursor = next_expression(cursor, end) ;
  if (!has_next(cursor, end, {lexem::type_::rbracket})) return false ;
  cursor = std::next(cursor) ;
  if (!has_next(cursor, end, {lexem::type_::lbrace})) return false ;
  cursor = std::next(cursor) ;

  while (has_instruction(cursor, end)) 
    cursor = next_instruction (cursor, end) ;  

  if (!has_next(cursor, end, {lexem::type_::rbrace}))
    return false ;

  return true ;
}

auto next_while_statement (auto begin, auto end) {
  auto cursor = std::next(next_expression(std::next(begin, 2u), end), 2u) ;

  while (has_instruction(cursor, end)) 
    cursor = next_instruction(cursor, end) ;

  return std::next(cursor) ;
}

extracted<while_statement>
extract_while_statement (auto begin, auto end) {
  auto && test_ext = extract_expression(std::next(begin, 2u), end) ;
  auto cursor = test_ext.last ;
  std::vector<instruction> instructions ;

  while (has_instruction(cursor, end)) {
    auto && inst_ext = extract_instruction(cursor, end) ;
    instructions.push_back(inst_ext.ext) ;
    cursor = inst_ext.last ;
  }

  return make_extracted(
    std::next(cursor, 2u), 
    while_statement{test_ext.ext, instructions}) ; 
}

/**
 * Const déclaration 
 */

// struct const_declaration {
//   identifier name ;
//   identifier type ;
//   expression value ;
// } ;
#include <iostream>
bool has_const_declaration (auto begin, auto end) {
  return has_identifier(begin, end) && 
    has_next(std::next(begin), end, {lexem::type_::colon}) &&
    has_identifier(std::next(begin, 2u), end) && 
    has_expression(std::next(begin, 3u), end) && 
    has_next(next_expression(std::next(begin, 3u), end), end, {lexem::type_::semi_colon}) ;
}

auto next_const_declaration (auto begin, auto end) {
  return std::next(next_expression(std::next(begin, 3u), end)) ; 
}

extracted<const_declaration> 
extract_const_declaration (auto begin, auto end) {
  auto && name_ext = extract_identifier(begin, end) ;
  auto && type_ext = extract_identifier(std::next(name_ext.last), end) ;
  auto && expression_ext = extract_expression(type_ext.last, end) ;

  return make_extracted(
    std::next(expression_ext.last), 
    const_declaration{name_ext.ext, type_ext.ext, expression_ext.ext}) ;
}

/**
 * Return statement
 */

// struct return_statement {
//   expression value ;
// } ;

bool has_return_statement (auto begin, auto end) {
  return has_next(begin, end, {lexem::type_::return_kw}) &&
    has_expression(std::next(begin), end) && 
    has_next(next_expression(std::next(begin), end), end, {lexem::type_::semi_colon}) ;
}

auto next_return_statement (auto begin, auto end) {
  return std::next(next_expression(std::next(begin), end)) ;
}

extracted<return_statement> 
extract_return_statement (auto begin, auto end) {
  auto && [value, last] = extract_expression(std::next(begin), end) ;
  return make_extracted(std::next(last), value) ;
}

/**
 * Instruction
 */ 

bool has_instruction (auto begin, auto end) {
  // TODO add for and switch statement ;
  return has_const_declaration(begin, end) ||
    has_if_statement(begin, end) ||
    has_while_statement(begin, end) ||
    has_return_statement(begin, end) ;

}

auto next_instruction (auto begin, auto end) -> decltype(begin) {
  auto cursor = begin ;

  if (has_const_declaration(begin, end))
    cursor =  next_const_declaration(begin, end) ;
  else if (has_if_statement(begin, end))
    cursor = next_if_statement(begin, end) ;
  else if (has_while_statement(begin, end))
    cursor = next_while_statement(begin, end) ;
  else if (has_return_statement(begin, end))
    cursor = next_return_statement(begin, end) ;
  
  return cursor ;
}

extracted<instruction> extract_instruction (auto begin, auto end) {
  instruction::type_ type = 
    has_const_declaration(begin, end) ?
      instruction::type_::const_declaration :
    has_if_statement(begin, end) ?
      instruction::type_::if_statement :
    has_while_statement(begin, end) ?
      instruction::type_::while_statement :
    has_return_statement(begin, end) ?
      instruction::type_::return_statement : 
      instruction::type_::return_statement ;
  auto const last = next_instruction(begin, end) ;
  
  return make_extracted(last, instruction{type, copy_lexems(begin, last)}) ;  
}

/**
 * Function parameter
 */  

bool has_parameter (auto begin, auto end) {
  return has_identifier(begin, end) &&
    has_next(std::next(begin), end, {lexem::type_::colon}) && 
    has_identifier(std::next(begin, 2u), end) ;
}

auto next_parameter (auto begin, auto end) {
  return next_identifier(std::next(next_identifier(begin, end)), end) ;
}

extracted<parameter> 
extract_parameter (auto begin, auto end) {
  auto && name_ext = extract_identifier(begin, end) ;
  auto && type_ext = extract_identifier(std::next(name_ext.last), end) ;
  return make_extracted(type_ext.last, parameter{name_ext.ext, type_ext.ext}) ;
}

/**
 * Function declaration
 */

bool has_function_declaration (auto begin, auto end) {
  auto cursor = begin ;

  if (!has_identifier(cursor, end)) 
    return false ; 

  cursor = next_identifier(cursor, end) ;

  if (!has_next(cursor, end, {lexem::type_::lbracket}))
    return false ;

  cursor = std::next(cursor) ;

  while (has_parameter(cursor, end)) {
    cursor = next_parameter(cursor, end) ;
    
    if (has_next(cursor, end, {lexem::type_::comma}))
      cursor = std::next(cursor) ;
    else 
      break ;
  }
  
  if (!has_next(cursor, end, {lexem::type_::rbracket, lexem::type_::colon}))
    return false ;

  cursor = std::next(cursor, 2u) ;

  if (!has_identifier(cursor, end))
    return false ;

  cursor = next_identifier(cursor, end) ;
  
  if (!has_next(cursor, end, {lexem::type_::lbrace}))
    return false ;

  cursor = std::next(cursor) ;

  while (has_instruction(cursor, end)) 
    cursor = next_instruction(cursor, end) ;
  
  if (!has_next(cursor, end, {lexem::type_::rbrace}))
    return false ;

  return true ;
}

auto next_function_declaration (auto begin, auto end) {
  auto cursor = std::next(next_identifier(begin, end)) ;
  
  while (has_parameter(cursor, end)) {
    cursor = next_parameter(cursor, end) ;
    
    if (has_next(cursor, end, {lexem::type_::comma}))
      cursor = std::next(cursor) ;
    else 
      break ;
  }

  cursor = std::next(next_identifier(std::next(cursor, 2u), end));
  
  while(has_instruction(cursor, end))
    cursor = next_instruction(cursor, end) ;

  return std::next(cursor) ;
}

extracted<function_declaration>
extract_function_declaration (auto begin, auto end) {
  auto && name_ext = extract_identifier(begin, end) ;
  auto cursor = std::next(name_ext.last) ;
  std::vector<parameter> parameters ;

  while (has_parameter(cursor, end)) {
    auto && param_ext = extract_parameter(cursor, end) ;
    parameters.push_back(param_ext.ext) ;

    if (has_next(param_ext.last, end, {lexem::type_::comma}))
      cursor = std::next(param_ext.last) ;
    else 
      break ;
  }
  
  cursor = std::next(cursor, 2u) ;
  auto && type_ext = extract_identifier(cursor, end) ; 
  cursor = std::next(cursor) ;
  std::vector<instruction> instructions ;

  while (has_instruction(cursor, end)) {
    auto && inst_ext = extract_instruction(cursor, end) ;
    cursor = inst_ext.last ;
    instructions.push_back(inst_ext.ext) ;
  }
  
  return make_extracted(
    std::next(cursor), 
    function_declaration{name_ext.ext, type_ext.ext, parameters, instructions}) ;
}

/**
 * Pod member
 */

bool has_pod_member (auto begin, auto end) {
  return has_identifier(begin, end) &&
    has_next(std::next(begin), end, {lexem::type_::colon}) && 
    has_identifier(std::next(begin, 2u), end) && 
    has_next(next_identifier(std::next(begin, 2u), end), end, {lexem::type_::semi_colon}) ;
}

auto next_pod_member (auto begin, auto end) {
  return std::next(next_identifier(std::next(next_identifier(begin, end)), end)) ;
}

extracted<pod_member> 
extract_pod_member (auto begin, auto end) {
  auto && name_ext = extract_identifier(begin, end) ;
  auto && type_ext = extract_identifier(std::next(name_ext.last), end) ;
  return make_extracted(std::next(type_ext.last), pod_member{name_ext.ext, type_ext.ext}) ;
}

/**
 * Pod declaration 
 */

bool has_pod_declaration (auto begin, auto end) {
  auto cursor = begin ;

  if (!has_next(cursor, end, {lexem::type_::pod_kw}))
    return false ;
  
  cursor = std::next(cursor) ;

  if (!has_identifier(cursor, end))
    return false ;

  cursor = next_identifier(cursor, end) ;

  if (!has_next(cursor, end, {lexem::type_::lbrace}))
    return false ;

  cursor = std::next(cursor) ;

  while (has_pod_member(cursor, end))
    cursor = next_pod_member(cursor, end) ;

  if (!has_next(cursor, end, {lexem::type_::rbrace})) 
    return false ;

  return true ;
}

auto next_pod_declaration (auto begin, auto end) {
  auto cursor = std::next(next_identifier(std::next(begin), end)) ;

  while (has_pod_member(cursor, end))
    cursor = next_pod_member(cursor, end) ;

  return std::next(cursor) ;
}

extracted<pod_declaration> // TODO HERE
extract_pod_declaration (auto begin, auto end) {
  auto && name_ext = extract_identifier(begin, end) ;
  auto && type_ext = extract_identifier(std::next(name_ext.last), end) ;
  return make_extracted(std::next(type_ext.last), pod_member{name_ext.ext, type_ext.ext}) ;
}
#endif