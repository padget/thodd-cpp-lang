#ifndef __THODD_LANG_SYNTAX_AND_HPP__
#  define __THODD_LANG_SYNTAX_AND_HPP__

#  include <tuple>
#  include <utility>
#  include <type_traits>

#  include <thodd/lang/syntax/rule.hpp>
#  include <thodd/tuple/algorithm.hpp>
#  include <thodd/container.hpp>

namespace 
thodd::syntax
{
    template<
        typename ... rules_t>
    struct and_ : rule
    {
        std::tuple<rules_t...> rules ;

        constexpr and_(
            decltype(rules) && __rules):
            rules { std::forward<decltype(__rules)>(__rules) } {}

        constexpr and_(
            decltype(rules) const & __rules) :
            rules { std::move(__rules) } {}

        constexpr and_(and_ const&) = default ;
        constexpr and_(and_&&) = default ;
        constexpr and_& operator = (and_ const &) = default ;
        constexpr and_& operator = (and_ &&) = default ;
    } ;

      
    constexpr auto
    operator > (
        auto&& __lrule, 
        auto&& __rrule)
    requires rule_based<decltype(__lrule), decltype(__rrule)>
    {
        return
        and_<
            std::decay_t<decltype(__lrule)>, 
            std::decay_t<decltype(__rrule)>>
        { std::tuple(
            std::forward<decltype(__lrule)>(__lrule), 
            std::forward<decltype(__rrule)>(__rrule)) } ;
    }


    template<
        typename ... rules_t>
    constexpr auto
    operator > (
        and_<rules_t...> const& __and,
        auto&& __rrule)
    requires rule_based<decltype(__rrule), rules_t...>
    {
        return 
        and_<rules_t..., std::decay_t<decltype(__rrule)>>
        { std::tuple_cat(
            __and.rules, 
            std::tuple(std::forward<decltype(__rrule)>(__rrule))) } ;
    }


    template<
        typename ... rules_t>
    inline auto
    read (
        and_<rules_t...> const& __and,
        auto & __cursor,
        auto const & __end, 
        auto & __tokens)
    {
        std::decay_t<decltype(__tokens)> __and_tokens ;

        auto __read_next = 
            [&] (auto && __rule)
            { 
                read(std::forward<decltype(__rule)>(__rule), __cursor, __end, __and_tokens) ; 
            } ;
        
        auto __if_last_valid = 
            [&__and_tokens] (auto && ... __any) 
            { 
                return 
                __and_tokens.empty() 
                || (!__and_tokens.empty() 
                    && !__and_tokens.back().invalid()) ; 
            } ;

        ::thodd::tuple::iterate_if(__if_last_valid, __read_next, __and.rules) ;

        if(!__and_tokens.empty() 
            && std::none_of(
                __and_tokens.cbegin(), 
                __and_tokens.cend(), 
                [] (auto && __t) { return __t.invalid() ; }))
            __tokens.insert(
                __tokens.end(),
                __and_tokens.begin(), 
                __and_tokens.end()) ;
        else ;        
    }
}

#endif