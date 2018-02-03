#ifndef __thodd_structure_hpp__
#  define __thodd_structure_hpp__

#  include <vector>
#  include <string>

#  include "signatures.hpp"

struct expression_so {
  enum class type_ {
     identifier, identifiers, number, lambda, function_call
  } ;

  type_ type ;
  std::vector<lexem> data ;
} ;

struct identifier_so {
  std::string data ;
} ;

struct identifiers_so {
  std::vector<identifier_so> idents ;
} ;

struct number_so {
  std::string data ;
} ;

struct string_so {
  std::string data ;
} ;

struct function_call_so { 
  identifier_so name ;
  std::vector<expression_so> args ;
} ;

struct parameter_so {
  identifier_so name ;
  identifier_so type ;
} ;

struct const_instruction_so {
  identifier_so name ;
  identifier_so type ;
  expression_so value ;
} ;

struct return_instruction_so {
  expression_so expr ;
} ;


struct lambda_expression_so {
  identifier_so name ;
  identifier_so type ;
  std::vector<parameter_so> parameters ;
  std::vector<const_instruction_so> const_instructions ;
  return_instruction_so return_ ;
} ;

struct function_declaration_so {
  identifier_so name ;
  identifier_so type ;
  std::vector<parameter_so> parameters ;
  std::vector<const_instruction_so> const_instructions ;
  return_instruction_so return_ ;
} ;

struct pod_member_so {
  identifier_so name ;
  identifier_so type ;
} ;

struct pod_declaration_so {
  identifier_so name ;
  std::vector<pod_member_so> members ;
} ;

struct declaration_so {
  enum class type_ {
    import, function, pod
  } ;

  type_ type ;
  std::vector<lexem> data ;
} ;

struct thodd_so {
  std::string filename ;
  std::vector<declaration_so> declarations ;
} ;

#endif