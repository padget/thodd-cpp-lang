#include <string>
#include <iostream>
#include <algorithm>
#include <vector>
#include <functional>
#include <tuple>

#include "../src/regexes.hpp"

struct test_result {
  std::string name ;
  bool passed ;
} ;

test_result 
make_result (bool res, std::string const & name) {
  return {name, res} ;
}

test_result test_is_pure_rx () {
  std::string stream = "pure" ;
  auto && res = is_pure_rx(stream.begin(), stream.end()) ;
  return make_result(std::get<0>(res) == stream.end() && std::get<1>(res) == lexem::type_::pure_kw, "test_is_pure_rx") ;
}

test_result test_is_impure_rx () {
  std::string stream = "impure" ;
  auto && res = is_impure_rx(stream.begin(), stream.end()) ;
  return make_result(std::get<0>(res) == stream.end() && std::get<1>(res) == lexem::type_::impure_kw, "test_is_impure_rx") ;
}

test_result test_is_lambda_rx () {
  std::string stream = "lambda" ; 
  auto && res = is_lambda_rx(stream.begin(), stream.end()) ;
  return make_result(std::get<0>(res) == stream.end() && std::get<1>(res) == lexem::type_::lambda_kw, "test_is_lambda_rx") ;
}

test_result test_is_pod_rx () {
  std::string stream = "pod" ; 
  auto && res = is_pod_rx(stream.begin(), stream.end()) ;
  return make_result(std::get<0>(res) == stream.end() && std::get<1>(res) == lexem::type_::pod_kw, "test_is_pod_rx") ;
}

test_result test_is_return_rx () {
  std::string stream = "return" ; 
  auto && res = is_return_rx(stream.begin(), stream.end()) ;
  return make_result(std::get<0>(res) == stream.end() && std::get<1>(res) == lexem::type_::return_kw, "test_is_return_rx") ;
}

test_result test_is_identifier_rx () {
  std::string stream = "identifier" ; 
  auto && res = is_identifier_rx(stream.begin(), stream.end()) ;
  return make_result(std::get<0>(res) == stream.end() && std::get<1>(res) == lexem::type_::identifier, "test_is_identifier_rx") ;
}

test_result test_is_identifiers_rx () {
  std::string stream = "iden.tif.iers" ; 
  auto && res = is_identifiers_rx(stream.begin(), stream.end()) ;
  return make_result(std::get<0>(res) == stream.end() && std::get<1>(res) == lexem::type_::identifiers, "test_is_identifiers_rx") ;
}

test_result test_is_lbracket_rx () {
  std::string stream = "(" ; 
  auto && res = is_lbracket_rx(stream.begin(), stream.end()) ;
  return make_result(std::get<0>(res) == stream.end() && std::get<1>(res) == lexem::type_::lbracket, "test_is_lbracket_rx") ;
}

test_result test_is_rbracket_rx () {
  std::string stream = ")" ; 
  auto && res = is_rbracket_rx(stream.begin(), stream.end()) ;
  return make_result(std::get<0>(res) == stream.end() && std::get<1>(res) == lexem::type_::rbracket, "test_is_rbracket_rx") ;
}

test_result test_is_lbrace_rx () {
  std::string stream = "{" ; 
  auto && res = is_lbrace_rx(stream.begin(), stream.end()) ;
  return make_result(std::get<0>(res) == stream.end() && std::get<1>(res) == lexem::type_::lbrace, "test_is_lbrace_rx") ;
}

test_result test_is_rbrace_rx () {
  std::string stream = "}" ; 
  auto && res = is_rbrace_rx(stream.begin(), stream.end()) ;
  return make_result(std::get<0>(res) == stream.end() && std::get<1>(res) == lexem::type_::rbrace, "test_is_rbrace_rx") ;
}

test_result test_is_colon_rx () {
  std::string stream = ":" ; 
  auto && res = is_colon_rx(stream.begin(), stream.end()) ;
  return make_result(std::get<0>(res) == stream.end() && std::get<1>(res) == lexem::type_::colon, "test_is_colon_rx") ;
}

test_result test_is_semi_colon_rx () {
  std::string stream = ";" ; 
  auto && res = is_semi_colon_rx(stream.begin(), stream.end()) ;
  return make_result(std::get<0>(res) == stream.end() && std::get<1>(res) == lexem::type_::semi_colon, "test_is_semi_colon_rx") ;
}

test_result test_is_comma_rx () {
  std::string stream = "," ; 
  auto && res = is_comma_rx(stream.begin(), stream.end()) ;
  return make_result(std::get<0>(res) == stream.end() && std::get<1>(res) == lexem::type_::comma, "test_is_comma_rx") ;
}

test_result test_is_alias_rx () {
  std::string stream = "#" ; 
  auto && res = is_alias_rx(stream.begin(), stream.end()) ;
  return make_result(std::get<0>(res) == stream.end() && std::get<1>(res) == lexem::type_::alias, "test_is_alias_rx") ;
}

test_result test_is_strengh_rx () {
  std::string stream = "[@]" ; 
  auto && res = is_strengh_rx(stream.begin(), stream.end()) ;
  return make_result(std::get<0>(res) == stream.end() && std::get<1>(res) == lexem::type_::strengh, "test_is_strengh_rx") ;
}

test_result test_is_weak_rx () {
  std::string stream = "@" ; 
  auto && res = is_weak_rx(stream.begin(), stream.end()) ;
  return make_result(std::get<0>(res) == stream.end() && std::get<1>(res) == lexem::type_::weak, "test_is_weak_rx") ;
}

test_result test_is_number_rx () {
  std::string stream = "12.5" ; 
  auto && res = is_number_rx(stream.begin(), stream.end()) ;
  return make_result(std::get<0>(res) == stream.end() && std::get<1>(res) == lexem::type_::number, "test_is_number_rx") ;
}

test_result test_is_ignored_rx () {
  std::string stream = "   \t\t\n" ; 
  auto && res = is_ignored_rx(stream.begin(), stream.end()) ;
  return make_result(std::get<0>(res) == stream.end() && std::get<1>(res) == lexem::type_::ignored, "test_is_ignored_rx") ;
}




int main() {
  std::vector<std::function<test_result()>> tests = {
    test_is_pure_rx, test_is_impure_rx, test_is_lambda_rx, test_is_pod_rx, test_is_return_rx, 
    test_is_identifier_rx, test_is_identifiers_rx, test_is_lbracket_rx, test_is_rbracket_rx, 
    test_is_lbrace_rx, test_is_rbrace_rx, test_is_colon_rx, test_is_semi_colon_rx, test_is_comma_rx, 
    test_is_alias_rx, test_is_strengh_rx, test_is_weak_rx, test_is_number_rx, test_is_ignored_rx
  } ;
  
  bool passed = true ;

  std::for_each (tests.begin(), tests.end(), [&passed] (auto && test) {
    auto && result = test() ;
    std::cout << result.name << (result.passed ? " ---- OK" : " ---- KO /!\\") << std::endl ;
    passed = passed && result.passed ;
  }) ;
  
  if (passed)
    return EXIT_SUCCESS ;
  
  std::cout << "une erreur est survenue" << std::endl ; 
  return EXIT_FAILURE ;
}