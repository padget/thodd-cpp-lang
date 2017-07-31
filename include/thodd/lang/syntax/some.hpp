#ifndef __THODD_LANG_SYNTAX_SOME_HPP__
#  define __THODD_LANG_SYNTAX_SOME_HPP__

#  include <thodd/lang/syntax/item.hpp>

namespace 
thodd::syntax
{


    template<typename>
    struct some : item { size_t min{0}, max{1} ; } ;

    constexpr auto
    operator + (
        auto&& __after)
    requires item_based<decltype(__after)>
    {
        return 
        some<std::decay_t<decltype(__after)>> {} ;
    }

    constexpr auto
    operator * (
        auto&& __after)
    requires item_based<decltype(__after)>
    {
        return 
        some<std::decay_t<decltype(__after)>> {} ;
    }
}

#endif