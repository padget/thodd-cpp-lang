#ifndef __THODD_LANG_REGEX_AND_HPP__
#  define __THODD_LANG_REGEX_AND_HPP__

#  include <utility>
#  include <tuple>
#  include <type_traits>
#  include <algorithm>
#  include <array>

#  include <thodd/lang/regex/regex.hpp>
#  include <thodd/functional.hpp>

namespace 
thodd::regex
{
    template<
        typename ... regexs_t>
    struct and_ : regex
    {
        std::tuple<regexs_t...> regexs ;

        constexpr and_(
            decltype(regexs) && __regexs):
            regexs { std::forward<decltype(__regexs)>(__regexs) } {}

        constexpr and_(
            decltype(regexs) const & __regexs) :
            regexs { std::move(__regexs) } {}

        constexpr and_(and_ const&) = default ;
        constexpr and_(and_&&) = default ;
        constexpr and_& operator = (and_ const &) = default ;
        constexpr and_& operator = (and_ &&) = default ;
    } ;

    constexpr auto
    operator > (
        auto const & __lregex, 
        auto const & __rregex)
    requires regex_based<decltype(__lregex), decltype(__rregex)>
    {
        return
        and_<
            std::decay_t<decltype(__lregex)>, 
            std::decay_t<decltype(__rregex)>>
        { std::tuple(
            std::forward<decltype(__lregex)>(__lregex), 
            std::forward<decltype(__rregex)>(__rregex)) } ;
    }


    template<
        typename ... regexs_t>
    constexpr auto
    operator > (
        and_<regexs_t...> const & __and,
        auto const & __rregex)
    requires regex_based<decltype(__rregex), regexs_t...>
    {
        return 
        and_<regexs_t..., std::decay_t<decltype(__rregex)>>
        { std::tuple_cat(
            __and.regexs, 
            std::tuple(std::forward<decltype(__rregex)>(__rregex))) } ;
    }


    template<
        typename ... types_t>
    inline auto
    matches(
        and_<types_t...> const& __and,
        auto __cursor, 
        auto const __end)
    {
        auto __results = std::apply(
            [__cursor, __end] (auto && ... __regex)
            { 
                auto __tmp = __cursor ;
                return 
                std::array { (__tmp = matches(std::forward<decltype(__regex)>(__regex), __tmp, __end)) ... } ; } ,
            __and.regexs) ;

        auto __previous = __results.begin() ;

        return 
        std::all_of(
            ++__previous, __results.end(), 
            $0 != *(++ref(__previous))) ? 
            __results.back() : __cursor ; 
    }
}

#endif