#ifndef __THODD_LANG_SYNTAX_OR_HPP__
#  define __THODD_LANG_SYNTAX_OR_HPP__

#  include <thodd/lang/syntax/item.hpp>

namespace
thodd::syntax 
{
    template<typename ...>
    struct or_ 
    {
        using syntax_node = or_ ;
    } ;

    constexpr auto
    operator | (
        auto&& __before, 
        auto&& __after)
    requires is_syntax_node<
                decltype(__before), 
                decltype(__after)>
    {
        return 
        or_<
            std::decay_t<decltype(__before)>, 
            std::decay_t<decltype(__after)>> {} ;
    }

    template<typename ... item_t>
    constexpr auto
    operator | (
        or_<item_t...>&& __before, 
        auto&& __after)
    requires is_syntax_node<
                item_t..., 
                decltype(__after)>             
    {
        return 
        or_<
            item_t..., 
            std::decay_t<decltype(__after)>> {} ;
    } 
}

#endif