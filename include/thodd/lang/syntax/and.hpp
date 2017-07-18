#ifndef __THODD_LANG_SYNTAX_AND_HPP__
#  define __THODD_LANG_SYNTAX_AND_HPP__

#  include <tuple>

namespace 
thodd::syntax
{
    template<
        typename ... regexs_t>
    struct and_ : rule
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
        auto&& __lregex, 
        auto&& __rregex)
    requires rule_based<decltype(__lregex), decltype(__rregex)>
    {
        return
        and_<
            std::decay_t<decltype(__lregex)>, 
            std::decay_t<decltype(__rregex)>>
        { std::make_tuple(
            std::forward<decltype(__lregex)>(__lregex), 
            std::forward<decltype(__rregex)>(__rregex)) } ;
    }


    template<
        typename ... regexs_t>
    constexpr auto
    operator > (
        and_<regexs_t...> const& __and,
        auto&& __rregex)
    requires rule_based<decltype(__rregex), regexs_t...>
    {
        return 
        and_<regexs_t..., std::decay_t<decltype(__rregex)>>
        { std::tuple_cat(
            __and.regexs, 
            std::make_tuple(std::forward<decltype(__rregex)>(__rregex))) } ;
    }
}

#endif