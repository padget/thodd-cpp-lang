#ifndef __THODD_LANG_REGEX_NOT_HPP__
#  define __THODD_LANG_REGEX_NOT_HPP__

#  include <thodd/lang/regex/regex.hpp>
#  include <utility>
#  include <type_traits> 

namespace 
thodd::regex 
{
    template<
        typename regex_t>
    struct not_ : regex
    {
        regex_t reg ;

        constexpr not_(
            decltype(reg) const & __reg) : 
            reg { std::move(__reg) } {}
    } ;

    
    constexpr auto
    operator ! (
        auto&& __regex)
    requires regex_based<decltype(__regex)>
    {
        return
        not_<std::decay_t<decltype(__regex)>>
        { static_cast<decltype(__regex)&&>(__regex) } ; 
    }


    inline auto
    matches(
        not_<auto> const& __not, 
        auto& __cursor, 
        auto const& __end)
    {
        auto __save = __cursor ;
        auto&& __res = ! matches(__not.reg, __cursor, __end) ;
         
        if (__res) __cursor = __save ; 

        return 
        __res ; 
    }
}

#endif