#ifndef __THODD_LANG_INTERPRET_HPP__
#  define __THODD_LANG_INTERPRET_HPP__

#  include <type_traits>
#  include <thodd/lang/tree.hpp>


namespace 
thodd::lang
{
    template <
        typename language_t, 
        typename reactor_t>
    struct basic_rule 
    {
        language_t id ;
        reactor_t reactor ;
    } ;

    constexpr auto
    react (
        auto id, 
        auto reactor) 
    {
        return 
        basic_rule<
            std::decay_t<decltype(id)>, 
            std::decay_t<decltype(reactor)>>
        { id, reactor } ;
    }

    template <
        typename language_t,
        typename iterator_t, 
        typename ... reactor_t>
    void interpret (
        tree<language_t, iterator_t> const & tree_to_interpret,
        basic_rule<language_t, reactor_t> const & ... rule) 
    {
        
    }
}

#endif