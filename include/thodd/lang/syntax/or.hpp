#ifndef __THODD_LANG_SYNTAX_OR_HPP__
#  define __THODD_LANG_SYNTAX_OR_HPP__

#  include <tuple>
#  include <type_traits>
#  include <utility>

#  include <thodd/lang/syntax/rule.hpp>

namespace 
thodd::syntax
{
    template<
        typename ... choices_t>
    struct or_ : rule
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
        auto && __lregex, 
        auto && __rregex)
    requires rule_based<
                decltype(__lregex), 
                decltype(__rregex)>
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
        typename ... choices_t>
    constexpr auto
    operator | (
        or_<choices_t...> const & __or, 
        auto && __rregex)
    requires rule_based<decltype(__rregex), choices_t...>
    {
        return 
        or_<choices_t..., std::decay_t<decltype(__rregex)>>
        { std::tuple_cat(
            __or.choices,
            std::tuple(std::forward<decltype(__rregex)>(__rregex))) } ;
    }


    template<
        typename ... choices_t>
    inline auto
    read (
        or_<choices_t...> const& __or,
        auto & __cursor,
        auto const & __end, 
        auto & __tokens)
    {
        std::decay_t<decltype(__tokens)> __tmp ;
        
        std::apply(
            [&] (auto&& ... __choices) 
            { 
                (read(__choices, __cursor, __end, __tmp),...) ; 
            }, 
            __or.choices) ;

        if(sizeof...(choices_t) == __tmp.size())
            __tokens.insert(__tokens.end(), __tmp.begin(), __tmp.end()) ;
    }
}

#endif