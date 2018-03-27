//#include "element/element.hpp"

#include <iostream>
#include <string_view>
#include <string>
#include <vector>
#include <list>

#include "from_file.hpp"
#include "lexer/lexem.hpp"
#include "lexer/lexems.hpp"
#include "lexer/search.hpp"

#include "element/has.hpp"

using namespace thodd ;

void println (std::string_view str) {
  std::cout << str << '\n' ;
}

void print(std::string_view str, auto && printer) {
  printer(str) ;
}

int main () {
  println("début test thodd") ;

  std::string const && to_analyse = from_file("main.thodd") ;
  
  auto && lexems   = lexer::extract_lexems(to_analyse.begin(), to_analyse.end(), lexer::thodd_rxs(0)) ;
  auto && located  = lexer::add_line_location(lexems) ;
  auto && filtered = lexer::filter_lexems(located) ;
  
  auto const & has_writer = element::has_writer(container::begin(filtered), container::end(filtered)) ;
  std::cout << "has_writer" << std::boolalpha << std::get<element::has_idx>(has_writer) << std::endl ; 
  
  if (!has_writer) 
    auto const & has_writer_errors  = element::has_writer(container::begin(filtered), container::end(filtered)) ;


  println("fin test thodd") ;

  return EXIT_SUCCESS ;
}