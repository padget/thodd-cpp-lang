#ifndef __thodd_build_context_hpp__
#  define __thodd_build_context_hpp__

#  include <vector>
#  include <string>
#  include <map>

#  include "structure.hpp"

struct context_identifier {
  std::string name ;
  std::string type ;
} ;

std::vector<context_identifier>
build_context(std::string const &, thodd const &) ;

std::vector<context_identifier>
build_context(std::string const &, declaration const &) ;

std::vector<context_identifier>
build_context(std::string const &, function_declaration const &) ;

context_identifier
build_context(std::string const &, parameter const &) ;

std::vector<context_identifier>
build_context(std::string const &, pod_declaration const &) ;

context_identifier
build_context(std::string const &, pod_member const &) ;

std::vector<context_identifier>
build_context(std::string const &, instruction const &) ;

std::vector<context_identifier>
build_context(std::string const &, if_statement const &) ;

std::vector<context_identifier>
build_context(std::string const &, while_statement const &) ;

std::vector<context_identifier>
build_context(std::string const &, return_statement const &) ;

context_identifier
build_context(std::string const &, const_declaration const &) ;


// std::chrono::now
// => {{std, [chrono]}, {std::chrono, [now]}}
std::map<std::string, std::vector<std::string>>
merge_context(std::vector<std::string> const &) ;

#endif