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
    std::ptr<node<type_t>> first ;
    std::ptr<node<type_t>> last ; 
    size_t size{0u} ; 
  } ;

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
    auto cursor = l.first ;

    while (cursor) {
      func(cursor->data) ;
      cursor = cursor->next ;
    }
  }

  template <typename type_t>
  list<type_t> const filter (list<type_t> const & l, auto predicate) {
    list<type_t> ln ;
    auto cursor = l.first ;

    while (cursor) {
      ln = predicate(cursor->data) ? push(ln, cursor->data) : ln ;
      cursor = cursor->next ;
    }

    return ln ;
  }

  template <typename type_t>
  auto map (list<type_t> const & l, auto mapper) -> list<decltype(mapper(l.first->data))> {
    list<decltype(mapper(l.first->data))> ln ;
    auto cursor = l.first ;

    while (cursor) {
      ln = push(ln, mapper(cursor->data)) ;
      cursor = cursor->next ;
    }

    return ln ;
  }

  template <typename type_t>
  auto reduce(list<type_t> const & l, auto const & init, auto reducer) {
    auto acc = init ;
    auto cursor = l.first ;

    while (cursor) {
      acc = reducer(acc, cursor->data) ;
      cursor = cursor->next ;
    }

    return acc ;
  }
}

#endif