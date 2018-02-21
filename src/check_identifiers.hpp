#ifndef __thodd_check_identifiers_exist_hpp__
#  define __thodd_check_identifiers_exist_hpp__

#  include "structure.hpp"
#  include "child_ctx.hpp"
#  include "types_by_identifiers.hpp"

#  include <string>
#  include <vector>
#  include <set>
#  include <list>
#  include <algorithm>


std::vector<std::string> get_identifiers (member const & m, std::string const & ctx) ;
std::vector<std::string> get_identifiers (pod const & p, std::string const & ctx) ;
std::vector<std::string> get_identifiers (expression const & exp, std::string const & ctx) ;
std::vector<std::string> get_identifiers (function_call const & fcall, std::string const & ctx) ;
std::vector<std::string> get_identifiers (parameter const & p, std::string const & ctx) ;
std::vector<std::string> get_identifiers (const_instruction const & c, std::string ctx) ;
std::vector<std::string> get_identifiers (return_instruction const & r, std::string ctx) ;
std::vector<std::string> get_identifiers (function const & f, std::string ctx) ;
std::vector<std::string> get_identifiers (thodd const & tdd) ;

std::vector<std::string> get_identifiers (member const & m, std::string const & ctx) {
  return {detail::child_ctx(ctx, m.name.data)} ;
}

std::vector<std::string> get_identifiers (pod const & p, std::string const & ctx) {
  auto local_ctx = detail::child_ctx(ctx, p.name.data) ;
  std::vector<std::string> identifiers {local_ctx} ;
  
  for (member const & m : p.members) {
    auto && m_identifiers = get_identifiers(m, local_ctx) ;
    identifiers.insert(identifiers.end(), m_identifiers.begin(), m_identifiers.end()) ;
  }

  return identifiers ;
}

std::vector<std::string> get_identifiers (expression const & exp, std::string const & ctx) {
  switch (exp.type) {
    case expression::type_::function_call :
      return get_identifiers(extract_function_call(exp.data.begin(), exp.data.end()), ctx) ;
    default : 
      return {} ;
  }
}

std::vector<std::string> get_identifiers (function_call const & fcall, std::string const & ctx) {
  std::list<std::string> identifiers ;

  for (expression const & arg : fcall.args) {
    auto && arg_identifiers = get_identifiers(arg, ctx) ;
    identifiers.insert(identifiers.end(), arg_identifiers.begin(), arg_identifiers.end()) ;
  }

  return {identifiers.begin(), identifiers.end()} ;
}

std::vector<std::string> get_identifiers (const_instruction const & c, std::string ctx) {
  std::vector<std::string> identifiers {detail::child_ctx(ctx, c.name.data)} ;
  auto && value_identifiers = get_identifiers(c.value, ctx) ;
  identifiers.insert(identifiers.end(), value_identifiers.begin(), value_identifiers.end()) ;
  return identifiers ;
}

std::vector<std::string> get_identifiers (return_instruction const & r, std::string ctx) {
  return get_identifiers(r.expr, ctx) ;
}

std::vector<std::string> get_identifiers (function const & f, std::string ctx) {
  auto local_ctx = detail::child_ctx(ctx, f.name.data) ;
  std::list<std::string> identifiers {local_ctx} ;

  for (parameter const & p : f.parameters)  
    identifiers.push_back(detail::child_ctx(local_ctx, p.name.data)) ;
  
  for (const_instruction const & c : f.consts) {
    auto && c_identifiers = get_identifiers(c, local_ctx) ;
    identifiers.insert(identifiers.end(), c_identifiers.begin(), c_identifiers.end()) ;
  }

  auto && return_identifiers = get_identifiers(f.return_, local_ctx) ;
  identifiers.insert(identifiers.end(), return_identifiers.begin(), return_identifiers.end()) ;

  return {identifiers.begin(), identifiers.end()} ;
}

std::vector<std::string> get_identifiers (thodd const & tdd) {
  std::list<std::string> identifiers ;

  for (function const & f : tdd.functions) {
    auto && f_identifiers = get_identifiers(f, "") ;
    identifiers.insert(identifiers.end(), f_identifiers.begin(), f_identifiers.end()) ;
  }

  for (pod const & p : tdd.pods) {
    auto && p_identifiers = get_identifiers(p, "") ;
    identifiers.insert(identifiers.end(), p_identifiers.begin(), p_identifiers.end()) ;
  }

  return {identifiers.begin(), identifiers.end()} ;
}

