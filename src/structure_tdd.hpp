#ifndef __thodd_structure_tdd_hpp__
#  define __thodd_structure_tdd_hpp__

#  include <vector>
#  include <string>

#  include "signatures.hpp"


struct member_tdd {
  std::string name ;
  pod* parent {nullptr} ;
} ;

struct pod_tdd {
  std::string name ;
  std::vector<member> members ;
} ;

struct thodd_tdd {
  std::string filename ;
  std::vector<pod_tdd> pods ;
}


#endif