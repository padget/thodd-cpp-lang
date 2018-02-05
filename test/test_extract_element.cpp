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
    }, 
    [] {
      std::vector<lexem> lxs = {lexem{lexem::type_::identifier, "idenlkjn"}} ;

      describe("extract_identifier", 
        it("should extract an identifier with its content", 
           expect(extract_identifier(lxs.begin(), lxs.end()).data, equals_to("idenlkjn")))) ;
    }, 
    [] {
      std::vector<lexem> lxs = {lexem{lexem::type_::string, "12.5"}} ;

      describe("extract_number", 
        it("should extract a number with its content", 
           expect(extract_number(lxs.begin(), lxs.end()).data, equals_to("12.5")))) ;
    }) ;
  return EXIT_SUCCESS ;
}