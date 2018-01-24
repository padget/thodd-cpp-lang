#include "../src/extract_element.hpp"
#include "../src/extract_lexems.hpp"

#include <string>
#include <iostream>

struct test_result {
  std::string name ;
  bool res ;
} ;

test_result 
make_result (bool res, std::string const & name) {
  return {name, res} ;
}

test_result test_has_identifier () {
  std::string const stream = "an_identifier" ;
  auto const & lexems   = extract_lexems(stream.begin(), stream.end(), thodd_rxs()) ;
  auto const & filtered = filter_lexems(lexems.begin(), lexems.end()) ;
  return make_result(has_identifier(filtered.begin(), filtered.end()), "test_has_identifier") ;
}

test_result test_has_number () {
  std::string const stream = "3256.256" ;
  auto const & lexems   = extract_lexems(stream.begin(), stream.end(), thodd_rxs()) ;
  auto const & filtered = filter_lexems(lexems.begin(), lexems.end()) ;
  return make_result(has_number(filtered.begin(), filtered.end()), "test_has_number") ;
}

test_result test_has_function_call () {
  std::string const stream = "add(a, b, 2.3, 2, add(2, 3))" ;
  auto const & lexems   = extract_lexems(stream.begin(), stream.end(), thodd_rxs()) ;
  auto const & filtered = filter_lexems(lexems.begin(), lexems.end()) ;
  return make_result(has_identifier(filtered.begin(), filtered.end()), "test_has_function_call") ;
}

test_result test_has_expression () {
  std::string const stream = "an_identifier" ;
  auto const & lexems   = extract_lexems(stream.begin(), stream.end(), thodd_rxs()) ;
  auto const & filtered = filter_lexems(lexems.begin(), lexems.end()) ;
  return make_result(has_expression(filtered.begin(), filtered.end()), "test_has_expression") ;
}

test_result test_has_instruction () {
  std::string const stream = "an_identifier" ;
  auto const & lexems   = extract_lexems(stream.begin(), stream.end(), thodd_rxs()) ;
  auto const & filtered = filter_lexems(lexems.begin(), lexems.end()) ;
  return make_result(has_expression(filtered.begin(), filtered.end()), "test_has_expression") ;
}

int main() {
  std::vector<std::function<test_result()>> tests {
    {test_has_identifier}, {test_has_number}, {test_has_function_call}, {test_has_expression}
  } ;
  
  auto passed = std::all_of (tests.begin(), tests.end(), [] (auto && test) {
    auto result = test() ;
    std::cout << "----- > " << result.name << " " << (result.res ? "OK :D" : "KO :(") << std::endl ;
    return result.res ;
  }) ;

  if (passed)
    return EXIT_SUCCESS ;
  
  std::cout << "une erreur est survenue" << std::endl ; 
  return EXIT_FAILURE ;
}