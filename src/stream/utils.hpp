#ifndef __thodd_stream_utils_hpp__
#  define __thodd_stream_utils_hpp__

namespace thodd::stream {


  /**
   * Returns true if the first stream starts with 
   * the sequence of the second stream by equality 
   */
  bool start_with (auto begin, auto end, auto sbegin, auto send) {
    while (begin != end && sbegin != send && *begin == *sbegin) {
      ++ begin ; ++ sbegin ;   
    }

    return sbegin == send ;
  }

  /**
   * Returns new container based on type of c1 that contains 
   * items from c1 and c32 
   */
  auto concat(auto const & c1, auto const & c2) {
    auto c3 = c1 ; 
    std::copy(c2.begin(), c2.end(), std::back_inserter(c3)) ;
    return c3 ;
  }
}


#include <optional>
#include <memory>

namespace std {
  template <typename type_t>
  using ptr = std::shared_ptr<type_t> ;

  template <typename type_t>
  inline auto make_ptr(auto && ... args) {
    return std::make_shared<type_t>(static_cast<decltype(args)&&>(args)...) ;
  }
}

namespace container {
  
  template <typename type_t>
  struct node {
    type_t data ;
    std::ptr<node> next ;
  } ;

  template <typename type_t>
  struct list {
    using iterator = std::ptr<node<type_t>> ;
    iterator first ;
    iterator last ; 
    size_t size{0u} ; 
  } ;

  template <typename type_t>
  typename list<type_t>::iterator begin (list<type_t> const & l) {
    return l.first ;
  }

  template <typename type_t>
  typename list<type_t>::iterator end (list<type_t> const &) {
    return {} ;
  }

  size_t size(list<auto> const & l) {
    return l.size ;
  }

  template <typename type_t>
  auto next (std::ptr<node<type_t>> it) {
    return it ? it->next : it ;
  }

  template <typename type_t>
  type_t const & get (std::ptr<node<type_t>> it) {
    return it->data ;
  }

  template <typename type_t>
  list<type_t> const push (list<type_t> const & l, type_t const & item) {
    auto n     = std::make_ptr<node<type_t>>(node<type_t>{item, nullptr}) ;
    auto last  = l.last ;
    
    if (!l.first) {
      return {n, n, l.size + 1} ;
    } else {
      last->next = n ;
      return {l.first, n, l.size + 1} ;
    }
  }

  template <typename container_t>
  void for_each (container_t const & l, auto func) {
    auto cursor    = begin(l) ;
    auto const limit = end(l) ;

    while (cursor != limit) {
      func(get(cursor)) ;
      cursor = next(cursor) ;
    }
  }

  template <typename container_t>
  auto const filter (container_t const & l, auto predicate) {
    container_t ln ;
    auto cursor    = begin(l) ;
    auto const limit = end(l) ;

    while (cursor != limit) {
      ln     = predicate(get(cursor)) ? push(ln, get(cursor)) : ln ;
      cursor = next(cursor) ;
    }

    return ln ;
  }

  template <template <typename...> typename container_t, 
            typename type_t, 
            typename ... other_t>
  auto map (container_t<type_t, other_t...> const & l, auto mapper) {
    container_t<decltype(mapper(get(begin(l))))> ln ;
    auto cursor    = begin(l) ;
    auto const limit = end(l) ;

    while (cursor != limit) {
      ln     = push(ln, mapper(get(cursor))) ;
      cursor = next(cursor) ;
    }

    return ln ;
  }

  template <typename container_t>
  auto reduce (container_t const & l, auto const & init, auto reducer) {
    auto acc       = init ;
    auto cursor    = begin(l) ;
    auto const limit = end(l) ;

    while (cursor != limit) {
      acc    = reducer(acc, get(cursor)) ;
      cursor = next(cursor) ;
    }

    return acc ;
  }
}

#endif