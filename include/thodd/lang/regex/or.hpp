#ifndef __THODD_LANG_REGEX_OR_HPP__
#  define __THODD_LANG_REGEX_OR_HPP__

#  include <tuple>
#  include <type_traits>
#  include <utility>

#  include <thodd/lang/regex/regex.hpp>

namespace 
thodd::lang::regex
{
    template<
        typename ... choices_t>
    struct or_ : regex
    {
        std::tuple<choices_t...> choices ;

        constexpr or_(
            decltype(choices) && __choices):
            choices { std::forward<decltype(__choices)>(choices) } {}

        constexpr or_(
            decltype(choices) const & __choices) :
            choices { std::move(__choices) } {}

        constexpr or_(or_ const&) = default ;
        constexpr or_(or_&&) = default ;
        constexpr or_& operator = (or_ const &) = default ;
        constexpr or_& operator = (or_ &&) = default ;
    } ;


    constexpr auto
    operator | (
        auto&& __lregex, 
        auto&& __rregex)
    requires regex_based(__lregex, __rregex)
    {        
        return
        or_<
            decay_t<decltype(__lregex)>, 
            decay_t<decltype(__rregex)>>
        { std::make_tuple(
            std::forward<decltype(__lregex)>(__lregex), 
            std::forward<decltype(__rregex)>(__rregex)) } ;
    }


    template<
        typename ... lregexs_t>
    constexpr auto
    operator | (
        or_<lregexs_t...> const& __or,
        auto&& __rregex)
    requires regex_based(__rregex, lregexs_t{}...)
    {
        return 
        or_<lregexs_t..., decay_t<decltype(__rregex)>>
        { std::tuple_cat(
            __or.choices, 
            std::make_tuple(std::forward<decltype(__rregex)>(__rregex))) } ;
    }
}

#endif