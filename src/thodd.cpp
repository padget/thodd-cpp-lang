#include "from_file.hpp"
#include "structure.hpp"
#include "regexes.hpp"
#include "extract_lexems.hpp"
#include "extract_element.hpp"

#include <iostream>

int main () {
  auto && stream   = from_file("main.thodd") ;
  auto && lexems   = extract_lexems(stream.begin(), stream.end(), thodd_rxs()) ;
  auto && filtered = filter_lexems(lexems.begin(), lexems.end()) ;

  if (has_function_declaration(filtered.begin(), filtered.end())) {
    std::cout << extract_function_declaration(filtered.begin(), filtered.end()).ext.name.data ;
    std::cout << "prpout" << std::endl ;
  }
}