/// ///////////////// ///
/// PRIVATE FUNCTIONS ///
/// ///////////////// ///


namespace detail {
  
  bool contains (auto begin, auto end, auto const & value) {
    std::find(begin, end, value) != end ;
  }

  bool contains_end_with (auto begin, auto end , auto const value) {
    return std::find_if (begin, end, [&value] (auto && item) {
      auto && mismatch = std::mismatch(item.rbegin(), item.rend(), value.rbegin(), value.rend()) ;
      auto && first_size = std::distance(item.rbegin(), mismatch.first) ;
      auto && second_size = std::distance(value.rbegin(), mismatch.second) ;
      return first_size == second_size && first_size != 0 && second_size != 0 ;
    }) != end ;
  }
}


/// ///////////////// ///
/// CHECK TYPES EXIST ///
/// ///////////////// ///


bool check_types_exist (expression const & exp, auto const & types) ;
bool check_types_exist (function_call const & fcall, auto const & types) ;
bool check_types_exist (function const & f, auto const & types) ;
bool check_types_exist (pod const p , auto const & types) ;
bool check_types_exist (thodd const & tdd) ;


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


bool check_types_exist (function_call const & fcall, auto const & types) {
  bool all_types_exist = true ;

  for (expression const & arg : fcall.args)
    all_types_exist = all_types_exist && check_types_exist(arg, types) ;

  return all_types_exist ;
}

bool check_types_exist (expression const & exp, auto const & types) {
  switch (exp.type) {
    case expression::type_::function_call :
      return check_types_exist(extract_function_call(exp.data.begin(), exp.data.end()), types) ;
    default : return true ;
  }
}

bool check_types_exist (pod const p , auto const & types) {
    return std::all_of (p.members.begin(), p.members.end(), [&types] (member const & m) { 
      return detail::contains_end_with(types.begin(), types.end(), m.type.data) ;
    }) ;
}

bool check_types_exist (thodd const & tdd) {
  std::vector<std::string> types {"int", "uint", "float", "string"} ;

  for (pod const & p : tdd.pods)
    types.push_back(p.name.data) ;

  auto all_types_exist = [&types] (auto const & el) {return check_types_exist(el, types) ;};

  return std::all_of(tdd.pods.begin(), tdd.pods.end(), all_types_exist) && 
    std::all_of(tdd.functions.begin(), tdd.functions.begin(), all_types_exist) ;
}


/// /////////////////////// ///
/// CHECK IDENTIFIERS EXIST ///
/// /////////////////////// ///


bool check_identifiers_exist (expression const & exp, std::string const & ctx, auto const & identifiers) ;
bool check_identifiers_exist (function_call const & fcall, std::string const & ctx, auto const & identifiers) ;
bool check_identifiers_exist (identifier const & id, std::string const & ctx, auto const & identifiers) ;
bool check_identifiers_exist (parameter const & p, std::string const & ctx, auto const & identifiers) ;
bool check_identifiers_exist (const_instruction const & c, std::string ctx, auto const & identifiers) ;
bool check_identifiers_exist (return_instruction const & r, std::string ctx, auto const & identifiers) ;
bool check_identifiers_exist (function const & f, std::string ctx, auto const & identifiers) ;
bool check_identifiers_exist (thodd const & tdd) ;


 
bool check_identifiers_exist (expression const & exp, std::string const & ctx, auto const & identifiers) {
  switch (exp.type) {
    case expression::type_::function_call :
      return check_identifiers_exist(extract_function_call(exp.data.begin(), exp.data.end()), ctx, identifiers) ;
    case expression::type_::identifier :
      return check_identifiers_exist(extract_identifier(exp.data.begin(), exp.data.end()), ctx, identifiers) ;
    default : 
      return true ;
  }
}

bool check_identifiers_exist (identifier const & id, std::string const & ctx, auto const & identifiers) {
  return detail::contains_end_with(identifiers.begin(), identifiers.end(), detail::child_ctx(ctx, id.data)) ;
}

