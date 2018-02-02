#  include "../src/extract_element.hpp"
#  include "jasmine.hpp"

int main () {
  suite(
    "extract_element", 
    [] {
      std::vector<lexem> lxs = {lexem{lexem::type_::string, "\"str\""}} ;

      describe("extract_string", 
        it("should extract a string with its content", 
           expect(extract_string(lxs.begin(), lxs.end()).data, equals_to("\"str\"")))) ;
    }) ;
  return EXIT_SUCCESS ;
}