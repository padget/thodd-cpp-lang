#ifndef __thodd_structure_hpp__
#  define __thodd_structure_hpp__

#  include <vector>
#  include <string>

struct lexem {
  enum class type_ : int {
    identifier, number,
    lbrace, rbrace, string, 
    lbracket, rbracket,
    lsbracket, rsbracket,
    colon, semi_colon, 
    point, comma, ignored, 
    pure_kw, impure_kw, pod_kw,
    weak, strengh, alias, 
    if_kw, for_kw, switch_kw, while_kw, case_kw
  } ;

  type_ type ;
  std::string data ;
} ;

/**
 * Structure du langage thodd
 */ 

struct expression {
  enum class type_ {
    function_call, identifier, number
  } ;

  type_ type ;
  std::vector<lexem> data ;
} ;

struct instruction {
  enum class type_ {
    return_statement, if_statement, while_statement, for_statement, const_declaration
  } ;

  type_ type ;
  std::vector<lexem> data ;
} ;

struct import_declaration {
  
} ;

struct identifier {
  std::string data ;
} ;

struct number {
  std::string data ;
} ;

struct function_call {
  identifier fname ;
  std::vector<expression> params ; 
} ;

struct const_declaration {
  identifier name ;
  identifier type ;
  expression value ;
} ;

struct if_statement {
  expression test ;
  std::vector<instruction> instructions ;
} ;

struct while_statement {
  expression test ;
  std::vector<instruction> instructions ;
} ;

struct return_statement {
  expression expr ;
} ;

struct for_statement {
  expression range ;
  const_declaration item ;
  std::vector<instruction> instructions ;
} ;

struct case_statement {
  expression test ;
  std::vector<instruction> instructions ;
} ;

struct switch_statement {
  expression test ;
  std::vector<case_statement> cases ;
} ;


struct function_declaration {
  identifier name ;
  identifier type ;
  std::vector<instruction> instructions ;
} ;

struct pod_member {
  identifier name ;
  identifier type ;
} ;

struct pod_declaration {
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
  std::vector<declaration> decls ;
} ;

#endif