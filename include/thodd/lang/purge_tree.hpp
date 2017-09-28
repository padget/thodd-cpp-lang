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
        auto && in = std::array { ids ... } ;

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
    purge_tree_some (
        tree<language_t, iterator_t> const & tree_to_purge,
        basic_grammar<language_t> const & grammar, 
        ids_t ... ids_to_purge)
    {
        check_ids_types<language_t, ids_t...> () ;
    
        if (inside(tree_to_purge.id, ids_to_purge...)) 
        {
            // if some empty => suppress ;
            // if some single => do same as or purge ;
        }
}
        
    template <
        typename language_t, 
        typename iterator_t, 
        typename ... ids_t>
    purge_tree_and (
        tree<language_t, iterator_t> const & tree_to_purge,
        basic_grammar<language_t> const & grammar, 
        ids_t ... ids_to_purge)
    {
        check_ids_types<language_t, ids_t...> () ;

         if (inside(tree_to_purge.id, ids_to_purge...)) 
         {
            // do nothing
         }
    }
        
    template <
        typename language_t, 
        typename iterator_t, 
        typename ... ids_t>
    purge_tree_or (
        tree<language_t, iterator_t> const & tree_to_purge,
        basic_grammar<language_t> const & grammar, 
        ids_t ... ids_to_purge)
    {
        check_ids_types<language_t, ids_t...> () ;
        
        if (inside(tree_to_purge.id, ids_to_purge...)) 
        {
            // escape or and up the child directly ;
        }
    }
        

    template <
        typename language_t, 
        typename iterator_t, 
        typename ... ids_t>
    purge_tree (
        tree<language_t, iterator_t> const & tree_to_purge,
        basic_grammar<language_t> const & grammar, 
        ids_t ... ids_to_purge)
    {
        check_ids_types<language_t, ids_t...> () ;

        // bottom -> up purge
    }
}

#endif