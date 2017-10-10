#ifndef __THODD_LANG_CHECK_HPP__
#  define __THODD_LANG_CHECK_HPP__

#  include <thodd/lang/regex.hpp>
#  include <thodd/lang/match_result.hpp>
#  include <thodd/lang/grammar.hpp>

namespace 
thodd::lang::syntax
{
    inline constexpr auto 
    check_or = 
    [] (auto id, auto const & grammar, auto begin, auto const end)
    {
        auto local_cursor = begin ; 
        auto checked = false ;
        auto const & step_ids = get_ids_by_id (id, grammar) ;
        
        for (auto && step_id : step_ids)
        {
            auto && mres = generic_check (step_id, grammar, local_cursor, end) ;

            if (checked = matched (mres))
            {    
                local_cursor = cursor (mres) ;
                break ;
            }
        }

        return 
        match_result (checked, checked ? local_cursor : begin) ;
    } ;

    inline constexpr auto 
    check_and =
    [] (auto id, auto const & grammar, auto begin, auto const end)
    {
        auto local_cursor = begin ; 
        auto checked = true ;
        auto const & step_ids = get_ids_by_id (id, grammar) ;

        for (auto && step_id : step_ids)
        {
            auto && mres = generic_check (step_id, grammar, local_cursor, end) ;

            if (checked = matched (mres)) 
                local_cursor = cursor (mres) ; 
            else break ;
        }  
        
        return 
        match_result (checked, checked ? local_cursor : begin) ;
    } ;

    inline constexpr auto 
    check_some = 
    [] (auto id, auto const & grammar, auto begin, auto const end)
    {
        auto cpt = 0u ;
        auto checked = true ;
        auto local_cursor = begin ; 
        auto step_id = get_iss_by_id(id, grammar).begin() ;
        size_t const min = get_min(get_is_by_id(id, grammar)) ;
        size_t const max = get_max(get_is_by_id(id, grammar)) ;
        
        while (checked && cpt < max)
        {
            auto && mres = generic_check (step_id, grammar, local_cursor, end) ;
        
            if (checked = matched (mres))
            {
                ++ cpt ;
                local_cursor = cursor (mres) ;
            } 
        }

        checked = min <= cpt && cpt <= max ; 

        return 
        match_result (checked, checked ? local_cursor : begin) ;
    } ;


    inline constexpr auto 
    generic_check = 
    [] (auto id, auto const & grammar, auto begin, auto const end) 
    {
        if (is_terminal (id, grammar))
        {
            auto is_same = begin != end && *begin == id ; 
            
            return 
            match_result (is_same, is_same ? ++begin : begin) ;
        }
        else switch (get_operator_by_id (id, grammar))
        {
            case production_operator::some : 
                return check_some (id, grammar, begin, end) ;
            case production_operator::and_ : 
                return check_and (id, grammar, begin, end) ;
            case production_operator::or_  : 
                return check_or (id, grammar, begin, end) ;
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
            generic_check (grammar_start(grammar), grammar, begin, end) ;
        } ;
    } ;
}

#endif