#ifndef __THODD_LANG_SYNTAX_GRAMMAR_HPP__
#  define __THODD_LANG_SYNTAX_GRAMMAR_HPP__

#  include <type_traits>
#  include <tuple>

#  include <thodd/tuple/indexof.hpp>

namespace 
thodd::syntax
{
    template <
        typename start_t, 
        typename ... rules_t> 
    struct grammar_rules 
    {
        start_t start;
    } ;

    
    template <
        typename ... declaration_t>
    constexpr auto 
    grammar (
        auto const & __start,
        rule<declaration_t, auto> const & ... __rules)
    {
        return 
        grammar_rules<
            std::decay_t<decltype(__start)>,
            std::decay_t<decltype(__rules)>...> 
            { __start } ;
    } 
    

    template <
        typename ... declaration_t, 
        typename ... definition_t>
    constexpr auto
    get_definition (
        grammar_rules<auto, rule<declaration_t, definition_t>...> const & __grammar, 
        auto && __declaration)
    requires is_syntax_node<std::decay_t<decltype(__declaration)>>
    {
        using namespace ::thodd::tuple ;

        constexpr auto __index = index_of<std::decay_t<decltype(__declaration)>>(std::tuple(declaration_t{}...)) ;
        
        return 
        std::get<__index>(std::tuple(definition_t{}...)) ;
    }    
}   

#endif