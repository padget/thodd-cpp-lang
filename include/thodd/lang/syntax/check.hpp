#ifndef __THODD_LANG_SYNTAX_CHECK_HPP__
#  define __THODD_LANG_SYNTAX_CHECK_HPP__

#  include <tuple>
#  include <array>
#  include <type_traits>
#  include <algorithm>

#  include <thodd/lang/syntax/grammar.hpp>
#  include <thodd/lang/syntax/item.hpp>
#  include <thodd/lang/syntax/some.hpp>
#  include <thodd/lang/syntax/and.hpp>
#  include <thodd/lang/syntax/or.hpp>

namespace  
thodd::syntax 
{
    template <
        typename start_t, 
        typename ... rules_t>
    constexpr auto
    check (
        auto __begin, 
        auto const __end, 
        grammar_rules<start_t, rules_t...> const & __grammar)
    {
        return 
        check (
            __begin, __end, 
            __grammar, 
            __grammar.start) ;
    }


    /// LEAF
    template <
        auto id_c>
    constexpr auto
    check (
        auto __begin, 
        auto const __end,
        auto && __grammar,
        leaf<id_c> const & __leaf)
    {
        return 
        __begin != __end && (*__begin).id == id_c ? 
        std::tuple (true, ++__begin) : 
        std::tuple (false, __begin) ;
    }


    /// NODE
    template <
        typename declaration_t>
    constexpr auto 
    check (
        auto __begin, 
        auto const __end,
        auto && __grammar, 
        node<declaration_t> const & __node)
    {
        constexpr auto __def = get_definition(__grammar, declaration_t{}) ;

        return 
        check (__begin, __end, __grammar, __def) ;
    }

    /// SOME
    template <
        typename item_t>
    constexpr auto 
    check (
        auto __begin, 
        auto const __end,
        auto && __grammar,
        some<item_t> const & __some)
    {
        auto __cpt = 0u ;
        auto __previous = __begin ;
        bool __continue = true ;
        
        while (__continue)
        { 
            auto&& [__matched, __it] = check (__previous, __end, __grammar, __some.node) ;
            __continue = __matched ;

            if(__matched) 
            { ++__cpt ; __previous = __it ; }
        }

        return 
        std::tuple(
            __some.min <= __cpt && __cpt <= __some.max, 
            __previous) ;

    }

    /// AND 
    template <
        typename ... nodes_t>
    constexpr auto 
    check (
        auto __begin, 
        auto const __end,
        auto && __grammar,
        and_<nodes_t...> const & __and)
    {       
        auto&& __res = 
            std::apply(
                [&] (auto const & ... __node) 
                { return std::tuple(check(__begin, __end, __grammar, __node)...) ; }, 
                __and.nodes) ;
    }

    /// OR
    template <
        typename ... nodes_t>
    constexpr auto 
    check ( 
        auto __begin, 
        auto const __end,
        auto && __grammar,
        or_<nodes_t...> const & __or)
    {
        auto && __res = 
            std::apply (
                [&] (auto const & ... __node) 
                { return std::array(check(__begin, __end, __grammar, __node)...) ; }, 
                __or.nodes) ;

        return
        std::tuple(true, __begin) ;
    }
} 

#endif