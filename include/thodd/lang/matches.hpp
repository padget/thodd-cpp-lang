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
    /// TERMINAL
    template<
        auto id_c, 
        typename regex_t>
    constexpr auto 
    matches (
        terminal<id_c, regex_t>, 
        auto __cursor, 
        auto __end)
    {
        auto && [__matched, __it] = matches (regex_t {}, __cursor, __end) ;
        
        return 
        token { std::pair { __cursor, __it }, id_c } ;

    }    


    /// CHAR_
    template<auto c>
    constexpr auto 
    matches (
        chr<c>,
        auto __cursor, 
        auto __end)
    {
        return 
        __cursor != __end && *__cursor == c ?
        std::tuple { true, ++__cursor } : 
        std::tuple { false, __cursor } ;        
    }



    /// BETWEEN
    template<auto min_c, auto max_c>
    constexpr auto
    matches (
        between<min_c, max_c>,
        auto __cursor, 
        auto __end)
    {
        return 
        __cursor != __end && 
        (min_c <= *__cursor && *__cursor <= max_c) ? 
        std::tuple { true, ++__cursor } : 
        std::tuple { false, __cursor } ; 
    }



    /// NOT 
    template <
        typename regex_t>
    constexpr auto
    matches (
        not_<regex_t>, 
        auto __cursor, 
        auto __end)
    {
        auto&& [__matched, __it] = matches (regex_t {}, __cursor, __end) ;
     
        return 
        __it == __cursor ?
        std::tuple { !__matched, __it } :
        std::tuple { __matched, __cursor } ;  
    }



    /// SOME
    template<
        typename regex_t, 
        auto min_c, 
        auto max_c>
    constexpr auto 
    matches (
        some<regex_t, min_c, max_c>, 
        auto __cursor, 
        auto __end)
    {
        auto __cpt = 0u ;
        auto __previous = __cursor ;
        bool __continue = true ;
        constexpr auto __regex = regex_t {} ;

        while (__cpt <= max_c && __previous != __end && __continue)
        { 
            auto&& [__matched, __it] = matches (__regex, __previous, __end) ;
            __previous = (__continue = __matched) ? (++__cpt, __it) : __previous ;
        }

        return 
        min_c <= __cpt && __cpt <= max_c ? 
        std::tuple { true, __previous } : 
        std::tuple { false, __cursor } ; 
    }



    /// AND_
    template<
        typename ... item_t>
    constexpr auto
    matches (
        and_<item_t...>,
        auto __cursor, 
        auto __end)
    {
        auto __tmp_it = __cursor ; 
        auto __tmp_matched = true ;

        constexpr auto 
        __each = 
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

        auto __matcheds = 
            std::apply( 
                [__each, &__tmp_it, &__tmp_matched, __end] (auto const & ... __regex)
                { return std::array { __each (__tmp_matched, __regex, __tmp_it, __end) ... } ; },
                std::tuple {item_t {} ...}) ;

        return 
        std::all_of(__matcheds.begin(), __matcheds.end(), $0) ? 
        std::tuple { true, __tmp_it } : 
        std::tuple { false, __cursor } ; 
    }



    /// OR_
    template<
        typename ... item_t>
    constexpr auto
    matches (
        or_<item_t...>,
        auto __cursor, 
        auto __end)
    {
    	auto __tmp_it = __cursor ; 
        auto __tmp_matched = false ;

        constexpr auto __each =
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

        auto __matcheds = 
            std::apply(
                [__each, &__tmp_it, &__tmp_matched, __end] (auto const & ... __choice)
                { return std::array { __each(__tmp_matched, __choice, __tmp_it, __end) ... } ; },
                std::tuple {item_t {} ...}) ;       

        return 
        std::any_of (__matcheds.begin(), __matcheds.end(), $0)  ? 
        std::tuple { true, __tmp_it } : 
        std::tuple { false, __cursor } ; 
    }
}

#endif