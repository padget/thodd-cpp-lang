#include "from_file.hpp"
#include "structure.hpp"
#include "regexes.hpp"
#include "extract_lexems.hpp"
#include "extract_element.hpp"
#include "element_to_string.hpp"
#include "check_identifiers.hpp"
#include "types_by_identifiers.hpp"

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
  auto res = has_thodd(filtered.begin(), filtered.end()) ;
  std::cout << std::boolalpha << res ;
  if (has_thodd(filtered.begin(), filtered.end())) {
    std::cout << "has thodd !!\n" ; 
    auto && tdd = extract_thodd("main", filtered.begin(), filtered.end()) ;
    std::cout << "tdd : " << to_string(tdd) << std::endl ;

    std::cout << std::boolalpha << check_types_exist(tdd) << std::endl ;
    std::cout << std::boolalpha << check_types_not_duplicate(tdd) << std::endl ;
    std::cout << std::boolalpha << check_identifiers_exist(tdd) << std::endl ;
    std::cout << std::boolalpha << check_identifiers_not_duplicate(tdd) << std::endl ; 

    auto && tbi = type_by_identifier(tdd) ;

    for (auto & p : tbi) 
      std::cout << p.first << " : " << p.second << std::endl ; 
    
    std::cout << "end tests" << std::endl ; 
  }

  return EXIT_SUCCESS ;
}