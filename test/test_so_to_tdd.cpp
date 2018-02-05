#include <iostream>

#include "jasmine.hpp"
#include "../src/so_to_tdd.hpp"


int main() {
  suite("so_to_tdd", [] { 
    thodd_so tdd_so {"prout.txt", std::vector<pod_so>{{"name", std::vector<member_so>{{"prout", "po"}}}, {"name2", std::vector<member_so>{{"proutt", "po"}}}}} ;

    describe("so_to_tdd(tdd_so: thodd_so)",
      it("should return vector of pod", 
        expect(so_to_tdd(tdd_so), [](thodd_tdd && thdd){
          for (auto && pod : thdd.pods)
            for (auto && member : pod.members)
             std::cout << pod.name << "::" << member.name << " : " << member.type << std::endl ; 
          return true ;}))); 
    }) ;

  return EXIT_SUCCESS ;
}