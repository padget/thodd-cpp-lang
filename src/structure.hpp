#ifndef __thodd_structure_hpp__
#  define __thodd_structure_hpp__

#  include <vector>
#  include <string>

struct lexem {
  enum class type_ : int {
    identifier, identifiers, 
    number, string, 
    lbrace, rbrace, 
    lbracket, rbracket,
    lsbracket, rsbracket,
    colon, semi_colon, 
    point, comma, ignored, 
    pure_kw, impure_kw, pod_kw,
    lambda_kw,
    weak, strengh, alias, 
    switch_kw, case_kw, return_kw, 
    iterate_kw, iterate_if_kw
  } ;

  type_ type ;
  std::string data ;
} ;

/**
 * Structure du langage thodd
 */ 

struct expression {
  enum class type_ {
     identifier, identifiers, number, lambda, function_call
  } ;

  type_ type ;
  std::vector<lexem> data ;
} ;

/**
 * [a-z_]+
 */
struct identifier {
  std::string data ;
} ;

/**
 * 
 */ 
struct identifiers {
  std::vector<std::identifier> idents ;
} ;

/**
 * 
 */
struct number {
  std::string data ;
} ;

/**
 * 
 */
struct function_call {
  identifier fname ;
  std::vector<expression> params ; 
} ;

/**
 * 
 */
struct parameter {
  identifier name ;
  identifier type ;
} ;

/**
 * 
 */
struct const_instruction {
  identifier name ;
  identifier type ;
  expression value ;
} ;

/**
 * 
 */
struct return_instruction {
  expression expr ;
} ;

/**
 * 
 */
struct function_declaration {
  identifier name ;
  identifier type ;
  std::vector<parameter> parameters ;
  std::vector<const_instruction> const_instructions ;
  return_instruction return_ ;
} ;

/**
 * 
 */
struct pod_member {
  identifier name ;
  identifier type ;
} ;

/**
 * 
 */
struct pod_declaration {
  identifier name ;
  std::vector<pod_member> members ;
} ;

/**
 * 
 */
struct declaration {
  enum class type_ {
    import, function, pod
  } ;

  type_ type ;
  std::vector<lexem> data ;
} ;

/**
 * 
 */
struct thodd {
  std::string filename ;
  std::vector<declaration> declarations ;
} ;

#endif