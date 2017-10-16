#ifndef __THODD_LANG_CHECK_HPP__
#  define __THODD_LANG_CHECK_HPP__

#  include <thodd/lang/regex.hpp>
#  include <thodd/lang/check_result.hpp>
#  include <thodd/lang/grammar.hpp>
#  include <thodd/functional.hpp>

namespace 
thodd::lang::syntax
{
    template <
        typename language_t,
        typename handler_t>
    struct def_hanlder 
    {
        language_t id ;
        handler_t handler ;
    } ;

    template <typename language_t, typename handler_t>
    def_handler (language_t id, handler_t handler) -> def_handler<language_t, handler_t> ;


    constexpr auto
    check_handler_oneof_algorithm (
        auto step_ids, 
        auto const & def_handlers)  
    {
        return 
        [step_ids, def_handlers] (auto begin, auto const end) 
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
            check_result { checked, checked ? local_cursor : begin } ;
        } ;
    }

    constexpr auto
    check_handler_sequence_algorithm (
        auto step_ids, 
        auto const & def_handlers) 
    {
        return 
        [step_ids, def_handlers] (auto begin, auto const end) 
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
            check_result { checked, checked ? local_cursor : begin } ;
        } ;
    }

    constexpr auto
    check_handler_some_algorithm (
        auto step_id, 
        auto const & def_handlers) 
    {
        [step_id, def_handlers] (auto begin, auto const end) 
        {
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
            check_result { checked, checked ? local_cursor : begin } ;
        } ;
    }   
}

#endif