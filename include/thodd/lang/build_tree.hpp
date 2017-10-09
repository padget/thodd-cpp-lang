#ifndef __THODD_LANG_BUILD_TREE_HPP__
#  define __THODD_LANG_BUILD_TREE_HPP__

#  include <list>
#  include <tuple>
#  include <utility>
#  include <iterator>

#  include <thodd/lang/grammar.hpp>
#  include <thodd/lang/tree.hpp>

namespace 
thodd::lang 
{
    /*template <
        typename iterator_t>
    using data_iterator_t = 
        typename std::iterator_traits<iterator_t>::value_type::data_iterator ;


    template <
        typename language_t, 
        typename iterator_t>
    std::tuple<tree<language_t, data_iterator_t<iterator_t>>, iterator_t>
    build_tree_some (
        language_t some_id, 
        basic_grammar<language_t> const & grammar, 
        iterator_t begin, 
        iterator_t const end)
    {
        auto cpt = 0u ;
        auto checked = true ;
        auto local_cursor = begin ; 

        language_t step_id = *grammar.dictionary.at(some_id).ids.begin() ;

        size_t const min = grammar.dictionary.at(some_id).min ;
        size_t const max = grammar.dictionary.at(some_id).max ;

        std::list<tree<language_t, data_iterator_t<iterator_t>>> local_childs ;

        while (checked && cpt < max)
        {
            auto && [step_tree, step_cursor] = build_tree (step_id, grammar, local_cursor, end) ;

            if (checked = (step_tree.id != language_t::error))
            {
                local_childs.push_back (std::move(step_tree)) ;
                ++ cpt ;
                local_cursor = step_cursor ;
            } 
        }

        checked = min <= cpt && cpt <= max ;

        auto min_begin = local_childs.empty() ? (*begin).data.first : local_childs.front().data.first ;
        auto max_end = local_childs.empty() ? (*begin).data.first : local_childs.back().data.second ;
        
        return 
        { { checked ? some_id : language_t::error,  
            { min_begin, max_end }, 
            local_childs }, 
          checked ? local_cursor : begin };
    }


    template <
        typename language_t, 
        typename iterator_t>
    std::tuple<tree<language_t, data_iterator_t<iterator_t>>, iterator_t>
    build_tree_and (
        language_t and_id, 
        basic_grammar<language_t> const & grammar, 
        iterator_t begin, 
        iterator_t const end)
    {
        auto local_cursor = begin ; 
        auto checked = true ;
        auto const & step_ids = grammar.dictionary.at(and_id).ids ;

        std::list<tree<language_t, data_iterator_t<iterator_t>>> local_childs ;

        for (auto && step_id : step_ids)
        {
            auto && [step_tree, step_cursor] = build_tree (step_id, grammar, local_cursor, end) ;

            if (checked = (step_tree.id != language_t::error)) 
            {
                local_childs.push_back (std::move(step_tree)) ;
                local_cursor = step_cursor ; 
            }
            else break ;
        }  
        
        auto min_begin = local_childs.empty() ? (*begin).data.first : local_childs.front().data.first ;
        auto max_end = local_childs.empty() ? (*begin).data.first : local_childs.back().data.second ;
        
        return 
        { { checked ? and_id : language_t::error,  
            { min_begin, max_end }, 
            local_childs }, 
          checked ? local_cursor : begin };
    }


    template <
        typename language_t, 
        typename iterator_t>
    std::tuple<tree<language_t, data_iterator_t<iterator_t>>, iterator_t>
    build_tree_or (
        language_t or_id, 
        basic_grammar<language_t> const & grammar, 
        iterator_t begin, 
        iterator_t const end)
    {
        auto local_cursor = begin ; 
        auto checked = true ;
        auto const & step_ids = grammar.dictionary.at(or_id).ids ;

        std::list<tree<language_t, data_iterator_t<iterator_t>>> local_childs ;

        for (auto && step_id : step_ids)
        {
            auto && [step_tree, step_cursor] = build_tree (step_id, grammar, local_cursor, end) ;

            if (checked = (step_tree.id != language_t::error)) 
            {
                local_childs.push_back (std::move(step_tree)) ;
                local_cursor = step_cursor ; 
                break ;
            }
        }  
        
        auto min_begin = local_childs.empty() ? (*begin).data.first : local_childs.front().data.first ;
        auto max_end = local_childs.empty() ? (*begin).data.first : local_childs.back().data.second ;
        
        return 
        { { checked ? or_id : language_t::error,  
            { min_begin, max_end }, 
            local_childs }, 
          checked ? local_cursor : begin };
    }

    
    template <
        typename language_t, 
        typename iterator_t>
    std::tuple<tree<language_t, data_iterator_t<iterator_t>>, iterator_t>
    build_tree (
        language_t id,
        basic_grammar<language_t> const & grammar, 
        iterator_t begin, 
        iterator_t const end)
    {
        if (is_terminal (id, grammar))
        {
            auto && matched = begin != end && (*begin).id == id ; 
        
            return 
            { tree<language_t, data_iterator_t<iterator_t>> { matched ? (*begin).id : language_t::error, (*begin).data }, matched ? ++begin : begin } ;
        }
        else switch (grammar.dictionary.at(id).op)
        {
            case production_operator::some : 
                return build_tree_some (id, grammar, begin, end) ;
            case production_operator::and_ : 
                return build_tree_and (id, grammar, begin, end) ;
            case production_operator::or_ :
                return build_tree_or (id, grammar, begin, end) ;
        }
    }

    template <
        typename language_t, 
        typename iterator_t>
    std::tuple<tree<language_t, data_iterator_t<iterator_t>>, iterator_t>
    build_tree (
        basic_grammar<language_t> const & grammar, 
        iterator_t begin, 
        iterator_t const end)
    {
        return 
        build_tree (grammar.start, grammar, begin, end) ;
    }*/
}

#endif
