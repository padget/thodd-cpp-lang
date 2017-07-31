#ifndef __THODD_LANG_REGEX_SOME_HPP__
#  define __THODD_LANG_REGEX_SOME_HPP__

#  include <type_traits>
#  include <utility>
#  include <limits>

#  include <thodd/lang/regex/regex.hpp>

namespace 
thodd::regex
{
    template<
        typename regex_t>
    struct some : regex
    {
        regex_t reg ;

        std::size_t min {0} ;
        std::size_t max {1} ; 
        
        constexpr some(regex_based&& __reg) :
            reg { std::move(__reg) } {} 

        constexpr some(regex_based const& __reg) :
            reg { __reg } {}

        constexpr some(
            regex_based&& __reg, 
            size_t const& __min, 
            size_t const& __max) :
            reg { std::move(__reg) }, 
            min { __min }, 
            max { __max } {} 

        constexpr some(
            regex_based const& __reg, 
            size_t const& __min, 
            size_t const& __max) :
            reg { __reg }, 
            min { __min }, 
            max { __max } {}

        constexpr some
        operator () (
            std::size_t const& __min, 
            std::size_t const& __max) const
        {
            return 
            { reg, __min, __max } ;
        }
    } ;


    constexpr auto
    operator ~ (
        auto&& __regex)
    requires regex_based<decltype(__regex)>
    {   
        return
        some<std::decay_t<decltype(__regex)>>
        { std::forward<decltype(__regex)>(__regex) } ;
    }


    constexpr auto
    operator + (
        auto&& __regex)
    requires regex_based<decltype(__regex)>
    {   
        return
        (~std::forward<decltype(__regex)>(__regex))(1, std::numeric_limits<size_t>::max()) ;
    }


    constexpr auto
    operator * (
        auto&& __regex) 
    requires regex_based<decltype(__regex)>
    {   
        return
        (~std::forward<decltype(__regex)>(__regex))(0, std::numeric_limits<size_t>::max()) ;
    }

    inline auto 
    matches(
        some<auto> const& __some, 
        auto __cursor, 
        auto const __end)
    {
        auto __cpt = 0u ;
        auto __previous = __cursor ;
        auto __initial = __cursor ;

        while (__cpt <= __some.max 
               && (__cursor = matches(__some.reg, __cursor, __end)) != __previous)
        { ++__cpt ; __previous = __cursor ; }

        return 
        __some.min <= __cpt && __cpt <= __some.max ? 
        __cursor : __initial ; 
    }
}


#endif