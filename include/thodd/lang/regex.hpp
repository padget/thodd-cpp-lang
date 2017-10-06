#ifndef __THODD_LANG_REGEX_HPP__
#  define __THODD_LANG_REGEX_HPP__

#  include <limits>

#  include <thodd/lang/token.hpp>
#  include <thodd/lang/match_result.hpp>
#  include <thodd/functional.hpp>

namespace
thodd::lang::regex
{    
    inline constexpr auto   
    chr = 
    [] (auto c) 
    {
        return 
        [c] (auto begin, auto const end)
        {
            auto matches = begin != end && *begin == c ;
            
            return 
            match_result (matches, matches ? ++begin : begin) ;
        } ;
    } ;
    
    inline constexpr auto
    range = 
    [] (auto min, auto max)
    {
        return 
        [min, max] (auto begin, auto const end)
        {
            auto matched = 
                begin != end 
                && min <= *begin 
                && *begin <= max ;
            
            return 
            match_result (matched, matched ? ++begin : begin) ;
        } ;
    } ;


    inline constexpr auto
    some =
    [] (auto rx)
    {
        return
        [rx] (auto min, auto max)
        {
            return 
            [rx, min, max] (auto begin, auto const end)
            {
                auto cpt = 0u ;
                auto local_cursor = begin ; 
                auto local_matched = true ; 

                while (local_matched && cpt < max)
                {
                    auto && mres = rx (local_cursor, end) ;
                    
                    if (local_matched = matched (mres))
                    {
                        ++ cpt ;
                        local_cursor = cursor (mres) ;
                    }
                }

                local_matched = min <= cpt && cpt <= max ; 
                
                return 
                match_result (local_matched, local_matched ? local_cursor : begin) ;
            } ;
        } ;
    } ;

    inline constexpr auto
    one_more = 
    [] (auto rx)
    { return some (rx) (1u, std::numeric_limits<size_t>::max()) ; } ;

    inline constexpr auto
    zero_more = 
    [] (auto rx)
    { return some (rx) (0u, std::numeric_limits<size_t>::max()) ; } ;

    inline constexpr auto
    one_of = 
    [] (auto ... rx)
    {
        static_assert (sizeof...(rx) > 0, "or_ must be applied to one or more rxs") ;
        
        return 
        [rx...] (auto begin, auto const end)
        {
            auto step_matched = false ;
            auto step_cursor = begin ; 

            auto each = 
            [&step_matched, &step_cursor] (auto const & rx, auto begin, auto const end) 
            { if (!step_matched) { auto && mres = rx (begin, end) ; step_matched = matched (mres) ; step_cursor = cursor (mres) ; } } ;
            
            (each (rx, step_cursor, end), ...) ;
          
            return 
            match_result (step_matched, step_matched ? step_cursor : begin) ;
        } ;
    } ;
        
    inline constexpr auto
    sequence_of = 
    [] (auto ... rx)
    {
        static_assert (sizeof...(rx) > 0, "and_ must be applied to one or more rxs") ;

        return 
        [rx...] (auto begin, auto const end)
        {
            auto step_matched = true ;
            auto step_cursor = begin ; 

            auto each = 
            [&step_matched, &step_cursor] (auto const & rx, auto begin, auto const end) 
            { if (step_matched) { auto && mres = rx (begin, end) ; step_matched = matched (mres) ; step_cursor = cursor (mres) ; } } ;

            (each (rx, step_cursor, end), ...) ;

            return 
            match_result (step_matched, step_matched ? step_cursor : begin) ;
        } ;
    } ;

    inline constexpr auto
    term = 
    [] (auto id, auto rx)
    {
        return 
        [id, rx] (auto begin, auto end)
        {
            auto && mres = rx (begin, end) ;
        
            return 
            token { std::pair { begin, cursor (mres) }, id } ;
        } ;
    } ;
}





#endif
