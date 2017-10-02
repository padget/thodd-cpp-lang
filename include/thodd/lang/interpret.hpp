#ifndef __THODD_LANG_INTERPRET_HPP__
#  define __THODD_LANG_INTERPRET_HPP__

#  include <type_traits>
#  include <functional>
#  include <memory>

#  include <thodd/lang/tree.hpp>
#  include <thodd/functional.hpp>

namespace 
thodd::lang
{
    template <
        typename language_t, 
        typename reactor_t>
    struct basic_reactor
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
        basic_reactor<
            std::decay_t<decltype(id)>, 
            std::decay_t<decltype(reactor)>>
        { id, reactor } ;
    }

    template <
        typename basic_t,  
        typename language_t,
        typename iterator_t, 
        typename ... reactor_t>
    void interpret (
        tree<language_t, iterator_t> const & tree_to_interpret,
        basic_reactor<language_t, reactor_t> const & ... reactor) 
    {

       constexpr auto each = fix (
            [] (auto && each, 
                auto const & tree, 
                auto const & first, 
                auto const & ... next)
            {
                if (first.id == tree.id)
                    first.reactor (tree.data.first, tree.data.second) ;
                else if constexpr (sizeof...(next) >= 1)
                    each (tree, next...) ;
            });

        for(auto const & child : tree_to_interpret.childs)
            interpret <basic_t> (child, reactor...) ;

        if (tree_to_interpret.childs.empty()) 
           each (tree_to_interpret, reactor...) ;
    }
}

#endif