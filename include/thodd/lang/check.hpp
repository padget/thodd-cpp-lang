#ifndef __THODD_LANG_CHECK_HPP__
#  define __THODD_LANG_CHECK_HPP__

#  include <thodd/lang/pod.hpp>

namespace 
thodd::lang
{
    constexpr auto
    check (
        grammar_rules<auto...> const & __grammar,
        auto __cursor, 
        auto const __end) 
    {
        return 
        check (
            __cursor, __end, 
            extract_definition(
                meta::start (__grammar))) ;
    }
    
    template <auto id_c>
    constexpr auto
    check (
        terminal<id_c, auto> const &,
        auto const & __grammar,
        auto __cursor, 
        auto const __end)
    {
        return 
        __cursor != __end && (*__cursor).id == id_c ?
        std::tuple { true, ++__cursor } : 
        std::tuple { false, __cursor } ;        
    }

    
    template <auto id_c>
    constexpr auto 
    check (
        non_terminal<id_c> const &,
        auto const & __grammar,
        auto __curosr, 
        auto const __end)
    {

    }


}

#endif