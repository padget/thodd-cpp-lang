#ifndef __THODD_LANG_CHECK_HPP__
#  define __THODD_LANG_CHECK_HPP__

#  include <thodd/lang/pod.hpp>

namespace 
thodd::lang
{
    template <
        auto id_c>
    constexpr auto
    check (
        terminal<id_c, auto>,
        auto const & __grammar,
        auto __cursor, 
        auto const __end)
    {
        return 
        __cursor != __end && (*__cursor).id == id_c ?
        std::tuple { true, ++__cursor } : 
        std::tuple { false, __cursor } ;        
    }


    
          
    template <
        typename item_t, 
        auto min_c, 
        auto max_c>
    constexpr auto
    check (
        some<item_t, min_c, max_c>,
        auto __grammar, 
        auto __cursor, 
        auto __end)
    {
        auto __cpt = 0u ;
        auto __previous = __cursor ;
        bool __continue = true ;
        constexpr auto __regex = item_t {} ;

        while (__cpt <= max_c && __previous != __end && __continue)
        { 
            auto&& [__matched, __it] = check (__regex, __grammar, __previous, __end) ;
            __previous = (__continue = __matched) ? (++__cpt, __it) : __previous ;
        }

        return 
        min_c <= __cpt && __cpt <= max_c ? 
        std::tuple { true, __previous } : 
        std::tuple { false, __cursor } ; 
    }

    template <
        typename ... item_t>
    constexpr auto
    check (
        and_<item_t...>,
        auto __grammar, 
        auto __cursor, 
        auto __end)
    {
        auto __tmp_it = __cursor ; 
        auto __tmp_matched = true ;

        constexpr auto 
        __each = 
        [] (auto & __prev_matched, auto & __regex, auto const & __grammar, auto & __begin, auto const & __end) 
        { 
            if (__prev_matched)
            {
                auto&& [__matched, __it] = check (__regex, __grammar, __begin, __end) ;
                __begin = __it ;
                __prev_matched = __matched ;
            }

            return 
            __prev_matched ;  
        } ;

        auto __matcheds = 
            std::apply( 
                [__each, &__tmp_it, &__tmp_matched, &__grammar, __end] (auto const & ... __regex)
                { return std::array { __each (__tmp_matched, __regex, __grammar, __tmp_it, __end) ... } ; },
                std::tuple {item_t {} ...}) ;

        return 
        std::all_of(__matcheds.begin(), __matcheds.end(), $0) ? 
        std::tuple { true, __tmp_it } : 
        std::tuple { false, __cursor } ; 
    }

    template <
        typename ... item_t>
    constexpr auto
    check (
        or_<item_t...>,
        auto __grammar, 
        auto __cursor, 
        auto __end)
    {
        auto __tmp_it = __cursor ; 
        auto __tmp_matched = false ;

        constexpr auto __each =
            [] (auto & __prev_matched, auto const & __regex, auto const & __grammar, auto & __begin, auto & __end) 
            {
                if (!__prev_matched)
                {
                    auto&& [__matched, __it] = check (__regex, __grammar, __begin, __end) ;
                    __prev_matched = __matched ;
                    __begin = __it ;
                }

                return 
                __prev_matched ;
            } ;

        auto __matcheds = 
            std::apply(
                [__each, &__tmp_it, &__tmp_matched, &__grammar, __end] (auto const & ... __choice)
                { return std::array { __each(__tmp_matched, __choice, __grammar, __tmp_it, __end) ... } ; },
                std::tuple {item_t {} ...}) ;       

        return 
        std::any_of (__matcheds.begin(), __matcheds.end(), $0)  ? 
        std::tuple { true, __tmp_it } : 
        std::tuple { false, __cursor } ; 
    }


        
    template <
        auto id_c>
    constexpr auto 
    check (
        non_terminal<id_c> const & __nt,
        auto const & __grammar,
        auto __cursor, 
        auto const __end)
    {
        using definition_t = 
        decltype (
            extract_definition (
                std::decay_t<decltype(__nt)> {}, 
                std::decay_t<decltype(__grammar)> {})) ;
        
        return 
        check (
            definition_t {}, __grammar, 
            __cursor, __end) ;
    }
   

    template <
        typename start_t, 
        typename ... rule_t>
    constexpr auto
    check (
        grammar_rules<start_t, rule_t...> __grammar,
        auto __cursor, 
        auto __end) 
    {
        return 
        check (
            extract_definition(
                meta::start (__grammar), 
                __grammar), 
            __grammar,
            __cursor, 
            __end) ;
    }
}

#endif