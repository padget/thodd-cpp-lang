#ifndef __THODD_LANG_CHECK_HPP__
#  define __THODD_LANG_CHECK_HPP__

#  include <thodd/lang/pod.hpp>

namespace 
thodd::lang
{

    
    template <
        auto id_c>
    constexpr auto
    check (
        terminal<id_c, auto>,
        auto const & __grammar,
        auto __cursor, 
        auto const __end)
    {
        return 
        __cursor != __end && (*__cursor).id == id_c ?
        std::tuple { true, ++__cursor } : 
        std::tuple { false, __cursor } ;        
    }

    
    template <
        auto id_c>
    constexpr auto 
    check (
        non_terminal<id_c> const & __nt,
        auto const & __grammar,
        auto __cursor, 
        auto const __end)
    {
        return 
        check (
            extract_definition (__nt, __grammar),
            __grammar, 
            __cursor, 
            __end) ;
    }

    template <
        typename ... item_t>
    constexpr auto
    check (
        and_<item_t...>,
        auto __grammar, 
        auto __cursor, 
        auto __end)
    {
    }

    template <
        typename start_t, 
        typename ... rule_t>
    constexpr auto
    check (
        grammar_rules<start_t, rule_t...> __grammar,
        auto __cursor, 
        auto __end) 
    {
        return 
        check (
            extract_definition(
                meta::start (__grammar), 
                __grammar), 
            __grammar,
            __cursor, 
            __end) ;
    }
}

#endif