#include <vector>
#include <string>
#include <regex>


struct lexem {
  enum class type_ {
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

struct rx {
  std::regex reg ; 
  lexem::type_ type ;
} ;

rx
make_rx (
  std::regex const & reg, 
  lexem::type_ type) {
  return {reg, type} ;
}

std::vector<rx>
thodd_rxs () {
  return {
    make_rx(std::regex("^[a-z_]+"), lexem::type_::identifier),
    make_rx(std::regex("^\\("), lexem::type_::lbracket),
    make_rx(std::regex("^\\)"), lexem::type_::rbracket),
    make_rx(std::regex("^\\{"), lexem::type_::lbrace),
    make_rx(std::regex("^\\}"), lexem::type_::rbrace),
    make_rx(std::regex("^:"), lexem::type_::colon),
    make_rx(std::regex("^;"), lexem::type_::semi_colon),
    make_rx(std::regex("^,"), lexem::type_::comma),
    make_rx(std::regex("^##"), lexem::type_::alias),
    make_rx(std::regex("^\\[#\\]"), lexem::type_::strengh),
    make_rx(std::regex("^#"), lexem::type_::weak),
    make_rx(std::regex("^[0-9]+(\\.[0-9]+){0,1}"), lexem::type_::number),
    make_rx(std::regex("\".*\""), lexem::type_::string),
    make_rx(std::regex("^[ \\n\\t]+"), lexem::type_::ignored),
    make_rx(std::regex("^pure"), lexem::type_::pure_kw),
    make_rx(std::regex("^impure"), lexem::type_::impure_kw),
    make_rx(std::regex("^pod"), lexem::type_::pod_kw),
    make_rx(std::regex("^if"), lexem::type_::if_kw),
    make_rx(std::regex("^for"), lexem::type_::for_kw),
    make_rx(std::regex("^while"), lexem::type_::while_kw),
    make_rx(std::regex("^switch"), lexem::type_::switch_kw),
    make_rx(std::regex("^case"), lexem::type_::case_kw)
  } ;
}

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

struct return_statement {
  expression expr ;
} ;

struct while_statement {
  expression test ;
  std::vector<instruction> instructions ;
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

/**
 * Fonctions d'extraction des structures 
 */


int main () {}