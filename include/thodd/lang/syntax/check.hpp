#ifndef __THODD_LANG_SYNTAX_CHECK_HPP__
#  define __THODD_LANG_SYNTAX_CHECK_HPP__

#  include <tuple>
#  include <type_traits>

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
        grammar_rules<start_t, rules_t...> const & __grammar, 
        auto __begin, 
        auto const __end)
    {
        return 
        check (
            __grammar, 
            __grammar.start, 
            __begin, __end) ;
    }


    /// LEAF
    template <
        typename start_t, 
        typename ... rules_t,
        auto id_c>
    constexpr auto
    check (
        auto && __grammar,
        leaf<id_c> const & __leaf,
        auto __begin, 
        auto const __end)
    {
        return 
        __begin != __end && *__begin.id == id_c ? 
        std::tuple (true, ++__begin) : 
        std::tuple (false, __begin) ;
    }


    /// NODE
    template <
        typename declaration_t>
    constexpr auto 
    check (
        auto && __grammar, 
        node<declaration_t> && __node,
        auto __begin, 
        auto const __end)
    {
        constexpr auto __def = get_definition(__grammar, __node) ;

        return 
        check (__grammar, __def, __begin, __end) ;
    }

    /// SOME
    template <
        typename item_t>
    constexpr auto 
    check (
        auto && __grammar,
        some<item_t> const & __some, 
        auto __begin, 
        auto const __end)
    {
        constexpr auto __item = item_t {} ;
        auto __cpt = 0u ;
        auto __previous = __begin ;
        bool __continue = true ;
        
        while (__continue)
        { 
            auto&& [__matched, __it] = check (__grammar, __item, __previous, __end) ;
            __continue = __matched ;

            if(__matched) 
            {    
                ++__cpt ;            
                __previous = __it ;
            }
        }

        return 
        std::tuple(
            __some.min <= __cpt && __cpt <= __some.max, 
            __previous) ;

    }

    /// AND 
    template <
        typename item_t>
    constexpr auto 
    check (
        auto && __grammar,
        and_<item_t> const & __some, 
        auto __begin, 
        auto const __end)
    {
        constexpr item_t item {} ;
        
        return
        check(
            __grammar, 
            check (__grammar, item), 
            __begin, __end) ; 

    }

    /// OR
    template <
        typename start_t, 
        typename ... rules_t, 
        typename item_t>
    constexpr auto 
    check (
        auto && __grammar,
        or_<item_t> const & __some, 
        auto __begin, 
        auto const __end)
    {
        constexpr item_t item {} ;
        
        return
        check(
            __grammar, 
            check (__grammar, item), 
            __begin, __end) ; 

    }
} 

#endif