#ifndef __THODD_LANG_REGEX_CHAR_HPP__
#  define __THODD_LANG_REGEX_CHAR_HPP__ 

#  include <type_traits>
#  include <utility>

#  include <thodd/lang/regex/regex.hpp>

namespace 
thodd::regex 
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

    namespace detail
    {
        template<typename type_t>
        struct is_char : std::false_type {} ;

        template<typename type_t>
        struct is_char<char_<type_t>> : std::true_type {} ;
    }

    template<
        typename ... types_t>
    concept bool char_based = (detail::is_char<std::decay_t<types_t>>::value && ...) ;

    inline auto 
    matches(
        char_<auto> const& __char,
        auto& __cursor, 
        auto const& __end)
    {
        auto&& __res = __cursor != __end && *__cursor == __char.c ;
       
        if(__res)
            ++__cursor ;
       
        return 
        __res ;
        
    }
}

#endif