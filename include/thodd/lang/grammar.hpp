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
    one_of = 
    [] (auto ... ids) 
    { return tuple (production_operator::or_, ids...) ; } ;

    inline constexpr auto
    sequence_of = 
    [] (auto ... ids) 
    { return tuple (production_operator::and_, ids...) ; } ;

    inline constexpr auto
    get_def_operator =
    [] (auto && def)
    { return nth (std::forward<decltype(def)>(def), idx0) ; } ;

    inline constexpr auto
    get_def_ids = 
    [] (auto && def)
    { return std::forward<decltype(def)>(def)([] (auto op, auto ... ) {  } ) ; } ;

    inline constexpr auto
    is = 
    [] (auto id, auto && def)
    { return tuple (id, std::forward<decltype(def)>(def)) ; } ;

    inline constexpr auto
    get_is_id = 
    [] (auto const & is)
    { return nth (std::forward<decltype(is)>(is), idx0) ; } ;

    inline constexpr auto
    grammar = 
    [] (auto start, auto && ... iss)
    { return tuple (start, std::forward<decltype(iss)>(iss)...) ; } ;

    inline constexpr auto
    get_grammar_start = 
    [] (auto && grammar)
    { return nth (std::forward<decltype(grammar)>(grammar), idx0) ; } ;

    inline constexpr auto
    get_grammar_ids =
    [] (auto const & grammar)
    {
        return 
        grammar (
            [] (auto && start, auto && ... iss) 
            { return tuple(get_is_id(iss)...) ; }) ;
    } ;

    inline constexpr auto
    is_terminal = 
    [] (auto id, auto && grammar)
    {
        return 
        accumulate (
            get_grammar_ids(std::forward<decltype(grammar)>(grammar)), true, 
            ass($0, and_($0, not_equal(val(id), $1)))
        ) ;
    } ;
}

#endif
