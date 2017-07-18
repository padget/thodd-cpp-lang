#ifndef __THODD_LANG_SYNTAX_SOME_HPP__
#  define __THODD_LANG_SYNTAX_SOME_HPP__

#  include <thodd/lang/syntax/rule.hpp>

namespace 
thodd::syntax
{
    template<
        typename regex_t>
    struct some : rule
    {
        regex_t reg ;

        std::size_t min {0} ;
        std::size_t max {1} ; 
        
        constexpr some(rule_based&& __reg) :
            reg { std::move(__reg) } {} 

        constexpr some(rule_based const& __reg) :
            reg { __reg } {}

        constexpr some(
            rule_based&& __reg, 
            size_t const& __min, 
            size_t const& __max) :
            reg { std::move(__reg) }, 
            min { __min }, 
            max { __max } {} 

        constexpr some(
            rule_based const& __reg, 
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
    requires rule_based<decltype(__regex)>
    {   
        return
        some<std::decay_t<decltype(__regex)>>
        { std::forward<decltype(__regex)>(__regex) } ;
    }


    constexpr auto
    operator + (
        auto&& __regex)
    requires rule_based<decltype(__regex)>
    {   
        return
        (~std::forward<decltype(__regex)>(__regex))(1, std::numeric_limits<size_t>::max()) ;
    }


    constexpr auto
    operator * (
        auto&& __regex) 
    requires rule_based<decltype(__regex)>
    {   
        return
        (~std::forward<decltype(__regex)>(__regex))(0, std::numeric_limits<size_t>::max()) ;
    }

}

#endif