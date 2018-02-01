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


/// /////////////// ///
/// Search for true ///
/// /////////////// ///


test_result test_search_for_pure_rx () {
  std::string stream = "pure" ;
  auto && res = search_for_pure_rx(stream.begin(), stream.end()) ;
  return make_result(std::get<0>(res) == stream.end() && std::get<1>(res) == lexem::type_::pure_kw, "test_search_for_pure_rx") ;
}

test_result test_search_for_impure_rx () {
  std::string stream = "impure" ;
  auto && res = search_for_impure_rx(stream.begin(), stream.end()) ;
  return make_result(std::get<0>(res) == stream.end() && std::get<1>(res) == lexem::type_::impure_kw, "test_search_for_impure_rx") ;
}

test_result test_search_for_lambda_rx () {
  std::string stream = "lambda" ; 
  auto && res = search_for_lambda_rx(stream.begin(), stream.end()) ;
  return make_result(std::get<0>(res) == stream.end() && std::get<1>(res) == lexem::type_::lambda_kw, "test_search_for_lambda_rx") ;
}

test_result test_search_for_pod_rx () {
  std::string stream = "pod" ; 
  auto && res = search_for_pod_rx(stream.begin(), stream.end()) ;
  return make_result(std::get<0>(res) == stream.end() && std::get<1>(res) == lexem::type_::pod_kw, "test_search_for_pod_rx") ;
}

test_result test_search_for_return_rx () {
  std::string stream = "return" ; 
  auto && res = search_for_return_rx(stream.begin(), stream.end()) ;
  return make_result(std::get<0>(res) == stream.end() && std::get<1>(res) == lexem::type_::return_kw, "test_search_for_return_rx") ;
}

test_result test_search_for_identifier_rx () {
  std::string stream = "identifier" ; 
  auto && res = search_for_identifier_rx(stream.begin(), stream.end()) ;
  return make_result(std::get<0>(res) == stream.end() && std::get<1>(res) == lexem::type_::identifier, "test_search_for_identifier_rx") ;
}

test_result test_search_for_identifiers_rx () {
  std::string stream = "iden.tif.iers" ; 
  auto && res = search_for_identifiers_rx(stream.begin(), stream.end()) ;
  return make_result(std::get<0>(res) == stream.end() && std::get<1>(res) == lexem::type_::identifiers, "test_search_for_identifiers_rx") ;
}

test_result test_search_for_lbracket_rx () {
  std::string stream = "(" ; 
  auto && res = search_for_lbracket_rx(stream.begin(), stream.end()) ;
  return make_result(std::get<0>(res) == stream.end() && std::get<1>(res) == lexem::type_::lbracket, "test_search_for_lbracket_rx") ;
}

test_result test_search_for_rbracket_rx () {
  std::string stream = ")" ; 
  auto && res = search_for_rbracket_rx(stream.begin(), stream.end()) ;
  return make_result(std::get<0>(res) == stream.end() && std::get<1>(res) == lexem::type_::rbracket, "test_search_for_rbracket_rx") ;
}

test_result test_search_for_lbrace_rx () {
  std::string stream = "{" ; 
  auto && res = search_for_lbrace_rx(stream.begin(), stream.end()) ;
  return make_result(std::get<0>(res) == stream.end() && std::get<1>(res) == lexem::type_::lbrace, "test_search_for_lbrace_rx") ;
}

test_result test_search_for_rbrace_rx () {
  std::string stream = "}" ; 
  auto && res = search_for_rbrace_rx(stream.begin(), stream.end()) ;
  return make_result(std::get<0>(res) == stream.end() && std::get<1>(res) == lexem::type_::rbrace, "test_search_for_rbrace_rx") ;
}

test_result test_search_for_colon_rx () {
  std::string stream = ":" ; 
  auto && res = search_for_colon_rx(stream.begin(), stream.end()) ;
  return make_result(std::get<0>(res) == stream.end() && std::get<1>(res) == lexem::type_::colon, "test_search_for_colon_rx") ;
}

test_result test_search_for_semi_colon_rx () {
  std::string stream = ";" ; 
  auto && res = search_for_semi_colon_rx(stream.begin(), stream.end()) ;
  return make_result(std::get<0>(res) == stream.end() && std::get<1>(res) == lexem::type_::semi_colon, "test_search_for_semi_colon_rx") ;
}

test_result test_search_for_comma_rx () {
  std::string stream = "," ; 
  auto && res = search_for_comma_rx(stream.begin(), stream.end()) ;
  return make_result(std::get<0>(res) == stream.end() && std::get<1>(res) == lexem::type_::comma, "test_search_for_comma_rx") ;
}

