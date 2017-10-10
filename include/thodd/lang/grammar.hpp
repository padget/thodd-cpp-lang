#ifndef __THODD_LANG_GRAMMAR_HPP__
#  define __THODD_LANG_GRAMMAR_HPP__

#  include <array>
#  include <map>
#  include <list>
#  include <stack>
#  include <algorithm>
#  include <limits>

namespace
thodd::lang::syntax
{
    enum struct production_operator 
    { and_, or_, some } ;

    // SOME
    inline constexpr auto
    some = 
    [] (auto id) 
    {
        return
        [id] (auto min, auto max)
        { return tuple (production_operator::some, id, min, max) ; } ;
    } ;

    inline constexpr auto
    one_more = 
    [] (auto id) 
    { return some (id) (1u, std::numeric_limits<size_t>::max()) ; } ;

    inline constexpr auto
    zero_more = 
    [] (auto id) 
    { return some (id) (0u, std::numeric_limits<size_t>::max()) ; } ;

    inline constexpr auto
    some_min = bind (nth, $0, val(idx2)) ; 

    inline constexpr auto
    some_max = bind (nth, $0, val(idx3)) ;
    
    inline constexpr auto
    some_id = bind (nth, $0, val(idx1)) ;

    inline constexpr auto
    some_op = bind (nth, $0, val(idx0)) ;



    // ONE_OF
    inline constexpr auto
    one_of = 
    [] (auto ... ids) 
    { return tuple (production_operator::or_, tuple(ids...)) ; } ;

    inline constexpr auto
    one_of_ids = bind (nth, $0, val(idx1)) ;

    inline constexpr auto
    one_of_op = bind (nth, $0, val(idx0)) ;



    // SEQUENCE_OF
    inline constexpr auto
    sequence_of = 
    [] (auto ... ids) 
    { return tuple (production_operator::and_, ids...) ; } ;

    inline constexpr auto
    sequence_of_ids = bind (nth, $0, val(idx1)) ;

    inline constexpr auto
    sequence_of_op = bind (nth, $0, val(idx0)) ;


    // DEF
    inline constexpr auto
    def_operator = bind (nth, $0, val(idx0)) ;


    // IS
    inline constexpr auto
    is = 
    [] (auto id, auto && def)
    { return tuple (id, std::forward<decltype(def)>(def)) ; } ;

    inline constexpr auto
    is_id = bind(nth, $0, val(idx0)) ;

    inline constexpr auto
    is_operator = bind(nth, bind(nth, $0, val(idx1)), val(idx0)) ;

   
    // GRAMMAR
    inline constexpr auto
    grammar = 
    [] (auto start, auto && ... iss)
    { return tuple (start, std::forward<decltype(iss)>(iss)...) ; } ;

    inline constexpr auto
    grammar_start = 
    [] (auto && grammar)
    { return nth (std::forward<decltype(grammar)>(grammar), idx0) ; } ;

    inline constexpr auto
    grammar_iss = 
    [] (auto const & grammar)
    {
        return 
        grammar (
            [] (auto && start, auto && ... iss) 
            { return tuple(std::forward<decltype(iss)>(iss)...) ; }) ;
    } ;


    inline constexpr auto
    is_terminal = 
    [] (auto id, auto const & grammar)
    {
        return 
        accumulate (
            transform (grammar_iss(grammar), bind(is_id, $0)), 
            true, and_($0, not_equal(val(id), $1))) ;
    } ;


    inline constexpr auto 
    get_operator_by_id =
    [] (auto id, auto && grammar)
    {
        return 
        accumulate (
            grammar_iss(grammar),
            production_operator{},
            tern(
                equal(bind(is_id, $1)), 
                bind(is_operator, $1), 
                $0));
    } ;
}

#endif
