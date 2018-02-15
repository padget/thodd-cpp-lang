#ifndef __thodd_types_by_identifiers_hpp__
#  define __thodd_types_by_identifiers_hpp__

#  include <map>
#  include <string>

#  include "structure.hpp"
#  include "child_ctx.hpp"
#  include "extract_element.hpp"


using type_by_identifier_t = std::map<std::string, std::string> ;

type_by_identifier_t type_by_identifier (function_call const & fcall, std::string const & ctx) ;
type_by_identifier_t type_by_identifier (lambda const & l, std::string const & ctx) ;
type_by_identifier_t type_by_identifier (expression const & exp, std::string const & ctx) ;
type_by_identifier_t type_by_identifier (const_instruction const & c, std::string const & ctx) ;
type_by_identifier_t type_by_identifier (return_instruction const & r, std::string const & ctx) ;
type_by_identifier_t type_by_identifier (function const & f, std::string const & ctx) ;
type_by_identifier_t type_by_identifier (member const & m, std::string const & ctx) ;
type_by_identifier_t type_by_identifier (pod const & p, std::string const & ctx) ;
type_by_identifier_t type_by_identifier (thodd const & tdd, std::string const & ctx) ;

type_by_identifier_t types_registry (thodd const & tdd) ;



type_by_identifier_t type_by_identifier (function_call const & fcall, std::string const & ctx) {
  type_by_identifier_t tbi ;

  for (expression const & arg : fcall.args) {
    auto && arg_tbi = type_by_identifier(arg, ctx) ;
    tbi.insert(arg_tbi.begin(), arg_tbi.end()) ;
  }

  return tbi ;
}

type_by_identifier_t type_by_identifier (lambda const & l, std::string const & ctx) {
  std::string local_ctx = detail::child_ctx(ctx, l.name.data) ;
  type_by_identifier_t tbi {{local_ctx, l.type.data}} ;

  for (parameter const & p : l.parameters)
    tbi.insert({detail::child_ctx(local_ctx, p.name.data) , p.type.data}) ;

  for (const_instruction const & c : l.consts) {
    auto && c_tbi = type_by_identifier(c, local_ctx) ;
    tbi.insert(c_tbi.begin(), c_tbi.end()) ;
  }

  auto && return_tbi = type_by_identifier(l.return_, local_ctx) ;
  tbi.insert(return_tbi.begin(), return_tbi.end()) ;

  return tbi ;
}

type_by_identifier_t type_by_identifier (expression const & exp, std::string const & ctx) {
  switch (exp.type) {
    case expression::type_::lambda : 
      return type_by_identifier(extract_lambda(exp.data.begin(), exp.data.end()), ctx) ;
    case expression::type_::function_call : 
      return type_by_identifier(extract_function_call(exp.data.begin(), exp.data.end()), ctx) ;
    default : return {} ;
  }
}

type_by_identifier_t type_by_identifier (const_instruction const & c, std::string const & ctx) {
  type_by_identifier_t tbi {{detail::child_ctx(ctx, c.name.data), c.type.data}} ;
  auto && value_tbi = type_by_identifier(c.value, ctx) ;
  tbi.insert(value_tbi.begin(), value_tbi.end()) ;
  return tbi ;
}

type_by_identifier_t type_by_identifier (return_instruction const & r, std::string const & ctx) {
  return type_by_identifier(r.expr, ctx) ;
}

type_by_identifier_t type_by_identifier (function const & f, std::string const & ctx) {
  std::string local_ctx = detail::child_ctx(ctx, f.name.data) ;
  type_by_identifier_t tbi {{local_ctx, f.type.data}} ;

  for (parameter const & p : f.parameters)
    tbi.insert({detail::child_ctx(local_ctx, p.name.data) , p.type.data}) ;

  for (const_instruction const & c : f.consts) {
    auto && c_tbi = type_by_identifier(c, local_ctx) ;
    tbi.insert(c_tbi.begin(), c_tbi.end()) ;
  }

  auto && return_tbi = type_by_identifier(f.return_, local_ctx) ;
  tbi.insert(return_tbi.begin(), return_tbi.end()) ;

  return tbi ;
}

type_by_identifier_t type_by_identifier (pod const & p, std::string const & ctx) {
  type_by_identifier_t tbi ;

  for (member const & m : p.members) 
    tbi.insert({detail::child_ctx(ctx, m.name.data) , m.type.data}) ;

  return tbi ;
}

type_by_identifier_t types_registry (thodd const & tdd) {
  type_by_identifier_t tbi ;

  for (pod const & p : tdd.pods) {
    auto && p_tbi = type_by_identifier(p, "") ;
    tbi.insert(p_tbi.begin(), p_tbi.end()) ;
  }

  return tbi ; 
}

type_by_identifier_t type_by_identifier (thodd const & tdd) {
  type_by_identifier_t tbi = types_registry(tdd) ;

  for (function const & f : tdd.functions) {
    auto && f_tbi = type_by_identifier(f, "") ;
    tbi.insert(f_tbi.begin(), f_tbi.end()) ;
  }

  return tbi ;
}



#endif