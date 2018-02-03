#ifndef __thodd_extract_element_hpp__
#  define __thodd_extract_element_hpp__

#  include "signatures.hpp"
#  include "structure.hpp"

string_so extract_string (auto begin, auto end) {
  return string_so{(*begin).data} ;
}

identifier_so extract_identifier (auto begin, auto end) {
  return identifier_so{(*begin).data} ;
}

number_so extract_number (auto begin, auto end) {
  return number_so{(*begin).data} ;
}

#endif 