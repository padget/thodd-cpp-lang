#ifndef __THODD_LANG_CHECK_HPP__
#  define __THODD_LANG_CHECK_HPP__

#  include <thodd/lang2/regex.hpp>
#  include <thodd/lang2/grammar.hpp>
#  include <thodd/functional.hpp>

namespace 
thodd::lang::syntax
{
    template<
        typename iterator_t>
    struct check_result
    {
        bool checked ; 
        iterator_t local_cursor ; 
    } ;

    template <typename iterator_t>
    check_result (bool, iterator_t) -> check_result<iterator_t> ;


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
    compute_check_result (
        auto begin, 
        auto local_cursor, 
        auto checked) 
    {
        return 
        check_result { checked, checked ? local_cursor : begin } ;
    }


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
                auto && [step_checked, step_cursor] = generic_check (step_id, local_cursor, end, defs) ;

                if (checked = step_checked)
                {    
                    local_cursor = step_cursor ;
                    break ;
                }
            }

            return 
            compute_check_result (begin, local_cursor, checked) ;
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
            compute_check_result (begin, local_cursor, checked) ;
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
            compute_check_result (begin, local_cursor, checked) ;
        } ;
    }   

    constexpr auto 
    check_handler_terminal_algorithm (
        auto terminal_id, 
        auto const & def_handlers)
    {
        return 
        [terminal_id, def_handlers] 
        (auto begin, auto const end)
        {
            bool checked = begin != end && *begin == terminal_id ;
            auto local_cursor = checked ? ++begin : begin ; 
            compute_check_result (begin, local_cursor, checked) ;
        } ;
    }
}

#endif