#ifndef __THODD_LANG_PURGE_TREE_HPP__
#  define __THODD_LANG_PURGE_TREE_HPP__

#  include <thodd/lang/tree.hpp>
#  include <thodd/lang/grammar.hpp>

namespace
thodd::lang 
{
    template <
        typename language_t, 
        typename ... ids_t>
    bool
    inside (
        language_t const & id, 
        ids_t const & ... ids)
    {
        auto && in = std::array<language_t, sizeof...(ids)> { ids ... } ;

        return
        std::count(in.begin(), in.end(), id) > 0 ; 
    }

    template <
        typename language_t, 
        typename ... ids_t>
    constexpr auto 
    check_ids_types () 
    { static_assert ((std::is_same_v<ids_t, language_t> && ... ), "all ids must have same type") ; }


        
        

    template <
        typename language_t, 
        typename iterator_t, 
        typename ... ids_t>
    void
    purge_tree (
        tree<language_t, iterator_t> & tree_to_purge,
        basic_grammar<language_t> const & grammar, 
        ids_t ... ids_to_purge)
    {
        check_ids_types<language_t, ids_t...> () ;

        std::for_each (
            tree_to_purge.childs.begin(), 
            tree_to_purge.childs.end(), 
            [&grammar, &ids_to_purge...] (auto & child) 
            { purge_tree (child, grammar, ids_to_purge...) ; } ) ;

        // remove terminal to purge
        tree_to_purge.childs.erase (
            std::remove_if (
                tree_to_purge.childs.begin(), 
                tree_to_purge.childs.end(), 
                [&grammar, &ids_to_purge...] (auto const & child) 
                { return is_terminal (child.id, grammar) && inside (child.id, ids_to_purge...) ; } ), 
            tree_to_purge.childs.end()) ;
        
        // remove element that has no childs if not terminal
        tree_to_purge.childs.erase (
            std::remove_if (
                tree_to_purge.childs.begin(), 
                tree_to_purge.childs.end(),
                [&grammar, &ids_to_purge...] (auto const & child) 
                { return !is_terminal (child.id, grammar) && child.childs.empty() ; } ), 
            tree_to_purge.childs.end()) ;
        
        // up elements that has one child if not terminal
        std::for_each (
            tree_to_purge.childs.begin(), 
            tree_to_purge.childs.end(), 
            [&grammar] (auto & child) 
            {
                if (!is_terminal (child.id, grammar) && child.childs.size() == 1)
                    std::swap(child, *child.childs.begin()) ;
            }) ;
     }
}

#endif