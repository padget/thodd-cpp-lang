#ifndef __THODD_LANG_REGEX_BETWEEN_HPP__
#  define __THODD_LANG_REGEX_BETWEEN_HPP__

#  include <thodd/lang/regex/regex.hpp>
#  include <thodd/lang/regex/char.hpp>

#  include <type_traits>
#  include <utility>

namespace 
thodd::regex 
{
    template<
        typename min_t, 
        typename max_t>
    struct between
    {
        using regex_marker = between ;

        min_t min ;  
        max_t max ;
    } ;


    constexpr auto
    operator - (
        auto&& __min, 
        auto&& __max)
    requires char_based<decltype(__min), decltype(__max)>
    {
        return
        between<
            std::decay_t<decltype(__min)>, 
            std::decay_t<decltype(__max)>> 
        { std::forward<decltype(__min)>(__min), 
          std::forward<decltype(__max)>(__max) } ;
    }
}

#endif 