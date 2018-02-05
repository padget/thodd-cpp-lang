#ifndef __thodd_so_to_tdd_hpp__
#  define __thodd_so_to_tdd_hpp__

#  include <map>
#  include <vector>

#  include "signatures.hpp"
#  include "structure_so.hpp"
#  include "structure_tdd.hpp"


member_tdd so_to_tdd(member_so const & so) {
  return member_tdd{so.name.data, so.type.data} ;
}

pod_tdd so_to_tdd(pod_so const & so) {
  std::vector<member_tdd> members ;

  for (auto && m_so : so.members) 
    members.push_back(so_to_tdd(m_so)) ;

  return pod_tdd{so.name.data, members} ;
}

thodd_tdd so_to_tdd (thodd_so const & so) {
  std::vector<pod_tdd> pods ;
  
  for (auto && p_so : so.pods)
    pods.push_back(so_to_tdd(p_so)) ;

  return thodd_tdd{so.filename, pods} ;
}

#endif