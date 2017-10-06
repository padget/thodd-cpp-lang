#ifndef __THODD_LANG_MATCH_RESULT_HPP__
#  define __THODD_LANG_MATCH_RESULT_HPP__

#  include <utility>

#  include <thodd/functional.hpp>

namespace 
thodd::lang
{
    inline constexpr auto
    match_result = 
    [] (bool const & matched, auto const & cursor)
    { return tuple (matched, cursor) ; } ;
    
    inline constexpr auto
    matched =
    [] (auto && mres)
    -> decltype(auto)
    {
        return
        nth (std::forward<decltype(mres)>(mres), idx0) ;
    } ;
    
    inline constexpr auto
    cursor =
    [] (auto && mres) 
    ->  decltype(auto)
    {
        return
        nth (std::forward<decltype(mres)>(mres), idx1) ;
    } ;
}

#endif