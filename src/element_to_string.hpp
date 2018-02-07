#ifndef __thodd_element_to_string_hpp__
#  define __thodd_element_to_string_hpp__

#  include <string>
#  include "structure.hpp"

std::string to_string (identifier const & el) ;
std::string to_string (number const & el) ;
std::string to_string (string const & el) ;
std::string to_string (lambda const & el) ;
std::string to_string (function_call const & el) ;
std::string to_string (function const & el) ;
std::string to_string (parameter const & el) ;
std::string to_string (member const & el) ;
std::string to_string (pod const & el) ;
std::string to_string (thodd const & el) ;
std::string to_string (expression const & el) ;


std::string to_string (identifier const & el) {
  return el.data ;
}

std::string to_string (number const & el) {
  return el.data ;
}

std::string to_string (string const & el) {
  return el.data ;
}

std::string to_string (member const & el) {
  return to_string(el.name) + " : " + to_string(el.type) + ";" ;
}

std::string to_string (pod const & el) {
  std::string res = to_string(el.name) + "{" ;

  for (auto && mbr : el.members)
    res += to_string(mbr) ;

  return res + "}" ;
}

std::string to_string (return_instruction const & el) {
  return std::string("return ") + to_string(el.expr) + ";" ;
}

std::string to_string (const_instruction const & el) {
  return to_string(el.name) + " : " + to_string(el.type) + " " + to_string(el.value) ;
}

std::string to_string (expression const & el) {
  switch (el.type) {
    case expression::type_::function_call : return to_string(extract_function_call(el.data.begin(), el.data.end())) ; 
    case expression::type_::lambda : return to_string(extract_lambda(el.data.begin(), el.data.end())) ;
    case expression::type_::identifier : return to_string(extract_identifier(el.data.begin(), el.data.end())) ; 
    case expression::type_::number : return to_string(extract_number(el.data.begin(), el.data.end())) ;
    case expression::type_::string : return to_string(extract_string(el.data.begin(), el.data.end())) ;
    default : return "" ;
  }
}

std::string to_string (function_call const & el) {
  std::string res = to_string(el.name) + "(" ;
  
  for (auto && arg : el.args) 
    res += to_string(arg) + ", " ;

  res.erase(std::next(res.begin(), res.size() - 2), res.end()) ;

  return res + ")" ;
}

std::string to_string (parameter const & el) {
  return to_string(el.name) + " : " + to_string(el.type) ;
}

std::string to_string (lambda const & el) {
  std::string res = to_string(el.name) + "(" ;

  for (auto && param : el.parameters) 
    res += to_string(param) + ", " ;

  res.erase(std::next(res.begin(), res.size() - 2), res.end()) ;
  res += ") : " + to_string(el.type) + "{" ;

  for (auto && const_ : el.consts)  
    res += to_string(const_) ;

  return res + to_string(el.return_) + "}" ;
}

std::string to_string (function const & el) {
  std::string res = to_string(el.name) + "(" ;

  for (auto && param : el.parameters) 
    res += to_string(param) + ", " ;

  res.erase(std::next(res.begin(), res.size() - 2), res.end()) ;
  res += ") : " + to_string(el.type) + "{" ;

  for (auto && const_ : el.consts)  
    res += to_string(const_) ;

  return res + to_string(el.return_) + "}" ;
}

std::string to_string (thodd const & el) {
  std::string res ;

  for (auto && p : el.pods)
    res += to_string(p) ;

  for (auto && f : el.functions) 
    res += to_string(f) ;

  return res ;
}

#endif