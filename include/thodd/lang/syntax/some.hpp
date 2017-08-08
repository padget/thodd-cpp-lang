#ifndef __THODD_LANG_SYNTAX_SOME_HPP__
#  define __THODD_LANG_SYNTAX_SOME_HPP__

#  include <limits>

#  include <thodd/lang/syntax/item.hpp>

namespace 
thodd::syntax
{
    template<
        typename node_t>
    struct some 
    { 
        using syntax_node = some ;

        size_t min { 0 } ; 
        size_t max { 1 } ; 
        
        constexpr some() = default ; 
        constexpr some(size_t __min, size_t __max) : min { __min }, max { __max } {}
    } ;


    constexpr auto
    operator + (
        auto&& __after)
    requires is_syntax_node<decltype(__after)>
    {
        return 
        some<std::decay_t<decltype(__after)>> 
        { 1, std::numeric_limits<size_t>::max() } ;
    }

    constexpr auto
    operator * (
        auto&& __after)
    requires is_syntax_node<decltype(__after)>
    {
        return 
        some<std::decay_t<decltype(__after)>> 
        { 0, std::numeric_limits<size_t>::max() } ;
    }
}

#endif