#ifndef __THODD_LANG_SYNTAX_RULE_HPP__
#  define __THODD_LANG_SYNTAX_RULE_HPP__

#  include <type_traits>

namespace 
thodd::syntax
{
    struct rule {} ;

    template<
        typename ... types_t>
    concept bool 
    rule_based = std::conjunction_v<std::is_base_of<rule, std::decay_t<types_t>>...> ;
}

#endif