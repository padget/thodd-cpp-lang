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
        { std::forward<decltype(__regex)>(__regex) } ; 
    }
}

#endif