#include "jasmine.hpp"

int add (int a, int b) {
  return a + b ;
}

int main () {
  describe("add", 
    it("should return 4 when 2 and 2", expect(add(2, 2), equals_to(4))), 
    it("should return 0 when -1 and 1", expect(add(-1, 1), equals_to(0)))) ;
}