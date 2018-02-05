#ifndef __thodd_so_to_tdd_hpp__
#  define __thodd_so_to_tdd_hpp__

#  include "signatures.hpp"
#  include "structure_so.hpp"
#  include "structure_tdd.hpp"

thodd_tdd so_to_tdd (thodd_so so) {
  std::vector<pod_tdd> pods_tdd ;
  std::map<std::string, pod_tdd*> name_to_podptr ;

  for (auto const & pod : so.pods) {
    std::vector<member_tdd> members ;

    for (auto const member : pod.members) {
      members.push_back(member_tdd{member.name}) ;
      member_name_to_type.insert(pod.name + "::" + member.name, &pod) ;
    } 
    
    pods_tdd.push_back(pod_tdd{pod.name, members}) ;
  }

  for (auto & pod : pods)
    for (auto & member : pod.members) {
      auto const && name = pod.name + "::" + member.name ;

      if (name_to_podptr.count(name) == 1)
        member.parent = name_to_podptr.at (name) ;
      else std::cout << "error" << std::endl ; 
    }
  


}

#endif