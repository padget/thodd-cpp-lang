#ifndef __THODD_LANG_GRAMMAR_HPP__
#  define __THODD_LANG_GRAMMAR_HPP__

#  include <array>
#  include <map>
#  include <vector>
#include <stack>
#  include <algorithm>

namespace
thodd::lang 
{

    enum struct production_operator 
    { and_, or_, some } ;

    
    namespace 
    detail 
    {
        template <typename language_t, auto size_c>
        using ids_container_t = std::array <language_t, size_c> ;
        using operator_t = production_operator ;
    }


    template <
        typename language_t, 
        auto size_c, 
        detail::operator_t op>
    struct definition 
    {
        detail::ids_container_t <language_t, size_c> ids ;
    } ;



    template <
        typename language_t, 
        auto and_size_c>
    using and_definition = definition<language_t, and_size_c, production_operator::and_> ;

    template <
        typename language_t, 
        auto or_size_c>
    using or_definition = definition<language_t, or_size_c, production_operator::or_> ;
    
    template <
        typename language_t>
    using some_definition = definition<language_t, 1, production_operator::some> ;


    
    template <
        detail::operator_t op_c, 
        typename language_t, 
        auto size_c>
    constexpr auto 
    make_definition(
        std::array<language_t, size_c> ids)
    {
        return 
        definition <language_t, size_c, op_c> { ids } ;
    
    }

    template <
        detail::operator_t op_c,
        typename language_t>
    constexpr auto 
    make_definition (
        language_t first_id, 
        auto ... next_id)
    {
        return 
        make_definition<op_c> (
            std::array { first_id, next_id... } ) ;
    }


    

    constexpr auto 
    and_def (
        auto first_id, 
        auto ... next_id)
    {        
        return 
        make_definition <detail::operator_t::and_> (first_id, next_id ...) ;
    }

    constexpr auto 
    or_def (
        auto first_id, 
        auto ... next_id)
    {
        return 
        make_definition <detail::operator_t::or_> (first_id, next_id ...) ;
    }
           
    constexpr auto 
    some_def (
        auto id)
    {
        return
        make_definition <detail::operator_t::some> (id) ;
    }

    



    template <
        typename language_t,
        auto production_size_c> 
    struct production 
    {
        using ids_container_t = std::array<language_t, production_size_c> ;
        using operator_t = production_operator ;

        language_t id ;
        operator_t op ;
        ids_container_t ids ;
    } ;


    template <
        typename language_t, 
        auto definition_size_c, 
        auto op_c>
    constexpr auto 
    is (
        language_t id, 
        definition<language_t, definition_size_c, op_c> def)
    {
        using production_t = production <language_t, definition_size_c> ;

        return production_t { id, op_c, def.ids } ;
    }



    template <
        typename language_t>
    struct runtime_production 
    {
        language_t id ; 
        production_operator op ;
        std::vector<language_t> ids ;
    } ;

    

    template <
        typename language_t>
    struct runtime_grammar
    {
        using production_t = runtime_production<language_t> ;
        using productions_t = std::vector<production_t> ;
        using dictionary_t = std::map<language_t, production_t> ;
        using terminals_t = std::vector<language_t> ;

        language_t start ;
        dictionary_t dictionary ;
        terminals_t terminals ;
    } ;


    template <
        typename language_t, 
        auto size_c>
    auto 
    to_runtime (
        production<language_t, size_c> prod) 
    {
        std::vector<language_t> ids (std::distance(prod.ids.begin(), prod.ids.end())) ;
        std::copy (prod.ids.begin(), prod.ids.end(), ids.begin()) ;

        return 
        runtime_production<language_t> 
        { prod.id, prod.op, ids } ;
    }


    template <typename language_t>
    auto grammar (
        language_t start, 
        auto const & ... production) 
    {
        using grammar_t = runtime_grammar<language_t> ;
        using dictionary_t = typename grammar_t::dictionary_t ;

        auto dictionary = std::map<language_t, runtime_production<language_t>> { { production.id, to_runtime(production) } ... } ;
    
        return 
        grammar_t { start, dictionary } ; 
    }

    namespace detail
    {
        template <typename type_t, auto size_c, auto size2_c> 
        constexpr auto
        concat (
            std::array<type_t, size_c> const & left, 
            std::array<type_t, size2_c> const & right)
        {
            std::array<type_t, size_c + size2_c> concated {} ;
            
            auto cursor = concated.begin() ;

            for (auto && item : left) *(cursor++) = item ; 
            for (auto && item : right) *(cursor++) = item ;

            return concated ;
        }
    }

    constexpr auto 
    check_terminal (
        auto const & id,
        auto const & grammar, 
        auto begin, auto end)
    {
        return 
        begin != end 
        && grammar.dictionary.count(id) == 0 
        && *begin == id ;  
    }

    // template <
    //     typename language_t>
    // struct trace
    // {
    //     language_t id ; 
    //     size_t index ;
    //     production_operator op ;
    // } ;


    // constexpr auto 
    // check (
    //     auto const & grammar,
    //     auto begin, auto end)
    // {
    //     using id_t = decltype(grammar.start) ;
    //     std::stack<trace<id_t>> id_stack ;
        
    //     id_stack.push ( { grammar.start, 0 } ) ;

    //     while (begin != end)
    //     {
    //         while (grammar.dictonary.count(id_stack.top()) != 0)
    //         {
    //             auto definition = grammar.dictionary.at(id_stack.top()) ;
    //             id_stack.push( { definition.ids[index], index, definition.op } ) ;
    //             std::cout << 
    //         }
    //     }
    // }
}




