#ifndef __THODD_LANG_SYNTAX_AND_HPP__
#  define __THODD_LANG_SYNTAX_AND_HPP__

#  include <tuple>
#  include <utility>
#  include <type_traits>

#  include <thodd/lang/syntax/rule.hpp>

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
        { std::make_tuple(
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
            std::make_tuple(std::forward<decltype(__rrule)>(__rrule))) } ;
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
        std::decay_t<decltype(__tokens)> __tmp ;
        
        auto continue_ifok = [&] (auto&& __rule)
        {
            std::decay_t<decltype(__tokens)> __local ;
            
            read(__rule, __cursor, __end, __local) ;
            
            if (!__local.empty())
                __tmp.insert(__tmp.end(), __local.begin(), __local.end()) ;
            
            return !__local.empty() ;
        } ;

        std::apply(
            [&] (auto&& ... __rule) 
            {   
                return 
                (continue_ifok(__rule) && ...) ; 
            }, 
            __and.rules) ;
    }
}

#endif