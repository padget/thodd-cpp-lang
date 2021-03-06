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
  
  container::for_each(filtered, [] (auto const & lx){
    std::cout << (int) lx.type << "(at: l." << lx.line << "), ";
  }) ; 
  std::cout << std::endl ;
    
  auto const & has_writer = element::has_writer(container::begin(filtered), container::end(filtered)) ;
  std::cout << "has_writer" << std::boolalpha << std::get<element::has_idx>(has_writer) << std::endl ; 


  println("fin test thodd") ;



  struct person {int age ;} ;

  container::list<person> ps ;
  ps = container::push(ps, person{10}) ;
  ps = container::push(ps, person{12}) ;

  auto && ages = container::map(ps, [] (person const & p) {return p.age ;}) ;
  auto && sum  = container::reduce(ages, 0, [] (int const & sum, int const & age) {return sum + age ;}) ;

  std::cout << sum << std::endl ; 

  return EXIT_SUCCESS ;
}