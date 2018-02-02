#  include "../src/extract_element.hpp"
#  include "jasmine.hpp"

auto equals_to (string const & expected) {
  return [=] (string const & value) {
    return value.data == expected.data ;
  } ;
}

int main () {
  suite(
    "extract_element", 
    [] {
      std::vector<lexem> lxs = {lexem{lexem::type_::string, "\"str\""}} ;

      describe("extract_string", 
        it("should extract a string with its content", 
           expect(extract_string(lxs.begin(), lxs.end()), [] (auto && value) {return value.data == "\"str\"" ;}))) ;
    }) ;
  return EXIT_SUCCESS ;
}