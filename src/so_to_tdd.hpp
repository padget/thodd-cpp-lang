#ifndef __thodd_so_to_tdd_hpp__
#  define __thodd_so_to_tdd_hpp__

#  include "signatures.hpp"
#  include "structure_so.hpp"
#  include "structure_tdd.hpp"

identifier so_to_tdd (identifier_so const & i) {
  return identifier{i.data} ;
}

#endif