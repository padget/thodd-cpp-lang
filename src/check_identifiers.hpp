#ifndef __thodd_check_identifiers_hpp__
#  define __thodd_check_identifiers_hpp__

#  include "structure.hpp"
#  include <string>
#  include <vector>
#  include <algorithm>
bool check_identifiers (expression const & exp, std::string const & ctx, std::vector<std::string> const & identifiers) ;
bool check_identifiers (function_call const & exp, std::string const & ctx, std::vector<std::string> const & identifiers) ;
bool check_identifiers (identifier const & exp, std::string const & ctx, std::vector<std::string> const & identifiers) ;
//bool check_identifiers (lambda const & exp, std::string const & ctx, std::vector<std::string> const & identifiers) ;


bool check_identifiers (expression const & exp, std::string const & ctx, std::vector<std::string> const & identifiers) {
  switch (exp.type) {
    case expression::type_::function_call: 
      return check_identifiers(extract_function_call(exp.data.begin(), exp.data.end()), ctx, identifiers) ;
    case expression::type_::identifier: 
      return check_identifiers(extract_identifier(exp.data.begin(), exp.data.end()), ctx, identifiers) ;
    default: return true ;
  }
}

bool check_identifiers (function_call const & fn, std::string const & ctx, std::vector<std::string> const & identifiers) {
  bool all_identifiers_exists = check_identifiers(fn.name, ctx, identfiers  ;

  for (expression const & e : fn.args) 
    all_identifiers_exists = all_identifiers_exists && check_identifiers(e, ctx, identfiers) ;

  return all_identifiers_exists ;
}

bool check_identifiers (identifier const & exp, std::string const & ctx, std::vector<std::string> const & identifiers) {
  return std::find(identifiers.begin(), identifiers.end(), ctx + "::" + exp.data) != identifiers.end() ;
}

bool check_identifiers (thodd const & tdd) {
  std::vector<std::string> types {"int", "uint", "float", "string"} ;
  
  for (pod const & p : tdd.pods) {
    types.push_back(pod.name.data) ;

    for (member const & m : p.members) 
      identifiers.push_back(p.name.data + "::" + m.name.data) ;
  }

  for (function const & f : tdd.functions) {
    identifiers.push_back(f.name.data) ;

    for (parameter const & p : f.parameters)
      identifiers.push_back(f.name.data + "::" + p.name.data) ;

    for (const_instruction const & c : f.consts)
      identfiers.push_back(f.name.data + "::" + c.name.data) ;
  }

  bool all_types_exists = true ;

  for (pod const & p : tdd.pods)
    for (member const & m : p.members) 
      all_types_exists = all_types_exists && std::find(types.begin(), types.end(), m.type.data) != types.end() ;

  for (function const & f : tdd.functions) {
    all_types_exists = all_types_exists && std::find(types.begin(), types.end(), f.type.data) != types.end() ;
    
    for (parameter const & p : f.parameters)
      all_types_exists = all_types_exists && std::find(types.begin(), types.end(), p.type.data) != types.end() ;

    for (const_instruction const & c : f.consts)
      all_types_exists = all_types_exists && std::find(types.begin(), types.end(), c.type.data) != types.end() ;
  }

  // TODO faire check type pour les lambdas

  bool all_identifiers_exists = true ;

  for (function const & f : tdd.functions) {
    all_identifiers_exists = all_identifiers_exists && check_identifiers() ;
  }

  return all_types_exists ;
}

#endif