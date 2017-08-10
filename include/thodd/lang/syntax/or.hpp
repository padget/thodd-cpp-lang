#ifndef __THODD_LANG_SYNTAX_OR_HPP__
#  define __THODD_LANG_SYNTAX_OR_HPP__

#  include <tuple>

#  include <thodd/lang/syntax/item.hpp>

namespace
thodd::syntax 
{
    template<
        typename ... nodes_t>
    struct or_ 
    {
        using syntax_node = or_ ;

        std::tuple<nodes_t...> nodes ;
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
            std::decay_t<decltype(__after)>> 
        { std::tuple(__before, __after) } ;
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
            std::decay_t<decltype(__after)>> 
        { std::tuple_cat(__before, std::tuple(__after)) } ;
    } 
}

#endif