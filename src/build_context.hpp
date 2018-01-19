#ifndef __thodd_build_context_hpp__
#  define __thodd_build_context_hpp__

#  include <vector>
#  include <string>
#  include <map>

#  include "structure.hpp"

/**
 * 
 */ 
std::vector<std::string>
build_context(std::string const &, thodd const &) ;

std::vector<std::string>
build_context(std::string const &, declaration const &) ;

std::vector<std::string>
build_context(std::string const &, function_declaration const &) ;

std::vector<std::string>
build_context(std::string const &, pod_declaration const &) ;

std::vector<std::string>
build_context(std::string const &, pod_member const &) ;

std::vector<std::string>
build_context(std::string const &, instruction const &) ;

std::vector<std::string>
build_context(std::string const &, if_statement const &) ;

std::vector<std::string>
build_context(std::string const &, while_statement const &) ;

std::vector<std::string>
build_context(std::string const &, return_statement const &) ;

std::vector<std::string>
build_context(std::string const &, const_declaration const &) ;

std::vector<std::string>
build_context(std::string const &, expression const &) ;

std::vector<std::string>
build_context(std::string const &, function_call const &) ;

std::vector<std::string>
build_context(std::string const &, identifier const &) ;


// std::chrono::now
// => {{std, [chrono]}, {std::chrono, [now]}}
std::map<std::string, std::vector<std::string>>
merge_context(
  std::vector<std::string> const &, 
  std::vector<std::string> const &) ;
#endif