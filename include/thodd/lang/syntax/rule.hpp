#ifndef __THODD_LANG_SYNTAX_RULE_HPP__
#  define __THODD_LANG_SYNTAX_RULE_HPP__

#  include <thodd/lang/syntax/item.hpp>

namespace 
thodd::lang::syntax
{
    template <typename, typename>
    struct rule : item {} ; 


    constexpr auto
    operator <= (
        auto&& __def, 
        auto&& __production) 
    requires item_based<
                decltype(__def), 
                decltype(__production)>
    {
        return 
        rule<
            std::decay_t<decltype(__def)>, 
            std::decay_t<decltype(__production)>> {} ;
    }
}

#endif