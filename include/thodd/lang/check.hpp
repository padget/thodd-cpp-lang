#ifndef __THODD_LANG_CHECK_HPP__
#  define __THODD_LANG_CHECK_HPP__

#  include <thodd/lang/regex.hpp>
#  include <thodd/lang/match_result.hpp>
#  include <thodd/lang/grammar.hpp>

namespace 
thodd::lang::syntax
{
    /*template <
        typename language_t, 
        typename iterator_t>
    constexpr auto
    check_some (
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
        
        while (checked && cpt < max)
        {
            auto && mres = check (step_id, grammar, local_cursor, end) ;
        
            if (checked = matched (mres))
            {
                ++ cpt ;
                local_cursor = cursor (mres) ;
            } 
        }

        checked = min <= cpt && cpt <= max ; 

        return 
        match_result (checked, checked ? local_cursor : begin) ;
    }


    template <
        typename language_t, 
        typename iterator_t>
    constexpr auto
    check_and (
        language_t and_id, 
        basic_grammar<language_t> const & grammar, 
        iterator_t begin, 
        iterator_t const end)
    {
        auto local_cursor = begin ; 
        auto checked = true ;
        auto const & step_ids = grammar.dictionary.at(and_id).ids ;

        for (auto && step_id : step_ids)
        {
            auto && mres = check (step_id, grammar, local_cursor, end) ;

            if (checked = matched (mres)) 
                local_cursor = cursor (mres) ; 
            else break ;
        }  
        
        return 
        match_result (checked, checked ? local_cursor : begin) ;
    }

    template <
        typename language_t, 
        typename iterator_t>
    constexpr auto 
    check_or (
        language_t or_id, 
        basic_grammar<language_t> const & grammar, 
        iterator_t begin, 
        iterator_t const end)
    {
        auto local_cursor = begin ; 
        auto checked = false ;
        auto const & step_ids = grammar.dictionary.at(or_id).ids ;
        
        for (auto && step_id : step_ids)
        {
            auto && mres = check (step_id, grammar, local_cursor, end) ;

            if (checked = matched (mres))
            {    
                local_cursor = cursor (mres) ;
                break ;
            }
        }

        return 
        match_result (checked, checked ? local_cursor : begin) ; 
    }

    template <
        typename language_t, 
        typename iterator_t>
    constexpr auto
    check (
        language_t id, 
        basic_grammar<language_t> const & grammar, 
        iterator_t begin, 
        iterator_t const end)
    {
        if (is_terminal (id, grammar))
        {   
            auto is_same = begin != end && *begin == id ; 
            
            return 
            match_result(is_same, is_same ? ++begin : begin) ;
        }
        else switch (grammar.dictionary.at(id).op)
        {
            case production_operator::some : 
                return check_some (id, grammar, begin, end) ;
            case production_operator::and_ : 
                return check_and (id, grammar, begin, end) ;
            case production_operator::or_  : 
                return check_or (id, grammar, begin, end) ;
        }
    }    


    template <
        typename language_t>
    constexpr auto 
    check (
        basic_grammar<language_t> const & grammar, 
        auto begin,
        auto const end)
    {
        return 
        check (grammar.start, grammar, begin, end) ;
    }*/


    inline constexpr auto 
    generic_check = 
    [] (auto id, auto const & grammar, auto begin, auto const end) 
    {
        if (is_terminal (id, grammar))
        {
            auto is_same = begin != end && *begin == id ; 
            
            return 
            match_result(is_same, is_same ? ++begin : begin) ;
        }
    } ;

    inline constexpr auto
    check_builder = 
    [] (auto && grammar)
    {
        return 
        [grammar] (auto begin, auto const end)
        {
            return 
            generic_check (grammar, begin, end) ;
        } ;
    } ;
}

#endif