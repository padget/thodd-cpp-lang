#ifndef __THODD_LANG_GRAMMAR_HPP__
#  define __THODD_LANG_GRAMMAR_HPP__

namespace
thodd::lang 
{

    enum struct production_operator 
    { and_, or_, some } ;

    
    namespace 
    detail 
    {
        template <auto size_c>
        using ids_container_t = std::array <language_t, size_c> ;
        using operator_t = production_operator ;
    }


    template <
        typename language_t, 
        auto size_c, 
        detail::operator_t op>
    struct definition 
    {
        detail::ids_container_t <and_size_c> ids ;
    } ;



    template <
        typename language_t, 
        auto and_size_c>
    using and_definition = definition<language_t, and_size_c, production_operator::and_op> ;

    template <
        typename language_t, 
        auto or_size_c>
    using or_definition = definition<language_t, or_size_c, production_operator::or_op> ;
    
    template <
        typename language_t>
    using some_definition = definition<language_t, 1, production_operator::some_op> ;



    template <
        detail::operator_t op_c,
        typename language_t>
    constexpr auto 
    make_definition (
        language_t first_id, 
        auto ... next_id)
    {
        return 
        definition<language_t, 1 + sizeof...(next_id), op_c>
        { { first_id, next_id... } } ;
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
        auto definition_size_c>
    constexpr auto 
    is (
        language_t id, 
        definition<language_t, definition_size_c> def)
    {
        using production_t = production <language_t, definition_size_c> ;

        return production_t { id, def.op, def.ids } ;
    }

    

    template <
        typename language_t, 
        typename ... production_t>
    struct productions ;
}


















#define THODD_LANG_OPERATOR_FOR(language_t) \
template <auto size_c> \
constexpr auto \
operator <= (language_t id, thodd::lang::definition<language_t, size_c> def) \
{ return thodd::lang::is (id, def) ; } \
\
constexpr auto \
operator * (language_t id) \
{ return thodd::lang::some_op (id) ; } \
\
constexpr auto \
operator > (language_t id, language_t id2) \
{ return thodd::lang::and_op (id, id2) ; } \
\
constexpr auto \
operator | (language_t id, language_t id2) \
{ return thodd::lang::or_op (id, id2) ; } \
\




#endif