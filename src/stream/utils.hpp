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

#endif