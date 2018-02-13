#ifndef __thodd_extract_lexems_hpp__
#  define __thodd_extract_lexems_hpp__

#  include <vector>
#  include <functional>
#  include <tuple>

#  include "regexes.hpp"


std::vector<lexem> const
extract_lexems (auto begin, auto end, auto rxs_tuple) {
  std::vector<lexem> lexems ;

  while (std::not_equal_to{}(begin, end)) {
    std::vector<std::tuple<decltype(begin), lexem::type_>> cursors ;
    
    std::apply([&] (auto && ... rx) {(cursors.push_back(rx(begin, end)), ...) ;}, rxs_tuple) ;

    auto max = std::max_element (
      cursors.cbegin(), cursors.cend(), 
      [begin] (auto const & lcursor, auto const & rcursor) { 
        return std::get<1>(rcursor) != lexem::type_::error && 
          std::distance(begin, std::get<0>(lcursor)) < std::distance(begin, std::get<0>(rcursor)) ; 
      }) ;
      
      auto max_cursor = begin == std::get<0>(*max) ? 
        std::next(begin) : 
        std::get<0>(*max) ;

    lexems.push_back(lexem{std::get<1>(*max), std::string(begin, max_cursor)}) ;    
    begin = max_cursor ;
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