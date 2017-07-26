#ifndef __THODD_LANG_SYNTAX_GRAMMAR_HPP__
#  define __THODD_LANG_SYNTAX_GRAMMAR_HPP__

#  include <type_traits>

namespace 
thodd::lang::syntax
{
    template<typename...>
    struct grammar_item : item {} ;

    constexpr auto 
    grammar(
        auto&& ... __rule)
    {
        return 
        grammar_item<
            std::decay_t<decltype(__rule)>... > {} ;
    }
}

#endif