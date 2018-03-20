#ifndef __thodd_stream_utils_hpp__
#  define __thodd_stream_utils_hpp__

namespace thodd::stream {

  /**
   * Returns true if the first stream starts with 
   * the sequence of the second stream by comparator
   */
  bool start_with (auto begin, auto end, auto sbegin, auto send, auto comparator) {
    while (begin != end && sbegin != send && comparator(*begin, *sbegin)) {
      ++ begin ; ++ sbegin ;   
    }

    return sbegin == send ;
  }

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
  auto begin (list<type_t> const & l) {
    return l.first ;
  }

  template <typename type_t>
  auto end (list<type_t> const &) {
    return typename list<type_t>::iterator{} ;
  }

  template <typename type_t>
  auto size(list<type_t> const & l) {
    return l.size ;
  }

  template <typename type_t>
  auto next (std::ptr<node<type_t>> it, size_t pas = 1u) {
    while (pas-- > 0) it = it ? it->next : it ;
    return it ;
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

  template <typename type_t>
  void for_each (list<type_t> const & l, auto func) {
    auto cursor = begin(l) ;

    while (cursor != end(l)) {
      func(cursor->data) ;
      cursor = next(cursor) ;
    }
  }

  template <typename type_t>
  auto const filter (list<type_t> const & l, auto predicate) {
    list<type_t> ln ;
    auto cursor = begin(l) ;

    while (cursor != end(l)) {
      ln = predicate(cursor->data) ? push(ln, cursor->data) : ln ;
      cursor = next(cursor) ;
    }

    return ln ;
  }

  template <typename type_t>
  auto map (list<type_t> const & l, auto mapper) -> list<decltype(mapper(l.first->data))> {
    list<decltype(mapper(l.first->data))> ln ;
    auto cursor = begin(l) ;

    while (cursor != end(l)) {
      ln = push(ln, mapper(cursor->data)) ;
      cursor = next(cursor) ;
    }

    return ln ;
  }

  template <typename type_t>
  auto reduce (list<type_t> const & l, auto const & init, auto reducer) {
    auto acc = init ;
    auto cursor = begin(l) ;

    while (cursor != end(l)) {
      acc = reducer(acc, cursor->data) ;
      cursor = next(cursor) ;
    }

    return acc ;
  }

  template <typename type_t, typename type2_t>
  auto start_with(list<type_t> const & l, list<type2_t> const & l2) {
    while (begin != end && sbegin != send && comparator(*begin, *sbegin)) {
      ++ begin ; ++ sbegin ;   
    }

    return sbegin == send ;
  }
}

#endif