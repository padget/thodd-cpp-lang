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
  enum struct node_type 
  { defined, defining } ;

  template <
    typename language_t>
  struct node
  {
    language_t id ;
    node_type type ;
  } ;

  constexpr auto 
  make_node (auto id, node_type type)
  { return node<decltype(id)> { id, type } ; }

  template <
    typename language_t, 
    size_t size_c>
  struct grammar
  {    
    language_t start ;
    thodd::array<node<language_t>, size_c> rules {} ; 
  } ;

  constexpr auto 
  init_grammar (auto start) 
  {
    return 
    grammar<decltype(start), 0u> { start } ;
  }

  template <size_t size_c>
  constexpr auto
  patch_grammar(
    grammar<auto, size_c> && gr, 
    auto id, 
    auto ... ids)
  {
    // TODO faire la fonction concat of two array
    return 
    grammar<decltype(id), size_c + sizeof...(ids) + 1>
    { gr.start, concat(gr.rules, { make_node(id, node_type::defined), make_node(ids, node_type::defining)... }) } ;
  }

  template <typename type_t, size_t size1_c, size_t size2_c>
  constexpr auto concat (
    thodd::array<type_t, size1_c> const & arr1, 
    thodd::array<type_t, size2_c> const & arr2)
  {
    thodd::array<type_t, size1_c + size2_c> arr {} ; 
    
    thodd::for_eachi (arr1, [&arr] (auto && item, auto index) { std::cout << index << item << std::endl ; arr[index] = std::forward<decltype(item)>(item) ; }) ;
    thodd::for_eachi (arr2, [&arr] (auto && item, auto index) { arr[index + size1_c] = std::forward<decltype(item)>(item) ; }) ;
    
    return arr ;
  }
}

#endif