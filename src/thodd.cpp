#include "from_file.hpp"
#include "structure.hpp"
#include "regexes.hpp"
#include "extract_lexems.hpp"
#include "extract_element.hpp"

#include <iostream>

int main () {
  auto const & stream   = from_file("main.thodd") ;
  auto const & lexems   = extract_lexems(stream.begin(), stream.end(), thodd_rxs()) ;
  auto const & filtered = filter_lexems(lexems.begin(), lexems.end()) ;

  if (has_thodd(filtered.begin(), filtered.end())) {
    std::cout << extract_thodd("main", filtered.begin(), filtered.end()).ext.filename << std::endl ;
  }
}