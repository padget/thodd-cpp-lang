#ifndef __thodd_jasmine_hpp__
#  define __thodd_jasmine_hpp__

#  include <string_view>
#  include <string>
#  include <vector>
#  include <iostream>
#  include <algorithm>

auto be_true () {
  return [] (auto && value) {
    return value == true ;
  } ;
}

auto be_false () {
  return [] (auto && value) {
    return value == false ;
  } ;
}

auto equals_to (auto && expected) {
  return [=] (auto && value) {
    return value == expected ;
  } ;
}

auto range_equals_to (auto && expected, auto && comparator) {
  return [=] (auto && value) {
    return std::equal(value.begin(), value.end(), expected.begin(), expected.end(), comparator) ;
  } ;
}

auto expect (auto && value, auto && comparator) {
  return [&] {
    return static_cast<decltype(comparator)&&>(comparator)(
      static_cast<decltype(value)&&>(value)) ;
  } ;
}

auto it (std::string_view description, auto && test) {
  return [=, &test] {
    auto && res = static_cast<decltype(test)&&>(test)() ;
    std::cout << " --> " << description << " : " << (res ? "OK" : "KO /!\\") << std::endl ;
    return res ;
  } ; 
}

void describe (std::string_view description, auto && ... descriptors) {
  std::cout << "----------\n Test : " << description << std::endl ;
  std::vector<bool> results {descriptors()...} ;
  auto nb_passed = std::count(results.begin(), results.end(), true) ;
  std::cout << "    ==> total : "<< nb_passed << "/" << results.size() << std::endl ;
}

void suite (std::string_view suite_name, auto && ... descriptors) {
  std::cout << "\n######### SUITE : " << suite_name << std::endl ; 
  (descriptors(), ...) ;
}

#endif