#ifndef __THODD_LANG_REGEX_OR_HPP__
#  define __THODD_LANG_REGEX_OR_HPP__

#  include <tuple>
#  include <type_traits>
#  include <utility>
#  include <array>
#  include <algorithm>

#  include <thodd/lang/regex/regex.hpp>

namespace 
thodd::regex
{
    template<
        typename ... choices_t>
    struct or_ : regex
    {
        std::tuple<choices_t...> choices ;

        constexpr or_(
            decltype(choices) && __choices):
            choices { std::forward<decltype(__choices)>(__choices) } {}

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
        auto const & __lregex, 
        auto const & __rregex)
    requires regex_based<decltype(__lregex), decltype(__rregex)>
    {        
        return
        or_<
            std::decay_t<decltype(__lregex)>, 
            std::decay_t<decltype(__rregex)>>
        { std::tuple(
            std::forward<decltype(__lregex)>(__lregex), 
            std::forward<decltype(__rregex)>(__rregex)) } ;
    }


    template<
        typename ... regexs_t>
    constexpr auto
    operator | (
        or_<regexs_t...> const & __or,
        auto const & __rregex)
    requires regex_based<decltype(__rregex), regexs_t...>
    {
        return 
        or_<lregexs_t..., std::decay_t<decltype(__rregex)>>
        { std::tuple_cat(
            __or.choices, 
            std::tuple(std::forward<decltype(__rregex)>(__rregex))) } ;
    }
}

#endif