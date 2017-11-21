#ifndef __thodd_lang2_grammar_hpp__
#  define __thodd_lang2_grammar_hpp__

#  include <type_traits>
#  include <utility>
#  include <limits>
#  include <tuple>

#  include <thodd/range.hpp>

namespace
thodd::lang::syntax 
{
  template <
    typename begin_t, 
    typename end_t>
  struct tree_item 
  {
    size_t depth { 0 } ;
    thodd::range<begin_t, end_t> data ;
  } ;

  make_tree_item (
    size_t depth, 
    auto && begin_t, 
    auto && end_t)
  {
    return 
    tree_item<
      std::decay_t<decltype(begin_t)>, 
      std::decay_t<decltype(end_t)>>
    { std::forward<decltype(begin_t)>(begin_t), 
      std::forward<decltype(end_t)>(end_t) } ;
  }
}


namespace 
thodd::lang::syntax
{
  template <
    typename language_t, 
    size_t size_c>
  struct sequence_of_data
  {
    language_t id ;
    thodd::array<language_t, size_c> ids ;
  } ; 

  constexpr auto 
  sequence_of (
    auto id, auto ... ids)
  {
    return 
    sequence_of_data<decltype(id), sizeof...(ids)>
    { id, { ids... } } ; 
  }

  template <
    typename language_t, 
    size_t size_c>
  struct one_of_data
  {
    language_t id ;
    thodd::array<language_t, size_c> ids ;
  } ;

  constexpr auto
  one_of (
    auto id, auto ... ids)
  {
    return
    one_of_data<decltype(id), sizeof...(ids)>
    { id, { ids... } } ;
  }

  template < 
    typename language_t>
  struct some_data
  {
    language_t id ; 
    language_t some_id ;
    size_t min, max ;
  } ;

  constexpr auto 
  some (
    auto id, auto some_id)
  {
    return 
    [id, some_id] (size_t min, size_t max)
    {
      return 
      some_data<decltype(id)>
      { id, some_id, min, max } ;
    } ;
  }

  constexpr auto
  zero_more (
    auto id, auto some_id)
  {
    return 
    some(id, some_id)
      (0, std::numeric_limits<size_t>::max()) ;
  }

  constexpr auto
  one_more (
    auto id, auto some_id)
  {
    return 
    some(id, some_id)
      (1, std::numeric_limits<size_t>::max()) ;
  }

  template <
    typename language_t,
    typename ... rule>
  struct grammar_data
  {
    language_t id ;
    std::tuple<rule...> rules ;
  } ;

  constexpr auto 
  grammar(auto id, auto && ... rule)
  {
    return 
    grammar_data<decltype(id), std::decay_t<decltype(rule)>...>
    { id, { std::make_tuple(std::forward<decltype(rule)>(rule)...) } } ;
  }
}

#endif