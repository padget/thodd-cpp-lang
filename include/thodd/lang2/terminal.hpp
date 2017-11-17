#ifndef __thodd_lang_terminal_hpp__
#  define __thodd_lang_terminal_hpp__

#  include <thodd/optional.hpp>

#  include <utility>

namespace 
thodd::lang
{
  inline constexpr auto 
  terminal = 
  [] (auto id, auto && rx) 
  {
    return 
    [id, rx] (auto begin, auto const end)
    {
      return
      thodd::make_optional_if(
        rx(begin, end), 
        [] (auto && rx_res) { return has_value(std::forward<decltype(rx_res)>(rx_res)) ; },
        [&] (auto && rx_res) { return make_token(id, begin, std::move(value_of(rx_res))) ; }) ;
    } ;
  } ;
}

#endif