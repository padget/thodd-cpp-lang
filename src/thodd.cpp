#include "from_file.hpp"
#include "structure.hpp"
#include "regexes.hpp"
#include "extract_lexems.hpp"
#include "extract_element.hpp"
#include "build_context.hpp"

#include <iostream>

void print_lexems(auto begin, auto end) {
  std::cout << "----- extracted lexems -----" << std::endl ;
  std::for_each (begin, end, [] (auto && lex) {std::cout << lex.data << std::endl ; }) ;
  std::cout << "----- ---------------- -----" << std::endl ;
}

int main () {
  auto const & stream   = from_file("main.thodd") ;
  auto const & lexems   = extract_lexems(stream.begin(), stream.end(), thodd_rxs()) ;
  auto const & filtered = filter_lexems(lexems.begin(), lexems.end()) ;

  print_lexems (filtered.begin(), filtered.end()) ;

  if (has_thodd(filtered.begin(), filtered.end())) {
    std::cout << "has thodd !!\n" ; 
    auto const && contexts = build_context("main", extract_thodd("main", filtered.begin(), filtered.end()).ext) ;
    
    for (auto const & ctxt : contexts) std::cout << ctxt.name << "\n" ;
  }

  return EXIT_SUCCESS ;
}