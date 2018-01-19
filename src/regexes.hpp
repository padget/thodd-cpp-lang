#ifndef __thodd_regexes_hpp__
#  define __thodd_regexes_hpp__

#  include <regex>

#  include "structure.hpp"

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
    make_rx(std::regex("^pure"), lexem::type_::pure_kw),
    make_rx(std::regex("^impure"), lexem::type_::impure_kw),
    make_rx(std::regex("^pod"), lexem::type_::pod_kw),
    make_rx(std::regex("^if"), lexem::type_::if_kw),
    make_rx(std::regex("^for"), lexem::type_::for_kw),
    make_rx(std::regex("^while"), lexem::type_::while_kw),
    make_rx(std::regex("^switch"), lexem::type_::switch_kw),
    make_rx(std::regex("^case"), lexem::type_::case_kw),
    make_rx(std::regex("^return"), lexem::type_::return_kw),
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
    make_rx(std::regex("^[ \\n\\t]+"), lexem::type_::ignored)
  } ;
}

#endif