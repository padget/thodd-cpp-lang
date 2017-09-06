#ifndef __THODD_LANG_MATCHES_HPP__
#  define __THODD_LANG_MATCHES_HPP__

#  include <utility>
#  include <type_traits>
#  include <tuple>

#  include <thodd/lang/pod.hpp>
#  include <thodd/functional.hpp>


namespace 
thodd::lang
{
    /// CHAR_
    template<auto c>
    inline auto 
    matches (
        chr<c> const& __char,
        auto __cursor, 
        auto const __end)
    {
        return 
        __cursor != __end && *__cursor == c ?
        std::tuple { true, ++__cursor } : 
        std::tuple { false, __cursor } ;        
    }



    /// BETWEEN
    template<auto min_c, auto max_c>
    inline auto
    matches (
        between<min_c, max_c> const& __between,
        auto __cursor, 
        auto const __end)
    {
        return 
        __cursor != __end && 
        (min_c <= *__cursor && *__cursor <= max_c) ? 
        std::tuple { true, ++__cursor } : 
        std::tuple { false, __cursor } ; 
    }



    /// NOT 
    inline auto
    matches (
        not_<auto> const& __not, 
        auto __cursor, 
        auto const __end)
    {
        auto&& [__matched, __it] = matches (__not.item, __cursor, __end) ;
     
        return 
        !__matched ?
        std::tuple { !__matched, __it } :
        std::tuple { __matched, __cursor } ;  
    }



    /// SOME
    inline auto 
    matches (
        some<auto> const& __some, 
        auto __cursor, 
        auto const __end)
    {
        auto __cpt = 0u ;
        auto __previous = __cursor ;
        bool __continue = true ;
        
        while (__cpt <= __some.max && __previous != __end && __continue)
        { 
            auto&& [__matched, __it] = matches (__some.item, __previous, __end) ;
            __continue = __matched ;

            if(__matched) 
            {    
                ++__cpt ;            
                __previous = __it ;
            }
        }

        return 
        __some.min <= __cpt && __cpt <= __some.max ? 
        std::tuple { true, __previous } : 
        std::tuple { false, __cursor } ; 
    }



    /// AND_
    template<
        typename ... types_t>
    inline auto
    matches (
        and_<types_t...> const& __and,
        auto __cursor, 
        auto const __end)
    {

        auto __tmp_it = __cursor ; 
        auto __tmp_matched = true ;

        auto __each = 
            [] (auto & __prev_matched, auto & __regex, auto & __begin, auto const & __end) 
            { 
                if (__prev_matched)
                {
                    auto&& [__matched, __it] = matches (__regex, __begin, __end) ;
                    __begin = __it ;
                    __prev_matched = __matched ;
                }

                return 
                __prev_matched ;  
            } ;

        auto __matcheds = std::apply( 
                            [__each, &__tmp_it, &__tmp_matched, __end] (auto const & ... __regex)
                            { return std::array { __each (__tmp_matched, __regex, __tmp_it, __end) ... } ; },
                            __and.items) ;

        return 
        std::all_of(__matcheds.begin(), __matcheds.end(), $0) ? 
        std::tuple { true, __tmp_it } : 
        std::tuple { false, __cursor } ; 
    }



    /// OR_
    template<
        typename ... types_t>
    inline auto
    matches (
        or_<types_t...> const& __or,
        auto __cursor, 
        auto const __end)
    {

    	auto __tmp_it = __cursor ; 
        auto __tmp_matched = false ;

        auto __each =
            [] (auto & __prev_matched, auto const & __regex, auto & __begin, auto & __end) 
            {
                if(!__prev_matched)
                {
                    auto&& [__matched, __it] = matches (__regex, __begin, __end) ;
                    __prev_matched = __matched ;
                    __begin = __it ;
                }

                return 
                __prev_matched ;
            } ;

        auto __matcheds = std::apply(
            [__each, &__tmp_it, &__tmp_matched, __end] (auto const & ... __choice)
            { return std::array { __each(__tmp_matched, __choice, __tmp_it, __end) ... } ; } ,
             __or.choices) ;       

        return 
        std::any_of (__matcheds.begin(), __matcheds.end(), $0)  ? 
        std::tuple { true, __tmp_it } : 
        std::tuple { false, __cursor } ; 
    }
}

#endif