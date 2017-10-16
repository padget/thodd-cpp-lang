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
            match_result { matches, matches ? ++begin : begin } ;
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
            match_result { matched, matched ? ++begin : begin } ;
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
                    auto && [step_matched, step_cursor] = rx (local_cursor, end) ;
                    
                    if (local_matched = step_matched)
                    {
                        ++ cpt ;
                        local_cursor = step_cursor ;
                    }
                }

                local_matched = min <= cpt && cpt <= max ; 
                
                return 
                match_result { local_matched, local_matched ? local_cursor : begin } ;
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
            auto local_matched = false ;
            auto local_cursor = begin ; 

            auto each = 
            [&local_matched, &local_cursor] (auto const & rx, auto begin, auto const end) 
            { if (!local_matched) { auto && [step_matched, step_cursor] = rx (begin, end) ; local_matched = step_matched ; local_cursor = step_cursor ; } } ;
            
            (each (rx, local_cursor, end), ...) ;
          
            return 
            match_result { local_matched, local_matched ? local_cursor : begin } ;
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
            auto local_matched = true ;
            auto local_cursor = begin ; 

            auto each = 
            [&local_matched, &local_cursor] (auto const & rx, auto begin, auto const end) 
            { if (local_matched) { auto && [step_matched, step_cursor] = rx (begin, end) ; local_matched = step_matched ; local_cursor = step_cursor ; } } ;
            
            (each (rx, local_cursor, end), ...) ;
          
            return 
            match_result { local_matched, local_matched ? local_cursor : begin } ;
        } ;
    } ;

    inline constexpr auto
    term = 
    [] (auto id, auto rx)
    {
        return 
        [id, rx] (auto begin, auto end)
        {
            auto && [local_matched, local_cursor] = rx (begin, end) ;
        
            return 
            token { std::pair { begin, local_cursor }, id } ;
        } ;
    } ;
}





#endif
