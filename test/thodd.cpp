#include <iostream>
#include <string>
#include <regex>
#include <algorithm>


int main(int argc, char** argv) {
  auto stream = std::string ("__add(a:int,b:int):int;");
  auto const identifier_rx = std::regex("^[a-z_]+") ;
  auto const lbracket_rx = std::regex ("^\\(") ;
  auto const colon_rx = std::regex ("^:") ;
  auto const rbracket_rx = std::regex ("^\\)") ;

  auto begin = stream.cbegin() ;
  auto end = stream.cend() ;
  std::smatch matched ; 
  
  while (begin != end) {
    std::vector<std::smatch> matchs ;
    auto rxs = { identifier_rx, lbracket_rx, rbracket_rx, colon_rx } ;

    std::transform (
      rxs.begin(), rxs.end(), std::back_inserter(matchs), 
      [&] (auto && rx) { std::regex_search(begin, end, matched, rx) ; return matched ; }) ;

    constexpr auto match_result_cpr = 
      [] (auto const & l, auto const & r) { 
        return l.str().size() < r.str().size() ; 
      } ;

    auto max = std::max_element (matchs.cbegin(), matchs.cend(), match_result_cpr) ;

    std::advance (begin, (*max).str().size() == 0 ? 1 : (*max).str().size()) ;
    
    if ((*max).str().size() != 0)
      std::cout << (*max).str() << std::endl ;
  }  
}