#ifndef __thodd_lexer_lexems_hpp__
#  define  __thodd_lexer_lexems_hpp__ 

#  include <vector>
#  include <tuple>
#  include <algorithm>

#  include "lexem.hpp"

namespace thodd::lexer {
  namespace detail {
    auto cursor_less_comparator (auto begin) {
      return [begin] (auto const & lcursor, auto const & rcursor) { 
        return std::get<1>(rcursor) != lexem::type_::error && 
          std::distance(begin, std::get<0>(lcursor)) < std::distance(begin, std::get<0>(rcursor)) ; 
      } ; 
    }

    auto next_if_is_begin (auto const & begin, auto const & cursor) {
      return begin == cursor ? std::next(begin) : cursor ;
    }

    auto for_each_rx (auto begin, auto end, auto const & rxs) {
      std::vector<std::tuple<decltype(begin), lexem::type_>> cursors ;
      std::apply([&] (auto && ... rx) {(cursors.push_back(rx(begin, end)), ...) ;}, rxs) ;
      return cursors ; 
    }

    bool is_not_ignored (auto const & lx) {
      return lx.type != lexem::type_::ignored ;
    }
  }

  std::vector<lexem> const
  extract_lexems (auto begin, auto end, auto const & rxs) {
    std::vector<lexem> lexems ;

    while (std::not_equal_to{}(begin, end)) {
      auto && cursors     = detail::for_each_rx (begin, end, rxs) ;
      auto && max         = std::max_element (cursors.cbegin(), cursors.cend(), detail::cursor_less_comparator(begin)) ;
      auto && next_cursor = detail::next_if_is_begin(begin, std::get<0>(*max)) ;

      lexems.push_back(lexem{std::get<1>(*max), std::string(begin, next_cursor)}) ;    
      begin = next_cursor ;
    } 

    return lexems ;
  }

  std::vector<lexem>
  filter_lexems (auto begin, auto end) {
    std::vector<lexem> filtered ; 
    std::copy_if(begin, end, std::back_inserter(filtered), [] (lexem const & lx) {return detail::is_not_ignored(lx) ;}) ;
    return filtered ;
  }

  std::vector<lexem>
  copy_lexems (auto begin, auto end) {
    std::vector<lexem> lexems ;
    std::copy (begin, end, std::back_inserter(lexems)) ;
    return lexems ;
  }
}

#endif 