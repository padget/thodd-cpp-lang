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
  
  if (has_function_call(begin, end))
    cursor = next_function_call(begin, end) ;
  else if (has_identifier(begin, end))
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

  while (has_expression(cursor, end))
    cursor = next_expression(cursor, end) ;

  return std::next(cursor) ;
}

extracted<function_call> 
extract_function_call (auto begin, auto end) {
  auto && fname = extract_identifier(begin, end) ;
  auto cursor = fname.last ;
  std::vector<expression> expressions ;

  while (has_expression(cursor, end)) {
    auto && extracted = extract_expression(cursor, end) ;
    expressions.push_back(extracted.ext) ;
    cursor = extracted.last ;
  }
    

  return make_extracted(
    std::next(cursor, 2u), 
    function_call{fname.ext, expressions}) ;  
}


 
#endif