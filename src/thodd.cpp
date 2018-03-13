//#include "element/element.hpp"

#include <iostream>
#include <string_view>
#include <string>
#include <vector>

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

  std::string const && to_analyse = "toto";//from_file("main.thodd") ;
  
  std::vector<lexer::lexem> const && lexems   = lexer::extract_lexems(to_analyse.begin(), to_analyse.end(), lexer::thodd_rxs(0)) ;
  std::vector<lexer::lexem> const && filtered = lexer::filter_lexems(lexems.begin(), lexems.end()) ;
  
  auto const & has_expression = element::has_expression(filtered.begin(), filtered.end()) ;
  std::cout << "has_expression" << std::boolalpha << std::get<element::has_idx>(has_expression) << std::endl ; 


  println("fin test thodd") ;
  return EXIT_SUCCESS ;
}