#ifndef __THODD_LANG_GRAMMAR_HPP__
#  define __THODD_LANG_GRAMMAR_HPP__

#  include <array>
#  include <map>
#  include <vector>
#  include <stack>
#  include <algorithm>

namespace
thodd::lang 
{
    enum struct production_operator 
    { and_, or_, some } ;


    template <
        typename language_t>
    struct definition 
    {
        production_operator op ;

        std::vector<language_t> ids ;
    } ;


    template <
        typename language_t>
    inline auto 
    make_definition (
        production_operator op, 
        std::vector<language_t> const & ids)
    {
        return 
        definition<language_t> 
        { op, ids } ;
    }


    template <
        typename language_t>
    inline auto 
    make_definition (
        production_operator op, 
        language_t first_id, 
        auto ... next_id)
    {
        return 
        make_definition( 
            op, std::vector { first_id, next_id... } ) ;
    }


    

    inline auto 
    and_def (
        auto first_id, 
        auto ... next_id)
    {        
        return 
        make_definition(
            production_operator::and_, 
            first_id, next_id...) ;
    }


    constexpr auto 
    or_def (
        auto first_id, 
        auto ... next_id)
    {
        return 
        make_definition (
            production_operator::or_, 
            first_id, next_id...) ;
    }
           

    constexpr auto 
    some_def (
        auto id)
    {
        return
        make_definition (
            production_operator::some, id) ;
    }




    template <
        typename language_t> 
    struct production 
    {
        language_t id ;
        production_operator op ;
        std::vector<language_t> ids ;
    } ;


    template <
        typename language_t>
    inline auto 
    is (
        language_t id, 
        definition<language_t> def)
    {
        return 
        production<language_t> { id, def.op, def.ids } ;
    }


    template <
        typename language_t>
    struct runtime_grammar
    {
        using production_t = production<language_t> ;
        using dictionary_t = std::map<language_t, production_t> ;
        using terminals_t = std::vector<language_t> ;

        language_t start ;
        dictionary_t dictionary ;
    } ;




    template <
        typename language_t>
    auto grammar (
        language_t start, 
        auto const & ... production) 
    {
        using grammar_t = runtime_grammar<language_t> ;
        using dictionary_t = typename grammar_t::dictionary_t ;

        auto dictionary = dictionary_t { { production.id, production } ... } ;
    
        return 
        grammar_t { start, dictionary } ; 
    }


    constexpr auto
    is_terminal (
        auto const & id_terminal,
        auto const & grammar)
    {
        return 
        grammar.dictionary.count(id_terminal) == 0 ;
    }

    constexpr auto const &
    production_of (
        auto const & id_production, 
        auto const & grammar)
    {
        return 
        grammar.dictionary.at(id_production) ; 
    }

    constexpr auto const &
    operator_of (
        auto const & id_production, 
        auto const & grammar)
    {
        return 
        production_of (id_production, grammar).op ;
    }



    constexpr auto 
    check_terminal (
        auto const & id,
        auto const & grammar, 
        auto begin, auto end)
    {
        return 
        begin != end 
        && is_terminal (id, grammar)
        && *begin == id ;  
    }
}




#define THODD_LANG_OPERATOR_FOR(language_t)                                                     \
                                                                                                \
inline auto                                                                                     \
operator <= (language_t id, thodd::lang::definition<language_t> const & def)                    \
{ return thodd::lang::is (id, def) ; }                                                          \
                                                                                                \
inline auto                                                                                     \
operator * (language_t id)                                                                      \
{ return thodd::lang::some_def (id) ; }                                                         \
                                                                                                \
inline auto                                                                                     \
operator > (language_t id, language_t id2)                                                      \
{ return thodd::lang::and_def (id, id2) ; }                                                     \
                                                                                                \
inline auto                                                                                     \
operator > (thodd::lang::definition<language_t> const & left, language_t id2)                   \
{                                                                                               \
    auto ids = left.ids ;                                                                       \
    ids.push_back(id2) ;                                                                        \
                                                                                                \
    return                                                                                      \
    thodd::lang::make_definition(                                                               \
        thodd::lang::production_operator::and_, ids) ;                                          \
}                                                                                               \
                                                                                                \
inline auto                                                                                     \
operator > (                                                                                    \
    thodd::lang::definition<language_t> const & left,                                           \
    thodd::lang::definition<language_t> const & right)                                          \
{                                                                                               \
    auto ids = left.ids ;                                                                       \
    std::copy (right.ids.begin(), right.ids.end(), ids.end()) ;                                 \
    return                                                                                      \
    thodd::lang::make_definition(                                                               \
        thodd::lang::production_operator::and_, ids) ;                                          \
}                                                                                               \
                                                                                                \
inline auto                                                                                     \
operator | (language_t id, language_t id2)                                                      \
{ return thodd::lang::or_def (id, id2) ; }                                                      \
                                                                                                \
inline auto                                                                                     \
operator | (thodd::lang::definition<language_t> const & left, language_t id2)                   \
{                                                                                               \
    auto ids = left.ids ;                                                                       \
    ids.push_back(id2) ;                                                                        \
                                                                                                \
    return                                                                                      \
    thodd::lang::make_definition(                                                               \
        thodd::lang::production_operator::or_, ids) ;                                           \
}                                                                                               \
                                                                                                \
inline auto                                                                                     \
operator | (                                                                                    \
    thodd::lang::definition<language_t> const & left,                                           \
    thodd::lang::definition<language_t> const & right)                                          \
{                                                                                               \
    auto ids = left.ids ;                                                                       \
    std::copy (right.ids.begin(), right.ids.end(), ids.end()) ;                                 \
    return                                                                                      \
    thodd::lang::make_definition(                                                               \
       thodd::lang::production_operator::or_, ids) ;                                            \
}                                                                                               \




#endif