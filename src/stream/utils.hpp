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
}

#endif