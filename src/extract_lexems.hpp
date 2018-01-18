#ifndef __thodd_extract_lexems_hpp__
#  define __thodd_extract_lexems_hpp__

#  include <vector>
#  include <functional>
#  include <regex>

#  include "regexes.hpp"

std::vector<lexem> const
extract_lexems (auto begin, auto end, std::vector<rx> rxs) {
  std::smatch matched ; 
  std::vector<lexem> lexems ;

  while (std::not_equal_to{}(begin, end)) {
    std::vector<lexem> matchs ;
    
    for (auto const & rx : rxs) {
      std::regex_search(begin, end, matched, rx.reg) ;
      matchs.push_back(lexem{rx.type, matched.str()}) ;
    }

    auto max = std::max_element (
      matchs.cbegin(), matchs.cend(), 
      [] (auto const & l, auto const & r) { 
        return l.data.size() < r.data.size() ; 
      }) ;
      
    auto const & max_size = (*max).data.size() ;
    begin = std::next (begin, max_size == 0 ? 1 : max_size) ;
    
    if (max_size != 0)
      lexems.push_back(*max) ;
  } 

  return lexems ;
}

std::vector<lexem>
filter_lexems (auto begin, auto end) {
  std::vector<lexem> filtered ;
  std::copy_if(
    begin, end, std::back_inserter(filtered), 
    [] (auto const & lexem) { 
      return lexem.type != lexem::type_::ignored ; 
    }) ;
  return filtered ;
}

std::vector<lexem>
copy_lexems (auto begin, auto end) {
  std::vector<lexem> lexems ;
  std::copy (begin, end, std::back_inserter(lexems)) ;
  return lexems ;
}



#endif