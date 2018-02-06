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
    }, 
    [] {
      std::vector<lexem> lxs = {
        lexem{lexem::type_::identifier, "name"}, 
        lexem{lexem::type_::colon, ":"}, 
        lexem{lexem::type_::identifier, "int"}
      } ;

      describe("extract_member", 
        it("should extract a member with its content", 
           expect(extract_member(lxs.begin(), lxs.end()), [] (auto && mbr) {
             return mbr.name.data == "name" &&
              mbr.type.data == "int" ;
           }))) ;
    }) ;
  return EXIT_SUCCESS ;
}