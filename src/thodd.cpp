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

  std::cout << std::boolalpha << has_if_statement(filtered.begin(), filtered.end()) ;
}