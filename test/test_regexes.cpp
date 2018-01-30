#include <string>
#include <iostream>
#include <algorithm>
#include <vector>
#include <functional>

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
  return make_result(is_pure_rx(stream.begin(), stream.end()) == stream.end(), "test_is_pure_rx") ;
}

test_result test_is_impure_rx () {
  std::string stream = "impure" ;
  return make_result(is_impure_rx(stream.begin(), stream.end()) == stream.end(), "test_is_impure_rx") ;
}

test_result test_is_lambda_rx () {
  std::string stream = "lambda" ;
  return make_result(is_lambda_rx(stream.begin(), stream.end()) == stream.end(), "test_is_lambda_rx") ;
}

test_result test_is_pod_rx () {
  std::string stream = "pod" ;
  return make_result(is_pod_rx(stream.begin(), stream.end()) == stream.end(), "test_is_pod_rx") ;
}

int main() {
  std::vector<std::function<test_result()>> tests = {
    test_is_pure_rx, test_is_impure_rx, test_is_lambda_rx, test_is_pod_rx
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