#ifndef __THODD_LANG_GRAMMAR_HPP__
#  define __THODD_LANG_GRAMMAR_HPP__

#  include <array>
#  include <map>
#  include <list>
#  include <stack>
#  include <algorithm>
#  include <limits>

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
        std::list<language_t> ids ;
        size_t min { 0 }, max { 1 } ;
    } ;


    template <
        typename language_t>
    inline auto 
    make_definition (
        production_operator op, 
        std::list<language_t> const & ids)
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
            op, std::list { first_id, next_id... } ) ;
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
        auto id, 
        size_t min, 
        size_t max)
    {
        auto && def = make_definition (production_operator::some, id) ;
        def.min = min ;
        def.max = max ;
        
        return
        def ;
    }




    template <
        typename language_t> 
    struct production 
    {
        language_t id ;
        production_operator op ;
        std::list<language_t> ids ;
        size_t min ;
        size_t max ;

    } ;


    template <
        typename language_t>
    inline auto 
    is (
        language_t id, 
        definition<language_t> def)
    {
        return 
        production<language_t> { id, def.op, def.ids, def.min, def.max } ;
    }


    template <
        typename language_t>
    struct basic_grammar
    {
        using dictionary_t = std::map<language_t, production<language_t>> ;

        language_t start ;
        dictionary_t dictionary ;
    } ;




    template <
        typename language_t>
    inline auto 
    grammar (
        language_t start, 
        auto const & ... production) 
    {
        using dictionary_t = typename basic_grammar<language_t>::dictionary_t ;

        auto const dictionary = dictionary_t { { production.id, production } ... } ;
    
        return 
        basic_grammar<language_t>
        { start, dictionary } ; 
    }


    inline auto
    is_terminal (
        auto const id_terminal,
        auto const & grammar)
    {
        return 
        grammar.dictionary.count(id_terminal) == 0 ;
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
{ return thodd::lang::some_def (id, 0, std::numeric_limits<size_t>::max()) ; }                  \
                                                                                                \
inline auto                                                                                     \
operator + (language_t id)                                                                      \
{ return thodd::lang::some_def (id, 1, std::numeric_limits<size_t>::max()) ; }                  \
                                                                                                \
inline auto                                                                                     \
operator - (language_t id)                                                                      \
{ return thodd::lang::some_def (id, 0, 1) ; }                                                   \
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