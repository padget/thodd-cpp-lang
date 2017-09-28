#ifndef __THODD_LANG_GRAMMAR_HPP__
#  define __THODD_LANG_GRAMMAR_HPP__

#  include <array>
#  include <map>
#  include <vector>
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
        std::vector<language_t> ids ;
        size_t min { 0 }, max { 1 } ;
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
        std::vector<language_t> ids ;
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

    

    template <
        typename language_t, 
        typename iterator_t>
    std::tuple<bool, iterator_t> 
    check_some (
        language_t some_id, 
        basic_grammar<language_t> const & grammar, 
        iterator_t begin, 
        iterator_t const end) 
    { 
        auto cpt = 0u ;
        auto checked = true ;
        auto local_cursor = begin ; 
        language_t step_id = grammar.dictionary.at(some_id).ids[0] ;
        size_t const min = grammar.dictionary.at(some_id).min ;
        size_t const max = grammar.dictionary.at(some_id).max ;
        
        while (checked && cpt <= max)
        {
            auto && [step_checked, step_cursor] = check(step_id, grammar, local_cursor, end) ;
            
            if (checked = step_checked)
            {
                ++ cpt ;
                local_cursor = step_cursor ;
            } 
        }

        checked = min <= cpt && cpt <= max ; 

        return 
        { checked, checked ? local_cursor : begin } ;
    }


    template <
        typename language_t, 
        typename iterator_t>
    std::tuple<bool, iterator_t>
    check_and (
        language_t and_id, 
        basic_grammar<language_t> const & grammar, 
        iterator_t begin, 
        iterator_t const end)
    {
        auto local_cursor = begin ; 
        auto checked = true ;
        auto const & step_ids = grammar.dictionary.at(and_id).ids ;

        for (auto && step_id : step_ids)
        {
            auto && [step_checked, step_cursor] = check(step_id, grammar, local_cursor, end) ;

            if (checked = step_checked) 
                local_cursor = step_cursor ; 
            else break ;
        }  
        
        return 
        { checked, checked ? local_cursor : begin } ;
    }

    template <
        typename language_t, 
        typename iterator_t>
    std::tuple<bool, iterator_t> 
    check_or (
        language_t or_id, 
        basic_grammar<language_t> const & grammar, 
        iterator_t begin, 
        iterator_t const end)
    {
        auto local_cursor = begin ; 
        auto checked = false ;
        auto const & step_ids = grammar.dictionary.at(or_id).ids ;
        
        for (auto && step_id : step_ids)
        {
            auto && [step_checked, step_cursor] = check(step_id, grammar, local_cursor, end) ;

            if (checked = step_checked)
            {    
                local_cursor = step_cursor;
                break ;
            }
        }

        return 
        { checked, checked ? local_cursor : begin } ; 
    }

    template <
        typename language_t, 
        typename iterator_t>
    std::tuple<bool, iterator_t>
    check (
        language_t id, 
        basic_grammar<language_t> const & grammar, 
        iterator_t begin, 
        iterator_t const end)
    {
        if (is_terminal (id, grammar))
        {   
            auto is_same = 
                begin != end 
                && *begin == id ; 
            
            return 
            { is_same, is_same ? ++begin : begin } ;
        }
        else switch (grammar.dictionary.at(id).op)
        {
            case production_operator::some : 
                return check_some (id, grammar, begin, end) ;
            case production_operator::and_ : 
                return check_and (id, grammar, begin, end) ;
            case production_operator::or_  : 
                return check_or (id, grammar, begin, end) ;
        }
    }    


    template <
        typename language_t>
    auto 
    check (
        basic_grammar<language_t> const & grammar, 
        auto begin,
        auto const end)
    {
        return 
        check (grammar.start, grammar, begin, end) ;
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