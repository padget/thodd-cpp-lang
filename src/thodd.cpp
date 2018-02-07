#include "from_file.hpp"
#include "structure.hpp"
#include "regexes.hpp"
#include "extract_lexems.hpp"
#include "extract_element.hpp"
#include "element_to_string.hpp"
#include "check_identifiers.hpp"

#include <iostream>

void print_lexems(auto begin, auto end) {
  std::cout << "----- extracted lexems -----" << std::endl ;
  std::for_each (begin, end, [] (auto && lex) {std::cout << lex.data << std::endl ; }) ;
  std::cout << "----- ---------------- -----" << std::endl ;
}

int main () {
  auto const & stream   = from_file("main.thodd") ;
  auto const & lexems   = extract_lexems(stream.begin(), stream.end(), thodd_rxs(0)) ;
  auto const & filtered = filter_lexems(lexems.begin(), lexems.end()) ;

  
  if (has_thodd(filtered.begin(), filtered.end())) {
    std::cout << "has thodd !!\n" ; 
    auto && tdd = extract_thodd("main", filtered.begin(), filtered.end()) ;
    std::cout << "tdd : " << to_string(tdd) ;
    std::cout << "not extraction\n" ;
    std::cout << std::boolalpha << check_all_identifiers(tdd) << std::endl ;
  }

  return EXIT_SUCCESS ;
}