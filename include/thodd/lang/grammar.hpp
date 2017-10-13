#ifndef __THODD_LANG_GRAMMAR_HPP__
#  define __THODD_LANG_GRAMMAR_HPP__

#  include <array>
#  include <limits>
#  include <tuple>
#  include <type_traits>
#  include <utility>

#  include <thodd/functional.hpp>

namespace
thodd::lang::syntax
{
    template <
        typename language_t>
    struct some_data 
    {
        language_t some_id ; 
        language_t step_id ;
        size_t min { 0 }, max { std::numeric_limits<size_t>::max() } ;
    } ;

 
    template <
        typename language_t, 
        size_t ids_size_c>
    struct sequence_data 
    {
        language_t sequence_id ;
        std::array<language_t, ids_size_c> step_ids ;  
    } ;

    
    template <
        typename language_t, 
        size_t ids_size_c>
    struct alternative_data 
    {
        language_t alternative_id ;
        std::array<language_t, ids_size_c> step_ids ;  
    } ;


    template<
        typename language_t, 
        typename ... def_t>
    struct grammar_data 
    {
        language_t start_id ;
        std::tuple<def_t...> defs ;
    } ;

    constexpr auto is_some_def (some_data<auto> const &) { return true ; }
    constexpr auto is_some_def (auto const &) { return false ; }
    template <auto size_c>
    constexpr auto is_sequence_def (sequence_data<auto, size_c> const &) { return true ; }
    constexpr auto is_sequence_def (auto const &) { return false ; }
    template <auto size_c>
    constexpr auto is_alternative_def (alternative_data<auto, size_c> const &) { return true ; }
    constexpr auto is_alternative_def (auto const &) { return false ; }

    constexpr auto
    id_of =
    [] (auto const & def)
    {
        if constexpr (is_some_def(def))
            return def.some_id ; 
        else if constexpr (is_sequence_def(def))
            return def.sequence_id ;
        else if constexpr (is_alternative_def(def))
            return def.alternative_id ;
    } ;

    constexpr auto 
    is_terminal (
        auto id, 
        auto const & grammar)
    {        
        return 
        std::apply (
            [id] (auto && ... defs) 
            { return (true && ... && (id != id_of(defs))) ; },
            grammar.defs) ;
    }

    constexpr auto
    some (
        auto some_id, 
        auto step_id, 
        auto min, 
        auto max) 
    {
        return 
        some_data<decltype(some_id)> 
        { some_id, step_id, min, max } ;
    }

    constexpr auto
    zero_more (
        auto some_id, 
        auto step_id)
    {
        return 
        some (some_id, step_id, 0u, std::numeric_limits<size_t>::max()) ;
    }

    constexpr auto
    one_more (
        auto some_id, 
        auto step_id)
    {
        return 
        some (some_id, step_id, 1u, std::numeric_limits<size_t>::max()) ;
    }
    constexpr auto
    sequence_of (
        auto sequence_id, 
        auto ... step_ids)
    {
        return
        sequence_data<decltype(sequence_id), sizeof...(step_ids)>
        { sequence_id, std::array {step_ids...} } ;
    }

    constexpr auto
    one_of (
        auto alternative_id, 
        auto ... step_ids)
    {
        return
        alternative_data<decltype(alternative_id), sizeof...(step_ids)>
        { alternative_id, std::array {step_ids...} } ;
    }

    constexpr auto 
    grammar (
        auto start_id, 
        auto const & ... defs) 
    {
        return
        grammar_data <decltype(start_id), std::decay_t<decltype(defs)>...> 
        { start_id, std::make_tuple (defs...) } ;
    }
}

#endif