#define THODD_LANG_OPERATOR_FOR(language_t)                                         \
template <auto size_c, auto op_c>                                                   \
constexpr auto                                                                      \
operator <= (language_t id, thodd::lang::definition<language_t, size_c, op_c> def)  \
{ return thodd::lang::is (id, def) ; }                                              \
                                                                                    \
constexpr auto                                                                      \
operator * (language_t id)                                                          \
{ return thodd::lang::some_def (id) ; }                                             \
                                                                                    \
constexpr auto                                                                      \
operator > (language_t id, language_t id2)                                          \
{ return thodd::lang::and_def (id, id2) ; }                                         \
                                                                                    \
template <auto size_c>                                                              \
constexpr auto                                                                      \
operator > (thodd::lang::and_definition<language_t, size_c> left, language_t id2)   \
{                                                                                   \
                                                                                    \
    return                                                                          \
    thodd::lang::make_definition<thodd::lang::detail::operator_t::and_>(            \
        thodd::lang::detail::concat(left.ids, std::array {id2} )) ;                 \
}                                                                                   \
                                                                                    \
template <auto size_c, auto size2_c>                                                \
constexpr auto                                                                      \
operator > (                                                                        \
    thodd::lang::and_definition<language_t, size_c> left,                           \
    thodd::lang::and_definition<language_t, size2_c> right)                         \
{                                                                                   \
    return                                                                          \
    thodd::lang::make_definition<thodd::lang::detail::operator_t::and_>(            \
        thodd::lang::detail::concat(left.ids, right.ids)) ;                         \
}                                                                                   \
                                                                                    \
constexpr auto                                                                      \
operator | (language_t id, language_t id2)                                          \
{ return thodd::lang::or_def (id, id2) ; }                                          \
                                                                                    \
template <auto size_c>                                                              \
constexpr auto                                                                      \
operator | (thodd::lang::or_definition<language_t, size_c> left, language_t id2)    \
{                                                                                   \
                                                                                    \
   return                                                                           \
    thodd::lang::make_definition<thodd::lang::detail::operator_t::or_>(             \
        thodd::lang::detail::concat(left.ids, std::array {id2} )) ;                 \
}                                                                                   \
                                                                                    \
template <auto size_c, auto size2_c>                                                \
constexpr auto                                                                      \
operator | (                                                                        \
    thodd::lang::or_definition<language_t, size_c> left,                            \
    thodd::lang::or_definition<language_t, size2_c> right)                          \
{                                                                                   \
    return                                                                          \
    thodd::lang::make_definition<thodd::lang::detail::operator_t::or_>(             \
        thodd::lang::detail::concat(left.ids, right.ids)) ;                         \
}                                                                                   \




#endif