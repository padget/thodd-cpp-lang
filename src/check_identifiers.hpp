#ifndef __thodd_check_identifiers_exist_hpp__
#  define __thodd_check_identifiers_exist_hpp__

#  include "structure.hpp"
#  include <string>
#  include <vector>
#  include <algorithm>

#  include <iostream>

bool check_types_exist (expression const & exp, auto const & types) ;
bool check_types_exist (lambda const & l, auto const & types) ;
bool check_types_exist (function_call const & fcall, auto const & types) ;
bool check_types_exist (function const & f, auto const & types) ;



bool check_identifiers_exist (expression const & exp, std::string const & ctx, auto const & identifiers) ;
bool check_identifiers_exist (function_call const & fcall, std::string const & ctx, auto const & identifiers) ;
bool check_identifiers_exist (identifier const & id, std::string const & ctx, auto const & identifiers) ;
bool check_identifiers_exist (lambda const & lam, std::string const & ctx, auto const & identifiers) ;
bool check_identifiers_exist (parameter const & p, std::string const & ctx, auto const & identifiers) ;
bool check_identifiers_exist (const_instruction const & c, std::string ctx, auto const & identifiers) ;
bool check_identifiers_exist (return_instruction const & r, std::string ctx, auto const & identifiers) ;
bool check_identifiers_exist (function const & f, std::string ctx, auto const & identifiers) ;

/// ///////////////// ///
/// PRIVATE FUNCTIONS ///
/// ///////////////// ///


namespace detail {
  bool contains (auto begin, auto end, auto const & value) {
    std::find(begin, end, value) != end ;
  }
}


/// ///////////////// ///
/// CHECK TYPES EXIST ///
/// ///////////////// ///

bool check_types_exist_function_body (auto const & body, auto const & types) {
  bool all_types_exist = detail::contains(types.begin(), types.end(), body.type.data) ;

  for (parameter const & p : body.parameters)
    all_types_exist = all_types_exist && detail::contains(types.begin(), types.end(), p.type.data) ;

  for (const_instruction const & c : body.consts)
    all_types_exist = all_types_exist && detail::contains(types.begin(), types.end(), c.type.data) && check_types_exist(c.value, types) ;

  return all_types_exist && check_types_exist(body.return_.expr, types) ;
}

bool check_types_exist (function const & f, auto const & types) {
  return check_types_exist_function_body(f, types) ;
} 

bool check_types_exist (lambda const & l, auto const & types) {
  return check_types_exist_function_body(l, types) ;
}

bool check_types_exist (function_call const & fcall, auto const & types) {
  bool all_types_exist = true ;

  for (expression const & arg : fcall.args)
    all_types_exist = all_types_exist && check_types_exist(arg, types) ;

  return all_types_exist ;
}

bool check_types_exist (expression const & exp, auto const & types) {
  if (has_lambda(exp.data.begin(), exp.data.end())) 
    return check_types_exist(extract_lambda(exp.data.begin(), exp.data.end()), types) ;
  
  if (has_function_call(exp.data.begin(), exp.data.end()))
    return check_types_exist(extract_function_call(exp.data.begin(), exp.data.end()), types) ;
  
  return true ;
}


/// /////////////////////// ///
/// CHECK IDENTIFIERS EXIST ///
/// /////////////////////// ///

namespace detail {
  auto child_ctx(auto const & ctx, auto const & child) {
    return ctx + "::" + child ;
  }
}
 
bool check_identifiers_exist (expression const & exp, std::string const & ctx, auto const & identifiers) {
  switch (exp.type) {
    case expression::type_::function_call :
      return check_identifiers_exist(extract_function_call(exp.data.begin(), exp.data.end()), ctx, identifiers) ;
    case expression::type_::lambda : 
      return check_identifiers_exist(extract_lambda(exp.data.begin(), exp.data.end()), ctx, identifiers) ;
    case expression::type_::identifier :
      return check_identifiers_exist(extract_identifier(exp.data.begin(), exp.data.end()), ctx, identifiers) ;
    default : 
      return true ;
  }
}

bool check_identifiers_exist (identifier const & id, std::string const & ctx, auto const & identifiers) {
  for (auto && ide : identifiers) std::cout << ide << " vs " << detail::child_ctx(ctx, id.data)<< std::endl ; 
  return detail::contains(identifiers.begin(), identifiers.end(), detail::child_ctx(ctx, id.data)) ;
}

bool check_identifiers_exist (function_call const & fcall, std::string const & ctx, auto const & identifiers) {
  bool all_identifiers_exists = true ;  

  for (expression const & arg : fcall.args)
    all_identifiers_exists = all_identifiers_exists && check_identifiers_exist(arg, ctx, identifiers) ;

  return detail::contains(identifiers.begin(), identifiers.end(), detail::child_ctx(ctx, fcall.name.data)) ;
}

bool check_identifiers_exist (lambda const & lam, std::string const & ctx, auto const & identifiers) {
  bool all_identifiers_exists = true ;
  auto local_identifiers = identifiers ;
  auto local_ctx = detail::child_ctx(ctx, lam.name.data) ;
  local_identifiers.push_back(local_ctx) ;

  for (parameter const & p : lam.parameters)  
    local_identifiers.push_back(detail::child_ctx(local_ctx, p.name.data)) ;
  
  for (const_instruction const & c : lam.consts) {
    all_identifiers_exists = all_identifiers_exists && check_identifiers_exist(c, local_ctx, local_identifiers) ;
    local_identifiers.push_back(c.name.data) ;
  }

  return all_identifiers_exists && check_identifiers_exist(lam.return_, local_ctx, local_identifiers) ;
}

bool check_identifiers_exist (const_instruction const & c, std::string ctx, auto const & identifiers) {
  return check_identifiers_exist(c.value, ctx, identifiers) ;
}

bool check_identifiers_exist (return_instruction const & r, std::string ctx, auto const & identifiers) {
  return check_identifiers_exist(r.expr, ctx, identifiers) ;
}

bool check_identifiers_exist (function const & f, std::string ctx, auto const & identifiers) {
  bool all_identifiers_exists = true ;
  auto local_identifiers = identifiers ;
  auto local_ctx = detail::child_ctx(ctx, f.name.data) ;
  local_identifiers.push_back(local_ctx) ;
  
  for (parameter const & p : f.parameters)  
    local_identifiers.push_back(detail::child_ctx(local_ctx, p.name.data)) ;

  for (const_instruction const & c : f.consts) {
    all_identifiers_exists = all_identifiers_exists && check_identifiers_exist(c, local_ctx, local_identifiers) ;
    local_identifiers.push_back(c.name.data) ;
  }

  return all_identifiers_exists && check_identifiers_exist(f.return_, local_ctx, local_identifiers) ;
}


bool check_all_identifiers (thodd const & tdd) {
  std::vector<std::string> types {"int", "uint", "float", "string"} ;

  for (pod const & p : tdd.pods)
    types.push_back(p.name.data) ;

  bool all_types_exists = true ;

  for (pod const & p : tdd.pods)
    for (member const & m : p.members) 
      all_types_exists = all_types_exists && detail::contains(types.begin(), types.end(), m.type.data) ;

  for (function const & f : tdd.functions)
    all_types_exists = all_types_exists && check_types_exist(f, types) ;

  std::vector<std::string> identifiers ;
  bool all_identifiers_exist = true ;

  for (function const & f : tdd.functions) 
    all_identifiers_exist = all_identifiers_exist && check_identifiers_exist(f, "", identifiers) ;

  return all_types_exists && all_identifiers_exist ;
}

#endif