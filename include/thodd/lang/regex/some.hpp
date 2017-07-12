#ifndef __THODD_LANG_REGEX_SOME_HPP__
#  define __THODD_LANG_REGEX_SOME_HPP__

#  include <type_traits>
#  include <utility>

#  include <thodd/lang/regex/regex.hpp>

namespace 
thodd::lang::regex
{
    template<
        typename regex_t>
    struct some : regex
    {
        regex_t reg ;

        mutable std::size_t min {0} ;
        mutable std::size_t max {1} ; 
        
        constexpr some(regex_based&& __reg) :
            reg { rvalue(__reg) } {} 

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
    {   
        return
        some<std::decay_t<decltype(__regex)>>
        { std::forward<decltype(__regex)>(__regex) }(1, infinity) ;
    }


    constexpr auto
    operator * (
        auto&& __regex) 
    requires regex_based<decltype(__regex)>
    {   
        return
        some<std::decay_t<decltype(__regex)>>
        { std::forward<decltype(__regex)>(__regex) }(1, infinity) ;
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
}


#endif