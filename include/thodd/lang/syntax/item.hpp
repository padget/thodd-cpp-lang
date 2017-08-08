#ifndef __THODD_LANG_SYNTAX_ITEM_HPP__
#  define __THODD_LANG_SYNTAX_ITEM_HPP__

#  include <type_traits>

namespace 
thodd::syntax
{
    template<
        typename type_t>
    concept bool is_syntax_node_one = requires { typename type_t::syntax_node; } ;

    template<
        auto id_c>
    struct leaf 
    { 
        using syntax_node = leaf ;
        static const decltype(id_c) id { id_c } ; 
    } ;    

    template<
        typename declaration_t>
    struct node
    {
        using syntax_node = node ;
    } ;
 
    template <typename ... type_t>
    concept bool is_syntax_node = (is_syntax_node_one<std::decay_t<type_t>> && ...) ;

} 


#endif 