#ifndef __THODD_LANG_SYNTAX_RULE_HPP__
#  define __THODD_LANG_SYNTAX_RULE_HPP__

#  include <thodd/lang/syntax/item.hpp>

namespace 
thodd::syntax
{
    template <
        typename declaration_t, 
        typename definition_t>
    struct rule  
    {
        declaration_t declaration ;
        definition_t definition ;
    } ; 


    constexpr auto
    operator <= (
        auto&& __decl, 
        auto&& __production) 
    requires is_syntax_node<
                decltype(__decl), 
                decltype(__production)>
    {
        return 
        rule<
            std::decay_t<decltype(__decl)>, 
            std::decay_t<decltype(__production)>> {} ;
    }
}

#endif