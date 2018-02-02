#include <string>
#include <iostream>
#include <algorithm>
#include <vector>
#include <functional>
#include <tuple>

#include "../src/start_with.hpp"
#include "../src/regexes.hpp"
#include "../src/extract_lexems.hpp"

#include "jasmine.hpp"


struct test_result {
  std::string name ;
  bool passed ;
} ;

test_result 
make_result (bool res, std::string const & name) {
  return {name, res} ;
}

using thd = lexem::type_ ;

std::vector<lexem>
make_lexems(std::vector<thd> const & thds) {
  std::vector<lexem> lxms ;
  std::transform(
    thds.begin(), thds.end(), 
    std::back_inserter(lxms), 
    [] (thd const & th) {return lexem{th} ;}) ;
  
  return lxms ;
}


test_result test_extract_lexems () {
  std::string stream = "int aaaa 2.3 dqd22" ;
  auto const && lexems = extract_lexems(stream.begin(), stream.end(), thodd_rxs(0)) ;
  auto const && expected = make_lexems({
    thd::identifier, thd::ignored, thd::identifier, thd::ignored, 
    thd::number, thd::ignored, thd::identifier, thd::number
  }) ;

  return make_result(
    lexems.size() == expected.size() && 
    start_with(
      lexems.begin(), lexems.end(), 
      expected.begin(), expected.end(), 
      [] (auto && l, auto && r) {return l.type == r.type ;}), 
    "test_extract_lexems") ;
}

test_result test_extract_lexems_false () {
  std::string stream = "int aaaa 2.3 dqd2d qdq dqd qdq sd q dq2" ;
  auto const && lexems = extract_lexems(stream.begin(), stream.end(), thodd_rxs(0)) ;
  auto const && expected = make_lexems({
    thd::identifier, thd::ignored, thd::identifier, thd::ignored, 
    thd::number, thd::ignored, thd::identifier, thd::number
  }) ;

  return make_result(
    lexems.size() != expected.size() && 
    start_with(
      lexems.begin(), lexems.end(), 
      expected.begin(), expected.end(), 
      [] (auto && l, auto && r) {return l.type == r.type ;}), 
    "test_extract_lexems_false") ;
}



int main() {
  std::vector<std::function<test_result()>> tests = {
    test_extract_lexems, test_extract_lexems_false
  } ;
  
  bool passed = true ;

  std::for_each (tests.begin(), tests.end(), [&passed] (auto && test) {
    auto && result = test() ;
    std::cout << result.name << (result.passed ? " ---- OK" : " ---- KO /!\\") << std::endl ;
    passed = passed && result.passed ;
  }) ;


  std::string stream = "int aaaa 2.3 dqd2" ;
  describe("extract_lexems", 
    it("should return the right lexems sequence", 
      expect(
        extract_lexems(stream.begin(), stream.end(), thodd_rxs(0)), 
        range_equals_to(
          make_lexems({
            thd::identifier, thd::ignored, thd::identifier, thd::ignored, 
            thd::number, thd::ignored, thd::identifier, thd::number
          }), 
          [] (auto && l, auto && r) {return l.type == r.type ;})))) ;
  
  if (passed)
    return EXIT_SUCCESS ;
  
  std::cout << "une erreur est survenue" << std::endl ; 
  return EXIT_FAILURE ;
}