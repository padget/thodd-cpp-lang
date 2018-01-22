#ifndef __thodd_regexes_hpp__
#  define __thodd_regexes_hpp__

#  include <regex>

#  include "structure.hpp"

struct rx {
  std::regex reg ; 
  lexem::type_ type ;
} ;

rx make_rx (std::regex const & reg, lexem::type_ type) ;
std::vector<rx> thodd_rxs () ;

#endif