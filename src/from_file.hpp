#ifndef __thodd_from_file_hpp__
#  define __thodd_from_file_hpp__

#  include <string>
#  include <string_view>
#  include <fstream> 
#  include <iterator>


std::string const
from_file (std::string_view filename) {
  std::fstream thodd_file (filename.data(), std::ios::in) ;
 
  return thodd_file.is_open() ?
    std::string(std::istreambuf_iterator<char> (thodd_file), std::istreambuf_iterator<char> ()) :
    std::string() ;
}

#endif