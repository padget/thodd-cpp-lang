#ifndef __THODD_LANG_SYNTAX_SOME_HPP__
#  define __THODD_LANG_SYNTAX_SOME_HPP__

#  include <thodd/lang/syntax/rule.hpp>

namespace 
thodd::syntax
{
    template<
        typename rule_t>
    struct some : rule
    {
        rule_t reg ;

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
        auto&& __rule)
    requires rule_based<decltype(__rule)>
    {   
        return
        some<std::decay_t<decltype(__rule)>>
        { std::forward<decltype(__rule)>(__rule) } ;
    }


    constexpr auto
    operator + (
        auto&& __rule)
    requires rule_based<decltype(__rule)>
    {   
        return
        (~std::forward<decltype(__rule)>(__rule))(1, std::numeric_limits<size_t>::max()) ;
    }


    constexpr auto
    operator * (
        auto&& __rule) 
    requires rule_based<decltype(__rule)>
    {   
        return
        (~std::forward<decltype(__rule)>(__rule))(0, std::numeric_limits<size_t>::max()) ;
    }


    template<
        typename rule_t>
    inline auto
    read (
        some<rule_t> const& __some,
        auto & __cursor,
        auto const & __end, 
        auto & __tokens)
    {
        std::decay_t<decltype(__tokens)> __tmp ;

        

        __tokens.insert(__tokens.end(), __tmp.begin(), __tmp.end()) ;
    }
}

#endif