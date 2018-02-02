#ifndef __thodd_structure_hpp__
#  define __thodd_structure_hpp__

#  include <vector>
#  include <string>

#  include "signatures.hpp"

struct expression {
  enum class type_ {
     identifier, identifiers, number, lambda, function_call
  } ;

  type_ type ;
  std::vector<lexem> data ;
} ;

struct identifier {
  std::string data ;
} ;

struct identifiers {
  std::vector<identifier> idents ;
} ;

struct number {
  std::string data ;
} ;

struct string {
  std::string data ;
} ;

struct function_call { 
  identifier name ;
  std::vector<expression> args ;
} ;

struct parameter {
  identifier name ;
  identifier type ;
} ;

struct const_instruction {
  identifier name ;
  identifier type ;
  expression value ;
} ;

struct return_instruction {
  expression expr ;
} ;


struct lambda_expression {
  identifier name ;
  identifier type ;
  std::vector<parameter> parameters ;
  std::vector<const_instruction> const_instructions ;
  return_instruction return_ ;
} ;

struct function_declaration {
  identifier name ;
  identifier type ;
  std::vector<parameter> parameters ;
  std::vector<const_instruction> const_instructions ;
  return_instruction return_ ;
} ;

struct pod_member {
  identifier name ;
  identifier type ;
} ;

struct pod_declaration {
  identifier name ;
  std::vector<pod_member> members ;
} ;

struct declaration {
  enum class type_ {
    import, function, pod
  } ;

  type_ type ;
  std::vector<lexem> data ;
} ;

struct thodd {
  std::string filename ;
  std::vector<declaration> declarations ;
} ;

#endif