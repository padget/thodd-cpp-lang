#ifndef __THODD_LANG_SYNTAX_ITEM_HPP__
#  define __THODD_LANG_SYNTAX_ITEM_HPP__

namespace 
thodd::lang::syntax
{
    struct item {} ;

    template<typename ... type_t>
    concept bool item_based = std::conjunction_v<std::is_base_of<item, std::decay_t<type_t>>...>  ;
} 


#endif 