bool check_identifiers_exist (function_call const & fcall, std::string const & ctx, auto const & identifiers) {
  bool all_identifiers_exists = true ;  

  for (expression const & arg : fcall.args)
    all_identifiers_exists = all_identifiers_exists && check_identifiers_exist(arg, ctx, identifiers) ;

  return detail::contains_end_with(identifiers.begin(), identifiers.end(), detail::child_ctx(ctx, fcall.name.data)) ;
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

bool check_identifiers_exist (thodd const & tdd) {
  std::vector<std::string> identifiers ;
  
  return std::all_of (tdd.functions.begin(), tdd.functions.end(), [&identifiers] (function const & f) {
    return check_identifiers_exist(f, "", identifiers) ;
  }) ;
}


/// //////////////////// ///
/// ALREADY EXISTS TYPES ///
/// //////////////////// ///


bool check_types_not_duplicate (thodd const & tdd) {
  std::multiset<std::string> types {"int", "uint", "float", "string"} ;

  for (pod const & p : tdd.pods)
    types.insert(p.name.data) ; 

  return std::adjacent_find(types.begin(), types.end()) == types.end() ;
}

/// ////////////////////////// ///
/// ALREADY EXISTS IDENTIFIERS ///
/// ////////////////////////// ///

bool check_identifiers_not_duplicate (thodd const & tdd) {
  auto && tdd_identifiers = get_identifiers (tdd) ;
  std::multiset<std::string> identifiers {tdd_identifiers.begin(), tdd_identifiers.end()} ;
  return std::adjacent_find(identifiers.begin(), identifiers.end()) == identifiers.end() ;
}


/// /////////////////////////////// ///
/// CHECK IDENTIFIERS ACCESS EXISTS ///
/// /////////////////////////////// ///


using access_by_context_t = std::map<std::string, std::vector<std::string>> ;

access_by_context_t get_accesses (access const & acc, std::string const & ctx) ;
access_by_context_t get_accesses (function_call const & fcall, std::string const & ctx) ;
access_by_context_t get_accesses (expression const & exp, std::string const & ctx) ;
access_by_context_t get_accesses (const_instruction const & c, std::string ctx) ;
access_by_context_t get_accesses (return_instruction const & r, std::string ctx) ;
access_by_context_t get_accesses (function const & f, std::string ctx) ;
access_by_context_t get_accesses (thodd const & tdd) ;



access_by_context_t get_accesses (access const & acc, std::string const & ctx) {
  access_by_context_t accesses ;
  std::vector<std::string> members_str ;

  for (auto && mb : acc.members)  members_str.push_back(mb.data) ;

  return {{acc.ident.data, members_str}} ;
}

access_by_context_t get_accesses (function_call const & fcall, std::string const & ctx) {
  access_by_context_t accesses ;

  for (expression const & arg : fcall.args) {
    auto && arg_accesses = get_accesses(arg, ctx) ;
    accesses.insert(arg_accesses.begin(), arg_accesses.end()) ;
  }

  return accesses ;
}

access_by_context_t get_accesses (expression const & exp, std::string const & ctx) {
  if (has_function_call(exp.data.begin(), exp.data.end())) return get_accesses(extract_function_call(exp.data.begin(), exp.data.end()), ctx) ;
  else if (has_access(exp.data.begin(), exp.data.end())) return get_accesses(extract_access(exp.data.begin(), exp.data.end()), ctx) ;
  else return {} ;
}

access_by_context_t get_accesses (const_instruction const & c, std::string ctx) {
  return get_accesses(c.value, ctx) ;
}

access_by_context_t get_accesses (return_instruction const & r, std::string ctx) {
  return get_accesses(r.expr, ctx) ;
}

access_by_context_t get_accesses (function const & f, std::string ctx) {
  access_by_context_t accesses ; 

  for (const_instruction const & c : f.consts) {
    auto && c_accesses = get_accesses(c, ctx) ;
    accesses.insert(c_accesses.begin(), c_accesses.end()) ;
  }

  auto && return_accesses = get_accesses(f.return_, ctx) ;
  accesses.insert(return_accesses.begin(), return_accesses.end()) ;

  return accesses ;
}

access_by_context_t get_accesses (thodd const & tdd) {
  access_by_context_t accesses ;

  for (function const & f : tdd.functions) {
    auto && f_accesses = get_accesses(f, "") ;
    accesses.insert(f_accesses.begin(), f_accesses.end()) ;
  }

  return accesses ;
}





bool check_access_exists (thodd const & tdd) {
  type_by_identifier_t && registry = types_registry(tdd) ;
  //access_by_context_t && all_access = get_access(tdd) ;
}

#endif