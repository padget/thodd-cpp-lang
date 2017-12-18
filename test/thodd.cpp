#include <iostream>
#include <thodd/range.hpp>
#include <thodd/functional.hpp>
#include <vector>
#include <string>

using str = std::string ;

/// Basic boolean regex validators
[[pure]]
auto is_between = 
[] (auto min, auto max) {
  return 
  [=] (auto const & cursor) {
    return 
    min <= thodd::value_of(cursor) && thodd::value_of(cursor) <= max ;
  } ;
} ;

[[pure]]
auto is_exactly = 
[] (auto sym) {
  return 
  [sym] (auto const & cursor) {
    return 
    value_of (cursor) == sym ; 
  } ;
} ;

/// Boolean regex validators
[[pure]]
auto is_alpha = thodd::or_(
  is_between ('a', 'z'), 
  is_between ('A', 'Z'), 
  is_exactly ('_')) ;

[[pure]]
auto is_num = is_between ('1', '9') ;

[[pure]]
auto is_alpha_num = thodd::or_(is_num, is_alpha) ;

[[pure]]
auto is_left_bracket = is_exactly('(') ;

[[pure]]
auto is_right_bracket = is_exactly(')') ;

[[pure]]
auto is_left_brace = is_exactly('{') ;

[[pure]]
auto is_right_brace = is_exactly('}') ;

[[pure]]
auto is_left_sbracket = is_exactly('[') ;

[[pure]]
auto is_right_sbracket = is_exactly(']') ;

[[pure]]
auto is_colon = is_exactly(':') ;

[[pure]]
auto is_semi_colon = is_exactly(';') ;

[[pure]]
inline constexpr auto 
identifier_rx = 
[] (auto const & stream) {
  [[local]]
  auto cursor = thodd::begin(stream) ; 

  thodd::next_if (is_alpha) (cursor, thodd::end(stream)) ;
  thodd::next_while (
    thodd::and_(
      is_alpha_num, 
      [&stream] (auto const & cursor) {
        return 
        thodd::not_equals(cursor, thodd::begin(stream)) ; 
      })) (cursor, thodd::end(stream)) ;
  
  return  
  thodd::make_optional_if (
    cursor, 
    [&stream] (auto const & cursor) { 
      return 
      thodd::not_equals(cursor, thodd::begin(stream)) ; 
    },
    [&stream] (auto const & cursor) { 
      return 
      thodd::make_range (
        thodd::begin(stream), 
        cursor) ; 
    }) ;
  
} ;

[[pure]]
inline constexpr auto
native_types = 
[] {
  return 
  thodd::make_array (
    str("byte"), str("char"), str("int"), str("long"), str("bigint"), 
    str("float"), str("double"), str("bigfloat"), 
    str("ubyte"), str("uchar"), str("uint"), str("ulong"), str("ubigint"), 
    str("ufloat"), str("udouble"), str("ubigfloat")) ;
} ;


[[pure]]
inline constexpr auto
is_type = 
[] (auto const & str, auto const & types) {
  return 
  thodd::count (types, str) == 1 ;
} ;

[[pure]]
inline constexpr auto
native_functions = 
[] {
  return 
  thodd::collect (  
    thodd::project (
      thodd::make_array (
        str("__add"), str("__sub"), str("__div"), str("__mult"), str("__mod"), 
        str("__less"), str("__equal"), str("__greater"), 
        str("__not"), str("__and"), str("__or")), 
      [] (auto const & name) { return str("thodd::") + name ; }), 
    thodd::to_list<std::string>) ; 
} ;

[[pure]]
inline constexpr auto
is_function = 
[] (auto const & str, auto const & functions) {
  return 
  count (functions, str) == 1 ;
} ;


enum class thodd_term 
{
  identifier, lbracket, rbracket, comma, twopoints, 
  purekw, impurekw, builderkw
} ;


[[pure]]
inline constexpr auto 
advance_tokens_if =
[] (auto const & tokens, auto predicate) {
  return
  thodd::make_optional_if (
    tokens, 
    predicate, 
    [] (auto const & tokens) {
      return 
      thodd::make_range (
        thodd::next(thodd::begin(tokens)), 
        thodd::end(tokens)) ;
    }) ;
} ;

[[pure]]
inline constexpr auto 
is_pure_impure_builder =
[] (auto const & tokens) {
  return 
  advance_tokens_if (
    tokens,
    [] (auto const & tokens) {
      auto const & token = thodd::value_of(thodd::begin(tokens)) ; 
      return 
      token == thodd_term::purekw 
      || token == thodd_term::impurekw
      || token == thodd_term::builderkw ; 
    }) ;
} ;

[[pure]]
inline constexpr auto 
is_identifier = 
[] (auto const & tokens) 
{
  return 
  advance_tokens_if (
    tokens, 
    [] (auto const & tokens) {
      return 
      thodd::value_of(thodd::begin(tokens)) == thodd_term::identifier ; 
    }) ;
} ;

[[pure]]
inline constexpr auto
is_function_declaration = 
[] (auto const & tokens) 
{ 
  return 
  thodd::if_exists(
    is_pure_impure_builder(tokens), 
    [] (auto && tokens) {
      return thodd::if_exists(
        is_identifier(tokens), 
        [] (auto && tokens) {
          return make_optional(tokens) ;
        });
    }) ;
} ;

// pure pow (val : int, exp : int) -> int
//   __add (l, r)


int main(int argc, char** argv)
{
  constexpr auto func_tokens = thodd::make_array (thodd_term::purekw, thodd_term::identifier) ;
  thodd::if_exists(
    identifier_rx(thodd::make_array('1', 'a', 'b', '_')), 
    [] (auto&&) { std::cout << "ca match !!" << std::endl ; }) ;


    // std::vector<std::string> args { argv, argv + argc } ;
    // thodd::for_each(
    //     args, [](auto const & item){std::cout << item << std::endl ; }) ;

    // thodd::for_each(
    //     thodd::filter(
    //         args, [] (auto && item) { return item == "--source"; }), 
    //     [] (auto && item) { std::cout << item << std::endl ; }) ;
}