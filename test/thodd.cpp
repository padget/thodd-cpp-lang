#include <iostream>
#include <thodd/range.hpp>
#include <vector>
#include <string>

using str = std::string ;

[[pure]]
inline constexpr auto
native_types = 
[] 
{
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
[] (auto const & str, auto const & types)
{
  return 
  thodd::count (types, str) == 1 ;
} ;

[[pure]]
inline constexpr auto
native_functions = 
[] 
{
  return 
  thodd::collect (  
    thodd::project (
      thodd::make_array (
        str("__add"), str("__sub"), str("__div"), str("__mult"), str("__mod"), 
        str("__less"), str("__equal"), str("__greater"), 
        str("__not"), str("__and"), str("__or"), 
      [] (auto const & name) { return str("thodd::") + name ; }), 
    thodd::to_array) ; 
} ;

[[pure]]
inline constexpr auto
is_function = 
[] (auto const & str, auto const & functions)
{
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
one_of = 
[] (auto const & ... cases)
{
  return 
  [=cases...] (auto const & tokens)
  {
    return 
    thodd::make_optional_if(
      tokens, 
      [&cases...] (auto const & tokens) 
      { return (thodd::value_of(thodd::begin(tokens)) == cases || ...) ; }, 
      [] (auto const & tokens) 
      { return thodd::make_range(thodd::next(thodd::begin(tokens)), thodd::end(tokens)) ; }) ;     
  } ;
} ;


[[pure]]
inline constexpr auto
sequence_of = 
[] (auto const & ... items)
{
  return 
  [=items...] (auto const & tokens)
  {
    return 
    thodd::make_optional_if(
      tokens, 
      [&items...] (auto const & tokens) 
      { return (thodd::value_of(thodd::begin(tokens)) == items && ...) ; }, 
      [] (auto const & tokens) 
      { return thodd::make_range(thodd::next(thodd::begin(tokens)), thodd::end(tokens)) ; }) ;     
  } ;
} ;

inline constexpr auto 
advance_tokens_if =
[] (auto const & tokens, auto predicate) 
{
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


inline constexpr auto 
is_pure_impure_builder =
[] (auto const & tokens) 
{
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

//pure __add (l : int, r : int) -> int
[[pure]]
inline constexpr auto
is_function_declaration = 
[] (auto const & tokens) 
{
  constexpr auto declaration = 
  (thodd_term::purekw|thodd_term::impurekw|thodd_term::builderkw) 
  > thodd_term::identifier > thodd_term::lbracket > 
    *(thodd_term::identifier > thodd_term::twopoints > thodd_term::identifier) > thodd_term::rbracket ;
  
  
  

  return 
  is_pure_impure_builder(tokens) ;
} 

// pure pow (val : int, exp : int) -> int
//   __add (l, r)


int main(int argc, char** argv)
{
    // std::vector<std::string> args { argv, argv + argc } ;
    // thodd::for_each(
    //     args, [](auto const & item){std::cout << item << std::endl ; }) ;

    // thodd::for_each(
    //     thodd::filter(
    //         args, [] (auto && item) { return item == "--source"; }), 
    //     [] (auto && item) { std::cout << item << std::endl ; }) ;
}