test_result test_search_for_alias_rx () {
  std::string stream = "#" ; 
  auto && res = search_for_alias_rx(stream.begin(), stream.end()) ;
  return make_result(std::get<0>(res) == stream.end() && std::get<1>(res) == lexem::type_::alias, "test_search_for_alias_rx") ;
}

test_result test_search_for_strengh_rx () {
  std::string stream = "[@]" ; 
  auto && res = search_for_strengh_rx(stream.begin(), stream.end()) ;
  return make_result(std::get<0>(res) == stream.end() && std::get<1>(res) == lexem::type_::strengh, "test_search_for_strengh_rx") ;
}

test_result test_search_for_weak_rx () {
  std::string stream = "@" ; 
  auto && res = search_for_weak_rx(stream.begin(), stream.end()) ;
  return make_result(std::get<0>(res) == stream.end() && std::get<1>(res) == lexem::type_::weak, "test_search_for_weak_rx") ;
}

test_result test_search_for_number_rx () {
  std::string stream = "12.5" ; 
  auto && res = search_for_number_rx(stream.begin(), stream.end()) ;
  return make_result(std::get<0>(res) == stream.end() && std::get<1>(res) == lexem::type_::number, "test_search_for_number_rx") ;
}

test_result test_search_for_ignored_rx () {
  std::string stream = "   \t\t\n" ; 
  auto && res = search_for_ignored_rx(stream.begin(), stream.end()) ;
  return make_result(std::get<0>(res) == stream.end() && std::get<1>(res) == lexem::type_::ignored, "test_search_for_ignored_rx") ;
}



/// /////////////// ///
/// Search for false ///
/// /////////////// ///


test_result test_search_for_pure_rx_false () {
  std::string stream = "dqskjdlkj" ;
  auto && res = search_for_pure_rx(stream.begin(), stream.end()) ;
  return make_result(std::get<0>(res) != stream.end() && std::get<1>(res) == lexem::type_::error, "test_search_for_pure_rx_false") ;
}

test_result test_search_for_impure_rx_false () {
  std::string stream = "impqsdqure" ;
  auto && res = search_for_impure_rx(stream.begin(), stream.end()) ;
  return make_result(std::get<0>(res) != stream.end() && std::get<1>(res) == lexem::type_::error, "test_search_for_impure_rx_false") ;
}

test_result test_search_for_lambda_rx_false () {
  std::string stream = "laqsdmbda" ; 
  auto && res = search_for_lambda_rx(stream.begin(), stream.end()) ;
  return make_result(std::get<0>(res) != stream.end() && std::get<1>(res) == lexem::type_::error, "test_search_for_lambda_rx_false") ;
}

test_result test_search_for_pod_rx_false () {
  std::string stream = "poqdsqd" ; 
  auto && res = search_for_pod_rx(stream.begin(), stream.end()) ;
  return make_result(std::get<0>(res) != stream.end() && std::get<1>(res) == lexem::type_::error, "test_search_for_pod_rx_false") ;
}

test_result test_search_for_return_rx_false () {
  std::string stream = "qsdre" ; 
  auto && res = search_for_return_rx(stream.begin(), stream.end()) ;
  return make_result(std::get<0>(res) != stream.end() && std::get<1>(res) == lexem::type_::error, "test_search_for_return_rx_false") ;
}

test_result test_search_for_identifier_rx_false () {
  std::string stream = "12_dqksjl" ; 
  auto && res = search_for_identifier_rx(stream.begin(), stream.end()) ;
  return make_result(std::get<0>(res) != stream.end() && std::get<1>(res) == lexem::type_::error, "test_search_for_identifier_rx_false") ;
}

test_result test_search_for_identifiers_rx_false () {
  std::string stream = "1iden.tif.iers" ; 
  auto && res = search_for_identifiers_rx(stream.begin(), stream.end()) ;
  return make_result(std::get<0>(res) != stream.end() && std::get<1>(res) == lexem::type_::error, "test_search_for_identifiers_rx_false") ;
}

test_result test_search_for_lbracket_rx_false () {
  std::string stream = "'(" ; 
  auto && res = search_for_lbracket_rx(stream.begin(), stream.end()) ;
  return make_result(std::get<0>(res) != stream.end() && std::get<1>(res) == lexem::type_::error, "test_search_for_lbracket_rx_false") ;
}

test_result test_search_for_rbracket_rx_false () {
  std::string stream = "')" ; 
  auto && res = search_for_rbracket_rx(stream.begin(), stream.end()) ;
  return make_result(std::get<0>(res) != stream.end() && std::get<1>(res) == lexem::type_::error, "test_search_for_rbracket_rx_false") ;
}

