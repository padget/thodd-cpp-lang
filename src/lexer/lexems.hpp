#ifndef __thodd_lexer_lexems_hpp__
#  define  __thodd_lexer_lexems_hpp__ 

#  include <vector>
#  include <tuple>
#  include <algorithm>

#  include "../stream/utils.hpp"
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

    bool is_not_new_line (auto const & lx) {
      return lx.type != lexem::type_::new_line ;
    }
  }



  container::list<lexem> const
  extract_lexems (auto begin, auto end, auto const & rxs) {
    container::list<lexem> lexems ;

    while (std::not_equal_to{}(begin, end)) {
      auto && cursors     = detail::for_each_rx (begin, end, rxs) ;
      auto && max         = std::max_element (cursors.cbegin(), cursors.cend(), detail::cursor_less_comparator(begin)) ;
      auto && next_cursor = detail::next_if_is_begin(begin, std::get<0>(*max)) ;

      lexems = container::push(lexems, lexem{std::get<1>(*max), std::string(begin, next_cursor)}) ;    
      begin = next_cursor ;
    } 

    return lexems ;
  }

  container::list<lexem> const 
  add_line_location (container::list<lexem> const & lxs) {
    size_t line = 1u ;
    return container::map(lxs, [&line] (lexem const & lx) {
      if (lx.type == lexem::type_::new_line) ++line ;
      return lexem{lx.type, lx.data, line} ;
    }) ;
  }


  container::list<lexem> const 
  filter_lexems (container::list<lexem> const lxs) {
    return container::filter(lxs, [] (lexem const & lx) {
      return detail::is_not_ignored(lx) && detail::is_not_new_line(lx) ;
    }) ;
  }
}

#endif 