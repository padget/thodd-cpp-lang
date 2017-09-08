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

    /*/// NON TERMINAL 
    template <auto id_c>
    constexpr auto 
    matches (
        non_terminal<id_c>, 
        auto const & __item_accessor, 
        auto __cursor, 
        auto const __end)
    {
        return 
        matches ( 
            extract_definition(meta::start (__grammar)), 
            __item_accessor,
            __cursor, __end) ;
        }*/
        
    /// TERMINAL
    template<
        auto id_c, 
        typename regex_t>
    constexpr auto 
    matches (
        terminal<id_c, regex_t> const & __term, 
        auto const & __item_accessor, 
        auto __cursor, 
        auto const __end)
    {
        auto && [__matched, __it] = matches (regex_t {}, __item_accessor, __cursor, __end) ;
        
        return 
        token { std::pair { __cursor, __it }, id_c } ;

    }
    


    /// CHAR_
    template<auto c>
    constexpr auto 
    matches (
        chr<c> const&,
        auto const & __item_accessor,
        auto __cursor, 
        auto const __end)
    {
        return 
        __cursor != __end && __item_accessor(__cursor) == c ?
        std::tuple { true, ++__cursor } : 
        std::tuple { false, __cursor } ;        
    }



    /// BETWEEN
    template<auto min_c, auto max_c>
    constexpr auto
    matches (
        between<min_c, max_c> const&,
        auto const & __item_accessor,
        auto __cursor, 
        auto const __end)
    {
        return 
        __cursor != __end && 
        (min_c <= __item_accessor(__cursor) && __item_accessor(__cursor) <= max_c) ? 
        std::tuple { true, ++__cursor } : 
        std::tuple { false, __cursor } ; 
    }



    /// NOT 
    template <
        typename regex_t>
    constexpr auto
    matches (
        not_<regex_t> const& __not, 
        auto const & __item_accessor,
        auto __cursor, 
        auto const __end)
    {
        auto&& [__matched, __it] = matches (regex_t {}, __item_accessor, __cursor, __end) ;
     
        return 
        !__matched ?
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
        some<regex_t> const&, 
        auto const & __item_accessor,
        auto __cursor, 
        auto const __end)
    {
        auto __cpt = 0u ;
        auto __previous = __cursor ;
        bool __continue = true ;
        constexpr auto __regex = regex_t {} ;

        while (__cpt <= max_c && __previous != __end && __continue)
        { 
            auto&& [__matched, __it] = matches (__regex, __item_accessor, __previous, __end) ;
            __continue = __matched ;

            if(__matched) 
            {    
                ++__cpt ;            
                __previous = __it ;
            }
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
        and_<item_t...> const& __and,
        auto const & __item_accessor,
        auto __cursor, 
        auto const __end)
    {

        auto __tmp_it = __cursor ; 
        auto __tmp_matched = true ;

        auto __each = 
            [__item_accessor] (auto & __prev_matched, auto & __regex, auto & __begin, auto const & __end) 
            { 
                if (__prev_matched)
                {
                    auto&& [__matched, __it] = matches (__regex, __item_accessor, __begin, __end) ;
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
        or_<item_t...> const& __or,
        auto const & __item_accessor,
        auto __cursor, 
        auto const __end)
    {

    	auto __tmp_it = __cursor ; 
        auto __tmp_matched = false ;

        auto __each =
            [__item_accessor] (auto & __prev_matched, auto const & __regex, auto & __begin, auto & __end) 
            {
                if(!__prev_matched)
                {
                    auto&& [__matched, __it] = matches (__regex, __item_accessor, __begin, __end) ;
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