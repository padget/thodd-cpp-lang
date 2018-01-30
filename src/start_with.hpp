#ifndef __thodd_start_with_hpp__
#  define __thodd_start_with_hpp__

bool start_with (auto begin, auto end, 
                 auto sbegin, auto send, 
                 auto comparator) {
  while (begin != end && sbegin != send && comparator(*begin, *sbegin)) {
    ++ begin ; ++ sbegin ;   
  }

  return sbegin == send ;
}

#endif