test_result test_search_for_lbrace_rx_false () {
  std::string stream = "'z{" ; 
  auto && res = search_for_lbrace_rx(stream.begin(), stream.end()) ;
  return make_result(std::get<0>(res) != stream.end() && std::get<1>(res) == lexem::type_::error, "test_search_for_lbrace_rx_false") ;
}

test_result test_search_for_rbrace_rx_false () {
  std::string stream = "'}" ; 
  auto && res = search_for_rbrace_rx(stream.begin(), stream.end()) ;
  return make_result(std::get<0>(res) != stream.end() && std::get<1>(res) == lexem::type_::error, "test_search_for_rbrace_rx_false") ;
}

test_result test_search_for_colon_rx_false () {
  std::string stream = "':" ; 
  auto && res = search_for_colon_rx(stream.begin(), stream.end()) ;
  return make_result(std::get<0>(res) != stream.end() && std::get<1>(res) == lexem::type_::error, "test_search_for_colon_rx_false") ;
}

test_result test_search_for_semi_colon_rx_false () {
  std::string stream = "';" ; 
  auto && res = search_for_semi_colon_rx(stream.begin(), stream.end()) ;
  return make_result(std::get<0>(res) != stream.end() && std::get<1>(res) == lexem::type_::error, "test_search_for_semi_colon_rx_false") ;
}

test_result test_search_for_comma_rx_false () {
  std::string stream = "'," ; 
  auto && res = search_for_comma_rx(stream.begin(), stream.end()) ;
  return make_result(std::get<0>(res) != stream.end() && std::get<1>(res) == lexem::type_::error, "test_search_for_comma_rx_false") ;
}

test_result test_search_for_alias_rx_false () {
  std::string stream = "'#" ; 
  auto && res = search_for_alias_rx(stream.begin(), stream.end()) ;
  return make_result(std::get<0>(res) != stream.end() && std::get<1>(res) == lexem::type_::error, "test_search_for_alias_rx_false") ;
}

test_result test_search_for_strengh_rx_false () {
  std::string stream = "'[@]" ; 
  auto && res = search_for_strengh_rx(stream.begin(), stream.end()) ;
  return make_result(std::get<0>(res) != stream.end() && std::get<1>(res) == lexem::type_::error, "test_search_for_strengh_rx_false") ;
}

test_result test_search_for_weak_rx_false () {
  std::string stream = "'@" ; 
  auto && res = search_for_weak_rx(stream.begin(), stream.end()) ;
  return make_result(std::get<0>(res) != stream.end() && std::get<1>(res) == lexem::type_::error, "test_search_for_weak_rx_false") ;
}

test_result test_search_for_number_rx_false () {
  std::string stream = "'12.5" ; 
  auto && res = search_for_number_rx(stream.begin(), stream.end()) ;
  return make_result(std::get<0>(res) != stream.end() && std::get<1>(res) == lexem::type_::error, "test_search_for_number_rx_false") ;
}

test_result test_search_for_ignored_rx_false () {
  std::string stream = "'  \t\t\n" ; 
  auto && res = search_for_ignored_rx(stream.begin(), stream.end()) ;
  return make_result(std::get<0>(res) != stream.end() && std::get<1>(res) == lexem::type_::ignored, "test_search_for_ignored_rx_false") ;
}



int main() {
  std::vector<std::function<test_result()>> tests = {
    test_search_for_pure_rx, test_search_for_impure_rx, test_search_for_lambda_rx, test_search_for_pod_rx, test_search_for_return_rx, 
    test_search_for_identifier_rx, test_search_for_identifiers_rx, test_search_for_lbracket_rx, test_search_for_rbracket_rx, 
    test_search_for_lbrace_rx, test_search_for_rbrace_rx, test_search_for_colon_rx, test_search_for_semi_colon_rx, test_search_for_comma_rx, 
    test_search_for_alias_rx, test_search_for_strengh_rx, test_search_for_weak_rx, test_search_for_number_rx, test_search_for_ignored_rx,

    test_search_for_pure_rx_false, test_search_for_impure_rx_false, test_search_for_lambda_rx_false, test_search_for_pod_rx_false, test_search_for_return_rx_false, 
    test_search_for_identifier_rx_false, test_search_for_identifiers_rx_false, test_search_for_lbracket_rx_false, test_search_for_rbracket_rx_false, 
    test_search_for_lbrace_rx_false, test_search_for_rbrace_rx_false, test_search_for_colon_rx_false, test_search_for_semi_colon_rx_false, test_search_for_comma_rx_false, 
    test_search_for_alias_rx_false, test_search_for_strengh_rx_false, test_search_for_weak_rx_false, test_search_for_number_rx_false, test_search_for_ignored_rx_false  
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