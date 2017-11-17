#ifndef __thodd_lang_token_hpp__
#  define __thodd_lang_token_hpp__

#  include <thodd/range.hpp>

#  include <utility>
#  include <type_traits>

namespace 
thodd::lang 
{
  template <
    typename id_t, 
    typename begin_t, 
    typename end_t>
  struct token
  {
    id_t id ;
    range<begin_t, end_t> data ;
  } ;

  inline auto 
  make_token(
    auto id, 
    auto && begin, 
    auto && end)
  -> decltype(auto) 
  {
    return
    token<std::decay_t<decltype(id)>, 
          std::decay_t<decltype(begin)>, 
          std::decay_t<decltype(end)>> 
    { id, 
      std::forward<decltype(begin)>(begin), 
      std::forward<decltype(end)>(end) } ;
  }
}

#endif