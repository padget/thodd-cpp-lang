#include <iostream>
#include <thodd/range.hpp>
#include <thodd/functional.hpp>
#include <vector>
#include <string>

using str = std::string ;

/// Basic boolean regex validators
[[pure]]
inline constexpr auto is_between =
[] (auto min, auto max) {
  return
  [=] (auto & cursor) {
    return
    min <= thodd::value_of(cursor) && thodd::value_of(cursor) <= max ;
  } ;
} ;

[[pure]]
inline constexpr auto is_exactly =
[] (auto sym) {
  return
  [sym] (auto & cursor) {
    return
    value_of (cursor) == sym ;
  } ;
} ;

/// Boolean regex validators
[[pure]]
inline constexpr auto is_alpha = thodd::or_(
  is_between ('a', 'z'),
  is_between ('A', 'Z'),
  is_exactly ('_')) ;

[[pure]]
inline constexpr auto is_num = is_between ('1', '9') ;

[[pure]]
inline constexpr auto is_alpha_num = thodd::or_(is_num, is_alpha) ;

[[pure]]
inline constexpr auto is_left_bracket = is_exactly('(') ;

[[pure]]
inline constexpr auto is_right_bracket = is_exactly(')') ;

[[pure]]
inline constexpr auto is_left_brace = is_exactly('{') ;

[[pure]]
inline constexpr auto is_right_brace = is_exactly('}') ;

[[pure]]
inline constexpr auto is_left_sbracket = is_exactly('[') ;

[[pure]]
inline constexpr auto is_right_sbracket = is_exactly(']') ;

[[pure]]
inline constexpr auto is_colon = is_exactly(':') ;

[[pure]]
inline constexpr auto is_semi_colon = is_exactly(';') ;

[[pure]]
inline constexpr auto make_optional_if =
[] (auto && predicate, auto && transformer) {
  return
  [&predicate, &transformer] (auto && obj) {
    return thodd::make_optional_if (
      std::forward<decltype(obj)>(obj),
      std::forward<decltype(predicate)>(predicate),
      std::forward<decltype(transformer)>(transformer)) ;
  } ;
} ;

[[pure]]
inline constexpr auto
not_equals =
[] (auto && l, auto && r) {
  return
  thodd::not_equals (
    std::forward<decltype(l)>(l),
    std::forward<decltype(r)>(r)) ;
} ;

[[pure]]
inline constexpr auto
make_range =
[] (auto && begin, auto && end) {
  return
  thodd::make_range (
    std::forward<decltype(begin)>(begin),
    std::forward<decltype(end)>(end)) ;
} ;

[[pure]]
inline constexpr auto
next_if_is = 
[] (auto && is, auto const & stream) {
  return 
  thodd::bind(
    thodd::next_if(is), 
    thodd::$0, 
    thodd::val(thodd::end(stream))) ; 
} ;

[[pure]]
auto const make_range_if_not_begin =
[] (auto const & stream) {
  return 
  make_optional_if(
    thodd::bind(not_equals, thodd::val(thodd::begin(stream)), thodd::$0),
    thodd::bind(make_range, thodd::val(thodd::begin(stream)), thodd::$0)) ;
} ;

enum class thodd_term
{
  identifier, lbracket, rbracket, comma, twopoints,
  purekw, impurekw, builderkw
} ;

[[pure]]
inline constexpr auto
identifier_rx =
[] (auto const & stream) {
  return
  thodd::compose(
    next_if_is (is_alpha, stream), 
     thodd::bind(
      thodd::next_while(
        thodd::and_(
          is_alpha_num,
          thodd::bind(not_equals, thodd::val(thodd::begin(stream)), thodd::$0))),
       thodd::$0, thodd::val(thodd::end(stream))), 
    make_range_if_not_begin(stream)) (thodd::begin(stream)) ;
} ;

[[pure]]
inline constexpr auto 
lbracket_rx = 
[] (auto const & stream) { 
  return 
  thodd::compose (
    next_if_is (is_left_bracket, stream), 
    make_range_if_not_begin(stream)) (thodd::begin(stream)) ;
} ;

[[pure]]
inline constexpr auto 
rbracket_rx = 
[] (auto const & stream) { 
  return 
  thodd::compose (
    next_if_is (is_right_bracket, stream), 
    make_range_if_not_begin(stream)) (thodd::begin(stream)) ;
} ;

[[pure]]
inline constexpr auto 
colon_rx = 
[] (auto const & stream) { 
  return 
  thodd::compose (
    next_if_is (is_colon, stream), 
    make_range_if_not_begin(stream)) (thodd::begin(stream)) ;
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







// pure pow (val : int, exp : int) -> int
//   __add (l, r)


int main(int argc, char** argv)
{
  auto && stream = thodd::make_string ("coucou") ;
  auto && cursor = thodd::begin (stream) ; 
  
 thodd::if_exists(
    identifier_rx(thodd::make_string("coucou")),
    [] (auto&&) { std::cout << "ca match !!" << std::endl ; }) ;


  // std::vector<std::string> args { argv, argv + argc } ;
  // thodd::for_each(
  //     args, [](auto const & item){std::cout << item << std::endl ; }) ;

  // thodd::for_each(
  //     thodd::filter(
  //         args, [] (auto && item) { return item == "--source"; }),
  //     [] (auto && item) { std::cout << item << std::endl ; }) ;
}