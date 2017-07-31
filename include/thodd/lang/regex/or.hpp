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
        typename ... lregexs_t>
    constexpr auto
    operator | (
        or_<lregexs_t...> const & __or,
        auto const & __rregex)
    requires regex_based<decltype(__rregex), lregexs_t...>
    {
        return 
        or_<lregexs_t..., std::decay_t<decltype(__rregex)>>
        { std::tuple_cat(
            __or.choices, 
            std::tuple(std::forward<decltype(__rregex)>(__rregex))) } ;
    }


        template<
        typename ... types_t>
    inline auto
    matches(
        or_<types_t...> const& __or,
        auto __cursor, 
        auto const __end)
    {
        auto __results = std::apply(
            [=] (auto && ... __choice)
            { return std::array { matches(std::forward<decltype(__choice)>(__choice), __cursor, __end) ... } ; } ,
             __or.choices) ;       

        auto __found = std::find_if(__results.begin(), __results.end(), $0 != val(__cursor)) ;  
   
        return 
        __found != __results.end() && *__found != __cursor ? 
        *__found : __cursor ; 
    }
}

#endif