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
    struct char_
    {
        using regex_marker = char_ ;
        
        char_t c ;
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
}

#endif