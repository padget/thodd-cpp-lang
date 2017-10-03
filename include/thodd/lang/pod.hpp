#ifndef __THODD_LANG_POD_HPP__
#  define __THODD_LANG_POD_HPP__

#  include <limits>

#  include <thodd/lang/token.hpp>

namespace
thodd::lang
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
            std::tuple { matches, matches ? ++begin : begin } ;
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
            std::tuple { matched, matched ? ++begin : begin } ;
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
                std::tuple { local_matched, local_matched ? local_cursor : begin } ;
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
    or_ = 
    [] (auto ... rx)
    {
        static_assert (sizeof...(rx) > 0, "or_ must be applied to one or more rxs") ;
        
        return 
        [rx...] (auto begin, auto const end)
        {
            auto matched = false ;
            auto step_cursor = begin ; 

            auto each = 
            [&matched, &step_cursor] (auto const & rx, auto begin, auto const end) 
            { 
                if (!matched)
                {
                    auto && [m, c] = rx (begin, end) ;
                    matched = m ; step_cursor = c ;
                }
            } ;

            return 
            std::tuple { matched, matched ? step_cursor : begin } ;
        } ;
    } ;
        
    inline constexpr auto
    and_ = 
    [] (auto ... rx)
    {
        static_assert (sizeof...(rx) > 0, "and_ must be applied to one or more rxs") ;

        return 
        [rx...] (auto begin, auto const end)
        {
            auto matched = true ;
            auto step_cursor = begin ; 

            auto each = 
            [&matched, &step_cursor] (auto const & rx, auto begin, auto const end) 
            { 
                if (matched)
                {
                    auto && [m, c] = rx (begin, end) ;
                    matched = m ; step_cursor = c ;
                }
            } ;

            (each(rx, step_cursor, end), ...) ;

            return 
            std::tuple { matched, matched ? step_cursor : begin } ;
        } ;
    } ;

    inline constexpr auto
    term = 
    [] (auto id, auto rx)
    {
        return 
        [id, rx] (auto begin, auto end)
        {
            auto && [matched, cursor] = rx(begin, end) ;
        
            return 
            token { std::pair { begin, cursor }, id } ;
        } ;
    } ;
}





#endif