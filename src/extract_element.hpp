#ifndef __thodd_extract_element_hpp__
#  define __thodd_extract_element_hpp__

#  include "signatures.hpp"
#  include "structure.hpp"

string extract_string (auto begin, auto end) {
  return string{(*begin).data} ;
}

identifier extract_identifier (auto begin, auto end) {
  return identifier{(*begin).data} ;
}

number extract_number (auto begin, auto end) {
  return number{(*begin).data} ;
}



#endif 