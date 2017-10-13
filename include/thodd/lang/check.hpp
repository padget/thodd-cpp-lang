#ifndef __THODD_LANG_CHECK_HPP__
#  define __THODD_LANG_CHECK_HPP__

#  include <thodd/lang/regex.hpp>
#  include <thodd/lang/match_result.hpp>
#  include <thodd/lang/grammar.hpp>
#  include <thodd/functional.hpp>

namespace 
thodd::lang::syntax
{
    inline constexpr auto 
    def_handler = bind(tuple, $0, $1) ;

    inline constexpr auto 
    def_handler_id = bind(nth, $0, val(idx0)) ;

    inline constexpr auto
    def_handler_handler = bind(nth, $0, val(idx1)) ;

    inline constexpr auto 
    generic_check =
    [] (auto id, auto begin, auto const end, auto const & def_handlers)
    {
        constexpr auto for_each_def_handler = 
        [id, begin, end] (auto && def_handler, auto && ... next_handler)
        {
            if (id == id_of(def))
                return def 
        } 

        return
        defs (
            [id, begin, end] (auto && ... def)
            {
                
            }) ;
    }

    inline constexpr auto
    check_builder = 
    [] (grammar_data<auto> const & grammar)
    {
        return 
        std::apply (
            [start_id = grammar.start_id] (auto && ... def) 
            {
                return 
                [start_id, def...] ()
                {
                    constexpr auto
                    build_specific_checker =  
                    [] (auto && def)
                    {
                        if constexpr (is_some_def(def))
                            return 
                            [step_id = def.step_id, 
                             min = def.min, 
                             max = def.max] 
                            (auto begin, auto const end, auto const & defs)
                            {        
                                auto checked = true ;
                                auto local_cursor = begin ; 
                                
                                auto cpt = 0u ;

                                while (checked && cpt < max)
                                {
                                    auto && mres = generic_check (step_id, local_cursor, end, defs) ;
                                
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
                        else if constexpr (is_sequence_def(def))
                            return 
                            [step_ids = def.step_ids] 
                            (auto begin, auto const end, auto const & defs)
                            {
                                auto local_cursor = begin ; 
                                auto checked = true ;

                                for (auto && step_id : step_ids)
                                {
                                    auto && mres = generic_check (step_id, local_cursor, end, defs) ;
                        
                                    if (checked = matched (mres)) 
                                        local_cursor = cursor (mres) ; 
                                    else break ;
                                }
                                
                                return 
                                match_result (checked, checked ? local_cursor : begin) ;
                            } ;
                        else if constexpr (is_alternative_def(def))
                            return 
                            [step_ids = def.step_ids] 
                            (auto begin, auto const end, auto const & defs)
                            {
                                auto local_cursor = begin ; 
                                auto checked = false ;

                                for (auto && step_id : step_ids)
                                {
                                    auto && mres = generic_check (step_id, local_cursor, end, defs) ;
                        
                                    if (checked = matched (mres))
                                    {    
                                        local_cursor = cursor (mres) ;
                                        break ;
                                    }
                                }
                        
                                return 
                                match_result (checked, checked ? local_cursor : begin) ;
                            } ;
                    } ;

                    constexpr auto defs = 
                    thodd::tuple(
                        build_specific_checker (def)...) ;
                } ;
            }, 
            grammar.defs) ;
    } ;
}

#endif