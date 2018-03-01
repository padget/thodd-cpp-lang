#ifndef __thodd_element_element_hpp__
#  define __thodd_element_element_hpp__

#  include <vector>
#  include <string>

namespace thodd::element {
  struct expression {
    enum class type_ {
      identifier, access, number, string, function_call, unknown
    } ;

    type_ type ;
    std::vector<lexem> data ;
  } ;

  struct identifier {
    std::string data ;
  } ;

  struct access {
    identifier ident ;
    std::vector<identifier> members ;
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

  struct function {
    identifier name ;
    identifier type ;
    std::vector<parameter> parameters ;
    std::vector<const_instruction> consts ;
    return_instruction return_ ;
  } ;

  struct member {
    identifier name ;
    identifier type ;
  } ;

  struct pod {
    identifier name ;
    std::vector<member> members ;
  } ;

  struct thodd {
    std::string filename ;
    
    std::vector<pod> pods ;
    std::vector<function> functions ;
  } ;
}

#endif