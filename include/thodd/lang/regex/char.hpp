#ifndef __THODD_LANG_REGEX_CHAR_HPP__
#  define __THODD_LANG_REGEX_CHAR_HPP__ 

#  include <type_traits>
#  include <utility>

#  include <thodd/lang/regex/regex.hpp>

namespace 
thodd::lang::regex 
{
    template<
        typename char_t>
    struct char_ : regex
    {
        char_t c ;

        constexpr char_(
            decltype(c) && __c) :
            c { std::move(__c) } {}

        constexpr char_(
            decltype(c) const& __c) :
            c { __c } {}

        constexpr char_(char_ const&) = default ;
        constexpr char_(char_&&) = default ;
        constexpr char_& operator = (char_ const&) = default ;
        constexpr char_& operator = (char_&&) = default ;
    } ;


    constexpr auto
    chr(auto&& __c)
    {        
        return 
        char_<std::decay_t<decltype(__c)>>
        { std::forward<decltype(__c)>(__c) } ;
    }

    namespace 
    detail
    {
        constexpr auto
        is_char(
            auto const&)
        {
            return false ;   
        }


        constexpr auto
        is_char(
            char_<auto> const&)
        {
            return true ;   
        }
    }

    template<
        typename ... types_t>
    concept bool char_based(types_t && ... __t) 
    {
        return  
        detail::is_char(__t) && ... ;
    }

}

#endif