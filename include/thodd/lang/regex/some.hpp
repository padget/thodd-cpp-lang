#ifndef __THODD_LANG_REGEX_SOME_HPP__
#  define __THODD_LANG_REGEX_SOME_HPP__

#  include <type_traits>
#  include <utility>

#  include <thodd/lang/regex/regex.hpp>

namespace 
thodd::regex
{
    template<
        typename regex_t>
    struct some : regex
    {
        regex_t reg ;

        mutable std::size_t min {0} ;
        mutable std::size_t max {1} ; 
        
        constexpr some(regex_based&& __reg) :
            reg { std::move(__reg) } {} 

        constexpr some(regex_based const& __reg) :
            reg { __reg } {}

        constexpr some
        operator () (
            std::size_t const& __min, 
            std::size_t const& __max) const
        {
            min = __min ;
            max = __max ;
            
            return 
            *this ;
        }
    } ;


    constexpr auto
    operator + (
        auto&& __regex)
    requires regex_based<decltype(__regex)>
    {   
        return
        some<std::decay_t<decltype(__regex)>>
        { std::forward<decltype(__regex)>(__regex) }(1, 1000000) ;
    }


    constexpr auto
    operator * (
        auto&& __regex) 
    requires regex_based<decltype(__regex)>
    {   
        return
        some<std::decay_t<decltype(__regex)>>
        { std::forward<decltype(__regex)>(__regex) }(1, 1000000) ;
    }

        
    constexpr auto
    operator ~ (
        auto&& __regex)
    requires regex_based<decltype(__regex)>
    {   
        return
        some<std::decay_t<decltype(__regex)>>
        { std::forward<decltype(__regex)>(__regex) } ;
    }


    inline auto 
    matches(
        some<auto> const& __some, 
        auto& __begin, 
        auto const& __end)
    {
        auto __cpt = 0u ;
        auto __save = __begin ;

        while (   __begin != __end 
               && __cpt <= __some.max 
               && matches(__some.reg, __begin, __end)) 
            {++__cpt ;std::cout << "#\n" ;}
        
        return 
        __some.min <= __cpt && __cpt <= __some.max ?
        true : 
        (__begin = __save, false) ;
    }
}


#endif