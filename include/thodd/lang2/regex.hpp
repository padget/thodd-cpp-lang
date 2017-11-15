#ifndef __thodd_lang_regex_hpp__
#  define __thodd_lang_regex_hpp__

#  include <thodd/functional.hpp>
#  include <thodd/optional.hpp>

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
            return
            thodd::make_optional_if(
                begin, 
                [&] (auto && cursor) { return not_equals(cursor, end) && get(cursor) == c ; }, 
                [] (auto && cursor) { return next(std::forward<decltype(cursor)>(cursor)) ; }); 
        } ; 
    } ;

    inline constexpr auto 
    range = 
    [] (auto min, auto max)
    {
        return 
        [min, max] (auto begin, auto const end)
        {
            return 
            thodd::make_optional_if (
                begin, 
                [&] (auto && cursor) { return not_equals(cursor, end) && min <= get(cursor) && get(cursor) <= max ; },
                [] (auto && cursor) { return next(std::forward<decltype(cursor)>(cursor)) ; }) ;
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
                    if_exists(
                        rx (local_cursor, end), 
                        [&] (auto && cursor) 
                        { 
                            local_matched = true ;
                            local_cursor = std::forward<decltype(cursor)>(cursor) ;
                            ++cpt ;
                        }, 
                        [&] { local_matched = false ; }) ;

                local_matched = min <= cpt && cpt <= max ; 
                
                return 
                make_optional_if (
                    local_cursor, 
                    cref (local_matched), 
                    tern (cref(local_matched)) [ref(local_cursor)] [ref(begin)]) ; 
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
            { 
                if (!local_matched) 
                    if_exists(
                        rx (begin, end),
                        [&] (auto && cursor) 
                        {
                            local_matched = true ; 
                            local_cursor = std::forward<decltype(cursor)>(cursor) ;
                        }, 
                        [&] { local_matched = false ; });
            } ;
            
            (each (rx, local_cursor, end), ...) ;
          
            return 
            make_optional_if (
                local_cursor, 
                cref (local_matched), 
                tern (cref(local_matched)) [ref(local_cursor)] [ref(begin)]) ; 
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
            { 
                if (local_matched) 
                    if_exists(
                        rx (begin, end),
                        [&] (auto && cursor) 
                        {
                            local_matched = true ; 
                            local_cursor = std::forward<decltype(cursor)>(cursor) ;
                        }, 
                        [&] { local_matched = false ; });
            } ;
            
            (each (rx, local_cursor, end), ...) ;
          
            return 
            make_optional_if (
                local_cursor, 
                cref (local_matched), 
                tern (cref(local_matched)) [ref(local_cursor)] [ref(begin)]) ; 
        } ;
    } ;
}

#endif